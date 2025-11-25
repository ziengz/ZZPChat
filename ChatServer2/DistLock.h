#pragma once
#include "const.h"
#include "Singleton.h"
#include <hiredis.h>

class DistLock:public Singleton<DistLock>
{
	friend class Singleton<DistLock>;
public:
	~DistLock();
	std::string acquirLock(redisContext* context, const std::string& LockName, int acquirTimeout, int LockTimeout);
	bool releaseLock(redisContext* context, const std::string& LockName, const std::string& identitify);
private:
	DistLock() = default;
};

