#pragma once
#include "const.h"
#include "MsgNode.h"

class CServer;
class LogicSystem;

typedef std::function<void(const system::error_code&, std::size_t)>FuncCallBack;

//会话层，维护服务器与客户端通过ioc连接
class Session:public std::enable_shared_from_this<Session>
{
public:
	Session(boost::asio::io_context& io_context, CServer* server);
	std::shared_ptr<Session> Shared_self();
	void Send(char* data, std::size_t msg_len, short msg_id);
	void Send(std::string data, short msg_id);
	void HandleWrite(const system::error_code& error, std::shared_ptr<Session>self);
	void Close();
	void Start();
	void AsyncReadBody(int length);
	void AsyncReadHead(int total_len);
	void AsyncReadFull(std::size_t max_len, FuncCallBack handle);
	void AsyncReadLen(std::size_t read_len,std::size_t total_len, FuncCallBack handle);
	std::string GetUuid();
	tcp::socket& GetSocket();
private:
	CServer* server_;
	tcp::socket socket_;
	std::queue<std::shared_ptr<SendNode>>_send_que;
	std::mutex mutex_;
	bool b_stop;
	bool b_head_parse;
	std::shared_ptr<MsgNode> _recv_head_node;
	std::shared_ptr<RecvNode> _recv_msg_node;
	char data_[MAX_LEN];
	std::string _uuid;
};

class LogicNode {
	friend class LogicSystem;
public:
	LogicNode(std::shared_ptr<Session>, std::shared_ptr<RecvNode>);
private:;
	   std::shared_ptr<Session> _session;
	   std::shared_ptr<RecvNode>_recvnode;

};

