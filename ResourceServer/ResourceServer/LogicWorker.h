#pragma once
#include "const.h"
#include "MsgNode.h"
#include <functional>

class CSession;
class LogicNode {
public:
	LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<recvNode> recvNode);
	std::shared_ptr<CSession> _session;
	std::shared_ptr<recvNode> _recvNode;
};

typedef std::function<void(std::shared_ptr<CSession>,const short& msgid,const std::string& msg_data)> FunCallBack;

class LogicWorker
{
public:
	LogicWorker();
	~LogicWorker();

	void PostTask(std::shared_ptr<LogicNode> logicNode);
	void RegisterCallBack();

private:
	void task_callback(std::shared_ptr<LogicNode> logicNode);
private:
	std::queue<std::shared_ptr<LogicNode>> _task_que;
	std::thread _thread;
	std::condition_variable _cv;
	std::atomic<bool> _b_stop;
	std::mutex _mtx;
	std::unordered_map<short, FunCallBack> _fun_callbacks;
};

