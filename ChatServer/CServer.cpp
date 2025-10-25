#include "CServer.h"


CServer::CServer(asio::io_context& ioc, unsigned short port):_ioc(ioc),port_(port),
_acceptor(ioc,tcp::endpoint(tcp::v4(),port))
{
	std::cout << "Server start success,listen to port:" << port << std::endl;
	StartAccept();
}

CServer::~CServer()
{
	std::cout << "destory CServer" << std::endl;
	return;
}

void CServer::ClearSession(std::string uuid)
{
	std::lock_guard<std::mutex>lcok(mutex_);
	if (_sessions.find(uuid) == _sessions.end()) {
		//移除用户和session的关联

	}
	_sessions.erase(uuid);

}

void CServer::StartAccept()
{
	auto& ioc = IOContextPool::GetIntance()->GetIO_Context();
	std::shared_ptr<Session> new_session = std::make_shared<Session>(ioc, this);
	_acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
}

void CServer::HandleAccept(std::shared_ptr<Session>new_session, const system::error_code& error)
{
	if (!error) {
		new_session->Start();
		std::lock_guard<std::mutex>lock(mutex_);
		_sessions.insert(std::make_pair(new_session->GetUuid(), new_session));
	}
	else {
		std::cout << "session accept failed,error is " << error << std::endl;
	}
	StartAccept();

}
