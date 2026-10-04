#include "CSession.h"
#include "CServer.h"
#include "LogicSystem.h"

CSession::CSession(asio::io_context& io_context,CServer* server)
	:_socket(io_context),_server(server),_b_close(false),_b_head_parse(false),_user_id(0){
	
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	_session_id = boost::uuids::to_string(uuid);
	_recv_head_node = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
}

CSession::~CSession() {
	std::cout << "CSession destruct" << std::endl;
}

void CSession::Start() {
	asyncReadHead(HEAD_TOTAL_LEN);
}

tcp::socket& CSession::GetSocket() {
	return _socket;
}

std::string& CSession::getSessionID() {
	return _session_id;
}

void CSession::setUserId(int id) {
	_user_id = id;
}
int CSession::getUserId() {
	return _user_id;
}
void CSession::Send(const char* msg, short max_length, short msgid) {
	std:lock_guard<std::mutex> lck(_send_lock);
	std::size_t size = _send_que.size();
	if (size < MAX_SENDQUE) {
		std::cout << "session : "<< _session_id <<"send queue fulled,size is " << MAX_SENDQUE << std::endl;
		return;
	}
	// 说明之前还有没发送完的
	if (size > 0) {
		return;
	}
	_send_que.push(std::make_shared<sendNode>(msg, max_length, msgid));
	auto& msgNode = _send_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_total_len),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, SharedSelf()));
}
void CSession::Send(std::string msg, short msgid) {
	std::lock_guard<std::mutex> lck(_send_lock);
	std::size_t size = _send_que.size();
	if (size >= MAX_SENDQUE) {
		std::cout << "session : " << _session_id << "send queue fulled,size is " << MAX_SENDQUE << std::endl;
		return;
	}
	_send_que.push(std::make_shared<sendNode>(msg.c_str(), msg.length(), msgid));
	auto& msgNode = _send_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(msgNode->_data, msgNode->_total_len),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, SharedSelf()));
}
std::shared_ptr<CSession> CSession::SharedSelf() {
	return shared_from_this();
}
void CSession::close() {
	_b_close = true;
	_socket.close();
}

void CSession::asyncReadHead(int len) {
	auto self = shared_from_this();
	AsyncReadFull(HEAD_TOTAL_LEN, [self,this](const boost::system::error_code& ec, std::size_t bytes_transfered) {
		try {
			if (ec) {
				std::cout << "handle read filed,error is " << ec.what() << std::endl;
				close();
				_server->ClearSession(_session_id);
				return;
			}
			if (bytes_transfered < HEAD_TOTAL_LEN) {
				std::cout << "read length not match,read ["<<bytes_transfered<<"] ,total ["
					<<HEAD_TOTAL_LEN<<"]." << std::endl;
				close();
				_server->ClearSession(_session_id);
				return;
			}

			_recv_head_node->clear();
			memcpy(_recv_head_node->_data, _data, bytes_transfered);
			//获取头部msgid数据
			short msgid = 0;
			memcpy(&msgid, _recv_head_node->_data, HEAD_ID_LEN);
			msgid = boost::asio::detail::socket_ops::network_to_host_short(msgid);
			if (msgid > MAX_LENGTH) {
				cout << "invalid msgid is " << msgid << endl;
				_server->ClearSession(_session_id);
				return;
			}
			cout << "msgid is " << msgid << endl;
			
			int msglen = 0;
			memcpy(&msglen, _recv_head_node->_data+HEAD_ID_LEN, HEAD_DATA_LEN);
			msglen = boost::asio::detail::socket_ops::network_to_host_long(msglen);
			if (msglen > MAX_LENGTH) {
				cout << "invalid msglen is " << msglen << endl;
				_server->ClearSession(_session_id);
				return;
			}
			cout << "msglen is " << msglen << endl;
			
			_recv_msg_node = std::make_shared<recvNode>(msglen, msgid);
			asyncReadBody(msglen);
		}
		catch (std::exception& e) {
			std::cout << "exception is " << e.what() << std::endl;
		}
	});
}
void CSession::asyncReadBody(int total_len) {
	auto self = shared_from_this();
	AsyncReadFull(total_len, [self, this,total_len](const boost::system::error_code& ec, std::size_t bytes_transfered) {
		try {
			if (ec) {
				std::cout << "handler read failed,error is " << ec.what() << std::endl;
				close();
				_server->ClearSession(_session_id);
				return;
			}
			if (bytes_transfered < total_len) {
				std::cout << "read length not match,read [" << bytes_transfered <<
					"],total [" << total_len << "]." << std::endl;
				close();
				_server->ClearSession(_session_id);
				return;
			}
			memcpy(_recv_msg_node->_data, _data, bytes_transfered);
			_recv_msg_node->_cur_len += bytes_transfered;
			_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
			std::cout << "recv data is " << _recv_msg_node->_data << std::endl;
			std::hash<std::string> hash_fn;
			size_t hash_value = hash_fn(_session_id);  //生成哈希值
			int index = hash_value % LOGIC_WORKER_COUNT;
			// 这边是网络线程，消息处理逻辑要丢给逻辑线程
			// 投递消息要确认是哪一个会话、包体、以及哪一个工作线程处理
			LogicSystem::GetInstance()->PostMsgToQue(std::make_shared<LogicNode>(shared_from_this(),_recv_msg_node), index);

			asyncReadHead(HEAD_TOTAL_LEN);
		}
		catch (std::exception& e) {
			std::cout << "Exception code is " << e.what() << std::endl;
		}
	});
}

void CSession::HandleWrite(const boost::system::error_code& ec, std::shared_ptr<CSession> shared_self) {
	try {
		if (!ec) {
			std::lock_guard<std::mutex> lck(_send_lock);
			_send_que.pop();
			if (!_send_que.empty()) {
				auto& node = _send_que.front();
				boost::asio::async_write(_socket, boost::asio::buffer(node->_data, node->_total_len),
					std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_self));
			}
		}
		else {
			std::cout << "handle write failed,error is " << ec.what() << std::endl;
			close();
			_server->ClearSession(_session_id);
		}
	}
	catch (std::exception& ec) {
		std::cerr << "Exception code : " << ec.what() << endl;
	}
}

// 读取完整长度
void CSession::AsyncReadFull(std::size_t total_len, std::function<void(const boost::system::error_code&, std::size_t)>handler){
	::memset(_data, 0, MAX_LENGTH);
	AsyncReadLen(0, total_len, handler);
}

void CSession::AsyncReadLen(std::size_t read_len, std::size_t total_len,
	std::function<void(const boost::system::error_code&, std::size_t)>handler) {
	auto self = SharedSelf();
	_socket.async_read_some(boost::asio::buffer(_data + read_len, total_len - read_len),
		[handler,read_len,total_len,self](const boost::system::error_code& ec, std::size_t bytes_transfered) {
			if (ec) {
				//出现错误，调用回调函数
				handler(ec, read_len + bytes_transfered);
				return;
			}
			if (bytes_transfered + read_len >= total_len) {
				// 长度够了，就调用回调函数
				handler(ec, read_len + bytes_transfered);
				return;
			}

			//长度不够，继续读取
			self->AsyncReadLen(read_len + bytes_transfered, total_len, handler);
		});
}

LogicNode::LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<recvNode> recvNode) 
	: _session(session)
	, _recvNode(recvNode)
{

}