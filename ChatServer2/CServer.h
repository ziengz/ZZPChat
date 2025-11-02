#pragma once
#include "const.h"
#include "IOContextPool.h"
#include "Session.h"


class CServer:public std::enable_shared_from_this<CServer>
{
public:
	CServer(asio::io_context&ioc, unsigned short port);
	~CServer();

	void ClearSession(std::string uuid);
private:
	void StartAccept();
	void HandleAccept(std::shared_ptr<Session>, const system::error_code& error);

	tcp::acceptor _acceptor;
	asio::io_context& _ioc;
	short port_;
	std::map<std::string, std::shared_ptr<Session>> _sessions;
	std::mutex mutex_;
};

