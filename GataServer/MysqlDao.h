#pragma once
#include "const.h"
#include "ConfigMgr.h"
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/exception.h>

class SqlConnection {
public:
	SqlConnection(sql::Connection* con, int64_t lasttime) :con_(con), last_oper_time_(lasttime) {}
	std::unique_ptr<sql::Connection> con_;
	int64_t last_oper_time_;

};

class MySqlPool {
public:
	MySqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int poolsize) :
		url_(url), user_(user), pass_(pass), schema_(schema), poolsize_(poolsize),b_stop_(false),fail_count_(0) {
		try {
			for (int i = 0; i < poolsize_; i++) {
				sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
				auto * con = driver->connect(url_, user_, pass_);
				con->setSchema(schema_);
				//获取当前时间
				auto currentTime = std::chrono::system_clock::now().time_since_epoch();
				//将时间戳转换为秒
				long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
				pool_.push(std::make_unique<SqlConnection>(con, timestamp));
			}
			check_thread_ = std::thread([this]() {
				while (!b_stop_) {
					//启动一个后台线程，每60秒执行一次函数，用于检查连接的健康状态
					checkConnectionPro();
					std::this_thread::sleep_for(std::chrono::seconds(60));
				}
				
			});
			//子线程置于后台
			check_thread_.detach();
		}
		catch (sql::SQLException&e) {
			std::cout << "mysql pool init failed,error is " << e.what() << std::endl;
		}
	}
	void checkConnectionPro()
	{
		//先读取目标处理数
		size_t targetCount;
		{
			std::lock_guard<std::mutex> lock(mutex_);
			targetCount = pool_.size();
		}
		//当前已经处理的数量
		size_t preccessed = 0;
		//获取时间
		auto currentTime = std::chrono::system_clock::now().time_since_epoch();
		long long timeStamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();

		bool heathly = true;
		while (preccessed < targetCount) {
			std::unique_ptr<SqlConnection>con;
			{
				std::lock_guard<std::mutex>lock(mutex_);
				if (pool_.empty())
					break;
				con = std::move(pool_.front());
				pool_.pop();
			}
			
			//检查连接是否空闲超过5秒（通过最后一次操作时间判断）。如果是，则执行SELECT 1来检查连接是否健康。
			if (timeStamp - con->last_oper_time_ > 5) {
				try {
					//简单查询用来测试连接有效性
					std::unique_ptr<sql::Statement> state(con->con_->createStatement());
					state->executeQuery("select 1");
					con->last_oper_time_ = timeStamp;
				}
				catch (sql::SQLException&e) {
					//链接已失效
					std::cout << "error keeping connection alive:" << e.what() << std::endl;
					heathly = false;
					fail_count_++;
				}
			}

			//如果健康返回队列中
			if (heathly) {
				std::lock_guard<std::mutex>lock(mutex_);
				pool_.push(std::move(con));
				cond_.notify_one();  //有可等待接口，通知
			}
			//不健康的连接自动销毁

			preccessed++;
		}
		
		//存在失败的连接，重连
		while (fail_count_ > 0) {
			bool b_res = reconnect(timeStamp);
			if (b_res) {
				fail_count_--;
			}
			else {
				break;
			}
		}
	}
	bool reconnect(long long timeStamp) {
		try {
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
			auto* con  =driver->connect(url_, user_, pass_);
			con->setSchema(schema_);

			auto newCon = std::make_unique<SqlConnection>(con, timeStamp);
			
			{
				std::lock_guard<std::mutex>lock(mutex_);
				pool_.push(std::move(newCon));
			}
			std::cout << "mysql connection reconnect success" << std::endl;
			return true;
		}
		catch (sql::SQLException& e) {
			std::cout << "reconnect failed,error is " << e.what() << std::endl;
			return false;
		}
	}
	
	std::unique_ptr<SqlConnection>getConnection()
	{
		std::unique_lock<std::mutex>lock(mutex_);
		cond_.wait(lock, [this]() {
			if (b_stop_) {
				return true;
			}
			return !pool_.empty();
		});
		if (b_stop_) {
			return nullptr;
		}
		
		std::unique_ptr<SqlConnection> con(std::move(pool_.front()));
		pool_.pop();

		return con;
	}

	void returnConnection(std::unique_ptr<SqlConnection>con) {
		std::unique_lock<std::mutex>lock(mutex_);
		if (b_stop_) {
			return;
		}

		auto currentTime = std::chrono::system_clock::now().time_since_epoch();
		long long timeStamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();

		con->last_oper_time_ = timeStamp;
		pool_.push(std::move(con));
		cond_.notify_one();
	}

	void Close()
	{
		b_stop_ = true;
		//关闭之前要唤醒所有线程
		cond_.notify_all();
	}
	~MySqlPool()
	{
		std::lock_guard<std::mutex>lock(mutex_);
		while (!pool_.empty()) {
			pool_.pop();
		}
	}

private:
	std::string url_;
	std::string user_;
	std::string pass_;
	std::string schema_;
	int poolsize_;
	std::queue<std::unique_ptr<SqlConnection>>pool_;
	std::mutex mutex_;
	std::condition_variable cond_;
	std::thread check_thread_;
	std::atomic<bool> b_stop_;
	std::atomic<int> fail_count_;

};

struct UserInfo {
	std::string name;
	std::string pwd;
	int uid;
	std::string email;
};

class MysqlDao
{
public:
	MysqlDao();
	~MysqlDao();
	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
	//int RegUserTransaction(const std::string& name, const std::string& email, const std::string& pwd, const std::string& icon);
	bool CheckEmail(const std::string& name, const std::string& email);
	bool UpdatePwd(const std::string& name, const std::string& newpwd);
	bool CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userInfo);
	//bool TestProcedure(const std::string& email, int& uid, std::string& name);
private:
	std::unique_ptr<MySqlPool> pool_;
};