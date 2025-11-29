#include "Session.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "CServer.h"
#include "LogicSystem.h"
#include "RedisMgr.h"

Session::Session(boost::asio::io_context& io_context, CServer* server) :
	socket_(io_context),b_stop(false),b_head_parse(false),server_(server),_uid(0)
{
	boost::uuids::uuid a_uuid = uuids::random_generator()();
	_session_id = boost::uuids::to_string(a_uuid);
	_recv_head_node = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
	
	_last_heartbeat = std::time(nullptr);
}

std::string Session::GetSessionid() {
	return _session_id;
}

void Session::SetUserId(int uid)
{
	_uid = uid;
}

int Session::GetUserId()
{
	return _uid;
}

tcp::socket& Session::GetSocket()
{
	return socket_;
}

bool Session::IsHeartbeatExpired(std::time_t& now)
{
	double diff_sec = std::difftime(now, _last_heartbeat);
	if (diff_sec > 20) {
		std::cout << "heartbeat expired,session id is " << _session_id << std::endl;
		return true;
	}
	return false;
}

void Session::DealExceptionSession() {
	auto self = Shared_self();
	//使用分布式锁
	auto uid_str = std::to_string(_uid);
	std::string lock_key = LOCK_PREFIX + uid_str;
	auto identifier = RedisMgr::GetIntance()->acquirLock(lock_key, LOCK_TIME_OUT, ACQUIRE_TIME_OUT);
	Defer defer([identifier,this,lock_key,self]() {
		server_->ClearSession(_session_id);
		RedisMgr::GetIntance()->releaseLock(lock_key, identifier);
	});
	if (identifier.empty()) {
		return;
	}
	std::string redis_session_id = "";
	auto b_success = RedisMgr::GetIntance()->Get(USER_SESSION_PREFIX + uid_str, redis_session_id);
	if (!b_success) {
		return;
	}
	//不相同则说明其他地方登录账号
	if (redis_session_id != _session_id) {
		return;
	}
	RedisMgr::GetIntance()->Del(USER_SESSION_PREFIX + uid_str);
	RedisMgr::GetIntance()->Del(USERIPPREFIX + uid_str);
}

void Session::UpdataHeartbeat() {
	std::time_t now = std::time(nullptr);
	_last_heartbeat = now;
}

void Session::Start() {
	AsyncReadHead(HEAD_TOTAL_LEN);
}

std::shared_ptr<Session> Session::Shared_self()
{
	return shared_from_this();
}

void Session::Send(char* data, std::size_t msg_len, short msg_id)
{
	std::lock_guard<std::mutex>lock(mutex_);
	int send_que_size = _send_que.size();
	if (send_que_size > MAX_SENDQUE) {
		std::cout << "send_que size is falled,size is " << MAX_SENDQUE << std::endl;
		server_->ClearSession(_session_id);
		return;
	}
	//如果发送队列中还残有数据则返回
	if (send_que_size > 0) {
		return;
	}
	_send_que.push(std::make_shared<SendNode>(data, msg_len, msg_id));
	auto msgnode = _send_que.front();
	asio::async_write(socket_, asio::buffer(msgnode->data_,msgnode->total_len_),
		std::bind(&Session::HandleWrite, this, std::placeholders::_1, Shared_self()));
}

void Session::Send(std::string data, short msg_id)
{
	std::lock_guard<std::mutex>lock(mutex_);
	int send_que_size = _send_que.size();
	if (send_que_size > MAX_SENDQUE) {
		std::cout << "send que is falled,size is " << MAX_SENDQUE << std::endl;
		server_->ClearSession(_session_id);
		return;
	}
	//如果大于0，说明队列中还残有数据未发送
	if (send_que_size > 0) {
		return;
	}
	_send_que.push(std::make_shared<SendNode>(data.c_str(), data.length(), msg_id));
	auto msgnode = _send_que.front();
	asio::async_write(socket_, asio::buffer(msgnode->data_, msgnode->total_len_),
		std::bind(&Session::HandleWrite, this, std::placeholders::_1, Shared_self()));
}

void Session::HandleWrite(const system::error_code& error, std::shared_ptr<Session> self)
{
	try {
		auto self = shared_from_this();
		if (!error) {
			//因为Send()函数已经发送了头节点，所以删掉
			_send_que.pop();
			if (!_send_que.empty()) {
				auto msgnode = _send_que.front();
				asio::async_write(socket_, asio::buffer(msgnode->data_, msgnode->total_len_),
					std::bind(&Session::HandleWrite, this, std::placeholders::_1, self));
			}
		}
		else {
			std::cout << "handle write failed, error is " << error.what() << std::endl;
			DealExceptionSession();
			Close();
		}
	}
	catch (system::error_code& e) {
		std::cout << "Exception is " << e.what() << std::endl;
		Close();
		return;
	}
}

void Session::Close() {
	std::lock_guard<std::mutex>lock(mutex_);
	socket_.close();
	b_stop = true;
}

//std::function<void(system::error_code&, std::size_t)>FuncCallBack;
void Session::AsyncReadHead(int total_len)
{
	auto self = shared_from_this();
	AsyncReadFull(total_len, [self,this](const system::error_code& error, std::size_t bytes_transfered) {
		try {
			if (error) {
				std::cout << "read data failed,exception is " << error.what() << std::endl;
				Close();
				DealExceptionSession();
				return;
			}
			if (bytes_transfered < HEAD_TOTAL_LEN) {
				std::cout << "read length not match, read [" << bytes_transfered << "] , total ["
					<< HEAD_TOTAL_LEN << "]" << std::endl;
				server_->ClearSession(_session_id);
				Close();
				return;
			}
			short msg_id = 0;
			_recv_head_node->Clear();
			//std::cout << data_ << std::endl;
			memcpy(_recv_head_node->data_, data_, HEAD_TOTAL_LEN);

			memcpy(&msg_id, _recv_head_node->data_, HEAD_ID_LEN);
			msg_id = asio::detail::socket_ops::network_to_host_short(msg_id);
			std::cout << "msg id is " << msg_id << std::endl;
			
			if (msg_id > MAX_LEN) {
				std::cout << "invalid msg_id is " << msg_id << std::endl;
				server_->ClearSession(_session_id);
				return;
			}

			std::size_t length = 0;
			memcpy(&length, _recv_head_node->data_ + HEAD_ID_LEN, HEAD_DATA_LEN);
			length = asio::detail::socket_ops::network_to_host_short(length);
			std::cout << "length id is " << length << std::endl;
			if (length > MAX_LEN) {
				std::cout << "invalid msg_length is " << length << std::endl;
				server_->ClearSession(_session_id);
				return;
			}
			_recv_msg_node = std::make_shared<RecvNode>(length, msg_id);
			AsyncReadBody(length);
		}
		catch (std::exception&e) {
			std::cout << "exception is " << e.what() << std::endl;
			return;
		}
	});	
}

void Session::AsyncReadBody(int length)
{
	auto self = shared_from_this();
	AsyncReadFull(length, [this,length](const system::error_code& e, std::size_t byte_transfered) {
		try {
			if (e) {
				std::cout << "read data failed,error is " << e.what() << std::endl;
				Close();
				DealExceptionSession();
				return;
			}
			if (byte_transfered < length) {
				std::cout << "read length not match, read [" << byte_transfered << "] , total ["
					<< length << "]" << std::endl;
				server_->ClearSession(_session_id);
				Close();
				return;
			}

			//判断连接是否有效
			if (!server_->CheckVaild(_session_id)) {
				Close();
				return;
			}

			_recv_msg_node->Clear();
			memcpy(_recv_msg_node->data_, data_, length);
			_recv_msg_node->cur_len_ += byte_transfered;
			_recv_msg_node->data_[_recv_msg_node->total_len_] = '\0';
			std::cout << "recv_msg is " << _recv_msg_node->data_ << std::endl;

			//更新session心跳时间
			UpdataHeartbeat();
			//此处将消息投递到逻辑队列中，给客户端回包
			LogicSystem::GetIntance()->PostMsgToQue(std::make_shared<LogicNode>(Shared_self(),_recv_msg_node));
			AsyncReadHead(HEAD_TOTAL_LEN);
		}
		catch (std::exception& e) {
			std::cout << "exception is " << e.what() << std::endl;
			return;
		}
		});
}

void Session::AsyncReadFull(std::size_t max_len, FuncCallBack handle)
{
	memset(data_, 0, MAX_LEN);
	AsyncReadLen(0,max_len, handle);
}

//读取指定长度
void Session::AsyncReadLen(std::size_t read_len, std::size_t total_len, FuncCallBack handle)
{
	try {
		auto self = shared_from_this();

		socket_.async_read_some(asio::buffer(data_ + read_len, total_len - read_len),
			[self,read_len, total_len, handle](const system::error_code& error, std::size_t bytes_transfered) {
				if (error) {
					handle(error, read_len + bytes_transfered);
					return;
				}
				if (read_len + bytes_transfered >= HEAD_TOTAL_LEN) {
					handle(error, read_len + bytes_transfered);
					return;
				}
				self->AsyncReadLen(read_len + bytes_transfered, total_len, handle);
			});
	}
	catch (std::exception& e) {
		std::cout << "Exception is " << e.what() << std::endl;
		return;
	}
}

void Session::NotifyOffline(int uid) {
	Json::Value root;
	root["error"] = Error_Codes::Success;
	root["uid"] = uid;
	
	std::string return_str = root.toStyledString();
	Send(return_str, ID_NOTIFY_OFF_LINE_REQ);
	return;
}

LogicNode::LogicNode(std::shared_ptr<Session>session, std::shared_ptr<RecvNode>recvnode):_session(session),_recvnode(recvnode)
{
}

