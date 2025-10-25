#include "Session.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "CServer.h"

Session::Session(boost::asio::io_context& io_context, CServer* server) :
	socket_(io_context),b_stop(false),b_head_parse(false),server_(server)
{
	boost::uuids::uuid a_uuid = uuids::random_generator()();
	_uuid = boost::uuids::to_string(a_uuid);
	_recv_head_node = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
}

std::string Session::GetUuid() {
	return _uuid;
}

tcp::socket& Session::GetSocket()
{
	return socket_;
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
		server_->ClearSession(_uuid);
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
		server_->ClearSession(_uuid);
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
				return;
			}
			if (bytes_transfered < HEAD_TOTAL_LEN) {
				std::cout << "read length not match, read [" << bytes_transfered << "] , total ["
					<< HEAD_TOTAL_LEN << "]" << std::endl;
				server_->ClearSession(_uuid);
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
				server_->ClearSession(_uuid);
				return;
			}

			std::size_t length = 0;
			memcpy(&length, _recv_head_node->data_ + HEAD_ID_LEN, HEAD_DATA_LEN);
			length = asio::detail::socket_ops::network_to_host_short(length);
			std::cout << "length id is " << length << std::endl;
			if (length > MAX_LEN) {
				std::cout << "invalid msg_length is " << length << std::endl;
				server_->ClearSession(_uuid);
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
				return;
			}
			if (byte_transfered < length) {
				std::cout << "read length not match, read [" << byte_transfered << "] , total ["
					<< length << "]" << std::endl;
				server_->ClearSession(_uuid);
				Close();
				return;
			}

			_recv_msg_node->Clear();
			memcpy(_recv_msg_node->data_, data_, length);
			_recv_msg_node->cur_len_ += byte_transfered;
			_recv_msg_node->data_[_recv_msg_node->total_len_] = '\0';
			std::cout << "recv_msg is " << _recv_msg_node->data_ << std::endl;

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

LogicNode::LogicNode(std::shared_ptr<Session>session, std::shared_ptr<RecvNode>recvnode):_session(session),_recvnode(recvnode)
{
}
