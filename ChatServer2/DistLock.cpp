#include "DistLock.h"
#include "const.h"

DistLock::~DistLock()
{
}

static std::string generateUUID() {
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	return to_string(uuid);
}

//尝试获取锁，返回锁的唯一标识符（UUID）,如果获取失败则返回空字符
std::string DistLock::acquirLock(redisContext* context, const std::string& LockName, int acquirTimeout, int LockTimeout)
{
	auto identifier = generateUUID();
	std::string lockKey = "lock:" + LockName;
	auto endTime = std::chrono::steady_clock::now() + std::chrono::seconds(acquirTimeout);

	while (std::chrono::steady_clock::now() < endTime) {
		redisReply* reply = (redisReply*)redisCommand(context, "SET %s %s NX EX %d",
			lockKey.c_str(), identifier.c_str(), LockTimeout);
		if (reply != nullptr) {
			if (reply->type == REDIS_REPLY_STATUS && std::string(reply->str) == "OK") {
				freeReplyObject(reply);
				return identifier;
			}
			freeReplyObject(reply);
		}
		//暂停一秒，防止忙等待
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return "";
}

bool DistLock::releaseLock(redisContext* context, const std::string& LockName, const std::string& identitier)
{
	std::string lockKey = "lock:" + LockName;
	std::string luaScript = "if redis.call('get',KEYS[1]) == ARGV[1] then \
								return redis.call('del',KEYS[1]) \
							else \
								return 0 \
							end";
	redisReply* reply = (redisReply*)redisCommand(context, "EVAL %s 1 %s %s",
		luaScript, lockKey.c_str(), identitier.c_str());
	bool success = false;
	if (reply != nullptr) {
		if (reply->type == REDIS_REPLY_STATUS && std::string(reply->str) == "OK") {
			success = true;
		}
		freeReplyObject(reply);
	}
	return success;
}
