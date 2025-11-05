#include "UserMgr.h"
#include "Session.h"

UserMgr::~UserMgr()
{
	_uid_to_session.clear();
}

std::shared_ptr<Session> UserMgr::getSession(int uid)
{
	std::lock_guard<std::mutex>lock(mutex_);
	auto iter = _uid_to_session.find(uid);
	if (iter == _uid_to_session.end()) {
		return nullptr;
	}
	return iter->second;
}

void UserMgr::SetUserSession(int uid, std::shared_ptr<Session> session)
{
	std::lock_guard<std::mutex>lock(mutex_);
	_uid_to_session[uid] = session;
}

void UserMgr::RMVUserSession(int uid, std::string session_id)
{
	std::lock_guard<std::mutex>lock(mutex_);
	auto find_iter = _uid_to_session.find(uid);
	if (find_iter == _uid_to_session.end()) {
		return;
	}
	auto _session_id = find_iter->second->GetSessionid();
	if (_session_id != session_id) {
		//不相等说明是其他地方登录该账号，因为session_id是随机生成字符串
		return;
	}
	_uid_to_session.erase(uid);

}

UserMgr::UserMgr()
{
}
