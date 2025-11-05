#pragma once
#include <memory>
#include <unordered_map>
#include "Singleton.h"
#include <mutex>

class Session;
class UserMgr:public Singleton<UserMgr>
{
	friend class Singleton<UserMgr>;
public:
	~UserMgr();
	std::shared_ptr<Session> getSession(int uid);
	void SetUserSession(int uid, std::shared_ptr<Session>session);
	void RMVUserSession(int uid, std::string session_id);


private:
	UserMgr();
	std::mutex mutex_;
	std::unordered_map<int, std::shared_ptr<Session>> _uid_to_session;
};

