#pragma once
#include "const.h"
#include "Singleton.h"

class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandle;
class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem() {}
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);
	bool HandlePost(std::string, std::shared_ptr<HttpConnection>);
	void RegGet(std::string, HttpHandle handle);
	void RegPost(std::string, HttpHandle handle);
private:
	LogicSystem();
	std::map<std::string, HttpHandle> _post_handles;
	std::map<std::string, HttpHandle> _get_handles;
};

