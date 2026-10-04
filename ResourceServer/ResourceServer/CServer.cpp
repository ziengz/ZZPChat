#include "CServer.h"
#include "CSession.h"

CServer::CServer(boost::asio::io_context& io_context,short port):_ioc(io_context),_port(port)
	, _acceptor(_ioc,tcp::endpoint(tcp::v4(),port))
{
	cout << "Server start success,listen on port is " << _port << endl;
	StartAccept();
}

CServer::~CServer() {
	cout << "Server destruct listen on port is " << _port << endl;
}

void CServer::StartAccept() {
	auto& io_context = AsioIOServicePool::GetInstance()->getIOService();
	std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context,this);
	_acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
}

void CServer::HandleAccept(std::shared_ptr<CSession> session, const boost::system::error_code& ec) {
	if (!ec) {
		session->Start();
		std::lock_guard<std::mutex>lck(_mtx);
		_sessions.insert(std::make_pair(session->getSessionID(), session));
	}
	else {
		cout << "session accept failed,error is " << ec.what() << endl;
	}
	StartAccept();
}

void CServer::ClearSession(std::string session_id) {
	// 移除用户和session的关联

	std::lock_guard<std::mutex> lck(_mtx);
	_sessions.erase(session_id);
}
