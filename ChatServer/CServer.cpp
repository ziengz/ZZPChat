#include "CServer.h"
#include "UserMgr.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"


CServer::CServer(asio::io_context& ioc, unsigned short port):_ioc(ioc),port_(port),
_acceptor(ioc,tcp::endpoint(tcp::v4(),port)),_timer(_ioc,std::chrono::seconds(60))
{
	std::cout << "Server start success,listen to port:" << port << std::endl;
	StartAccept();
}

CServer::~CServer()
{
	std::cout << "destory CServer" << std::endl;
	return;
}

void CServer::ClearSession(std::string session_id)
{
	std::lock_guard<std::mutex>lcok(mutex_);
	if (_sessions.find(session_id) == _sessions.end()) {
		//移除用户和session的关联
		auto uid = _sessions[session_id]->GetUserId();
		UserMgr::GetIntance()->RMVUserSession(uid, session_id);
	}
	_sessions.erase(session_id);

}

bool CServer::CheckVaild(std::string uuid)
{
	auto iter = _sessions.find(uuid);
	if (iter != _sessions.end()) {
		return true;
	}
	return false;
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
		_sessions.insert(std::make_pair(new_session->GetSessionid(), new_session));
	}
	else {
		std::cout << "session accept failed,error is " << error << std::endl;
	}
	StartAccept();

}

void CServer::on_timer(const boost::system::error_code& ec)
{
	if (ec) {
		std::cout << "timer error" << ec.message() << std::endl;
		return;
	}
	std::vector<std::shared_ptr<Session>> _expired_sessions;
	int session_count = 0;
	//线程锁拷贝_sessions
	std::map<std::string, std::shared_ptr<Session>>session_copy;
	{
		std::lock_guard<std::mutex>lock(mutex_);
		session_copy = _sessions;
	}
	std::time_t now = std::time(nullptr);
	for (auto iter = session_copy.begin(); iter != session_copy.end(); iter++) {
		auto b_expired = iter->second->IsHeartbeatExpired(now);
		if (b_expired) {
			iter->second->Close();
			_expired_sessions.push_back(iter->second);
			continue;
		}
		session_count++;
	}

	//设置session数量
	auto& cfg = ConfigMgr::Instance();
	auto self_name = cfg["SelfServer"]["Name"];
	auto count_str = std::to_string(session_count);
	RedisMgr::GetIntance()->HSet(LOGIN_COUNT, self_name, count_str);

	//处理过期session，单独提出，防止死锁
	for (auto& session : _expired_sessions) {
		session->DealExceptionSession();
	}
	
	_timer.expires_after(std::chrono::seconds(60));
	_timer.async_wait([this](boost::system::error_code ec) {
		on_timer(ec);
	});


}

void CServer::StartTimer()
{
	auto self = shared_from_this();
	//使用智能指针，引用计数最少为1，在这里+1，stop之后也不至于因为引用计数为0而出现异常
	_timer.async_wait([self](boost::system::error_code ec) {
		self->on_timer(ec);
	});
}

void CServer::StopTimer()
{
	_timer.cancel();
}
