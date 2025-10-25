#pragma once
#include "const.h"
#include "Singleton.h"
#include "Session.h"
#include "data.h"
#include "MsgNode.h"

typedef  std::function<void(std::shared_ptr<Session>, const short& msg_id, const std::string& msg_data)> FunCallBack;

class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	void PostMsgToQue(std::shared_ptr<LogicNode>msg);

private:
	LogicSystem();
	void DealMsg();
	void RegisterCallBack();
	void LogicHandler(std::shared_ptr<Session> session, const short& msg_id, const std::string& msg_data);

	std::thread _work_thread;
	std::queue<std::shared_ptr<LogicNode>> _msg_que;
	std::map<short, FunCallBack> _fun_callbacks;
	std::map<int, std::shared_ptr<UserInfo>>_users;
	std::condition_variable cond_;
	std::mutex mutex_;
	bool _b_stop;

};

