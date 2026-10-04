#pragma once
#include "const.h"
#include "MsgNode.h"
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <queue>
#include <mutex>


namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

class CServer;
class CSession: public std::enable_shared_from_this<CSession>
{
public:
	CSession(asio::io_context& io_context,CServer* server);
	~CSession();
	tcp::socket& GetSocket();
	void Start();
	std::string& getSessionID();	
	void setUserId(int id);
	int getUserId();
	void Send(const char* msg, short max_length, short msgid);
	void Send(std::string msg, short msgid);
	std::shared_ptr<CSession> SharedSelf();
	void close();

	void HandleWrite(const boost::system::error_code& ec,std::shared_ptr<CSession> shared_self);
	void asyncReadHead(int len);
	void asyncReadBody(int total_len);
private:
	void AsyncReadFull(std::size_t total_len, std::function<void(const boost::system::error_code&, std::size_t)>handler);
	void AsyncReadLen(std::size_t read_len, std::size_t total_len,
		std::function<void(const boost::system::error_code&, std::size_t)>handler);
private:
	tcp::socket _socket;
	std::string _session_id;
	char _data[MAX_LENGTH];
	CServer* _server;

	bool _b_close;
	bool _b_head_parse;
	int _user_id;	
	std::queue<std::shared_ptr<sendNode>> _send_que;

	// ??????????
	std::shared_ptr<MsgNode> _recv_head_node;
	// ??????????
	std::shared_ptr<recvNode> _recv_msg_node;

	std::mutex _send_lock;
};

