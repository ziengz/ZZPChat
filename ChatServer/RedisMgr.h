#pragma once
#include "const.h"
#include "Singleton.h"
#include "ConfigMgr.h"

class RedisConPool {
public:
    RedisConPool(std::size_t poolSize, const char* host, int port,const char* pass) :
        poolSize_(poolSize), host_(host), port_(port) {
        for (int i = 0; i < poolSize; i++) {
            auto* context = redisConnect(host, port);
            if (context == NULL || context->err) {
                if(context!=NULL)
                    redisFree(context);
                continue;
            }
            auto reply = (redisReply*)redisCommand(context, "AUTH %s", pass);
            if (reply->type == REDIS_REPLY_ERROR) {
                std::cout << "认证失败" << std::endl;
                //执行成功 释放redisCommand执行后返回的redisReply所占用的内存
                freeReplyObject(reply);
                redisFree(context);
                continue;
            }
            //执行成功 释放redisCommand执行后返回的redisReply所占用的内存
            freeReplyObject(reply);
            std::cout << "认证成功" << std::endl;
            redisQueue_.push(context);
        }
    }

    ~RedisConPool() {
        std::lock_guard<std::mutex>lock(mutex_);
        Close();
        while (!redisQueue_.empty()) {
            redisQueue_.pop();
        }
    }

    redisContext* getRedisContext() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] {
            if (b_stop) {
                return true;
            }
            return !redisQueue_.empty();
            });
        if (b_stop) {
            return nullptr;
        }
        auto* context = redisQueue_.front();
        redisQueue_.pop();
        return context;
    }
    void ReturnRedisCon(redisContext* context) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (b_stop) {
            return;
        }
        redisQueue_.push(context);
        cond_.notify_one();
    }
    void Close() {
        b_stop = true;
        cond_.notify_all();
    }

private:
    std::atomic<bool>b_stop;
    const char* host_;
    int port_;
    std::size_t poolSize_;
    std::queue<redisContext*> redisQueue_;
    std::condition_variable cond_;
    std::mutex mutex_;

};

class RedisMgr : public Singleton<RedisMgr>,
    public std::enable_shared_from_this<RedisMgr>
{
    friend class Singleton<RedisMgr>;
public:
    ~RedisMgr();
	// 相当于执行：GET key
    bool Get(const std::string& key, std::string& value);
	// 相当于执行：SET key value
	bool Set(const std::string& key, const std::string& value);
	// 相当于执行：LPUSH key value
	bool LPush(const std::string& key, const std::string& value);
	// 相当于执行：LPOP key
	bool LPop(const std::string& key, std::string& value);
	// 相当于执行：RPUSH key value
	bool RPush(const std::string& key, const std::string& value);
	// 相当于执行：RPOP key
	bool RPop(const std::string& key, std::string& value);
	// 相当于执行：HSET key field value
	bool HSet(const std::string& key, const std::string& hkey, const std::string& value);
	// 相当于执行：HSET key field value (二进制安全版本)
	bool HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen);
	// 相当于执行：HGET key field
	std::string HGet(const std::string& key, const std::string& hkey);
	// 相当于执行：HDEL key field
	bool HDel(const std::string& key, std::string& field);
	// 相当于执行：DEL key
	bool Del(const std::string& key);
	// 相当于执行：EXISTS key
	bool ExistsKey(const std::string& key);
    void Close();

    //获取分布式锁
    std::string acquirLock(const std::string& LockName, int lockTimeout, int acquireTimeout);
    //释放分布式锁
    bool releaseLock(const std::string& LockName, const std::string& identifier);
private:
    RedisMgr();
    std::unique_ptr<RedisConPool> con_pool_;
};

