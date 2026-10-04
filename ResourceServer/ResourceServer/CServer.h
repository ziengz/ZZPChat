#pragma once
#include "AsioIOServicePool.h"
#include <boost/asio.hpp>
#include <map>
#include <mutex>
#include "CSession.h"

using namespace std;
using boost::asio::ip::tcp;


class CServer
{
public:
	CServer(boost::asio::io_context& io_context, short port);
	~CServer();
	void ClearSession(std::string session_id);
private:
	void StartAccept();
	void HandleAccept(std::shared_ptr<CSession> session,const boost::system::error_code& ec);

private:
	boost::asio::io_context& _ioc;
	tcp::acceptor _acceptor;
	short _port;
	std::map<std::string, std::shared_ptr<CSession>> _sessions;
	std::mutex _mtx;
};	

