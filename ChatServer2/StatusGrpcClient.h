#pragma once
#include "const.h"
#include "ConfigMgr.h"
#include "Singleton.h"
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;
using message::LoginReq;
using message::LoginRsp;


class StatusConPool 
{
public:
	StatusConPool(int poolSize,std::string host,std::string port):
		poolSize_(poolSize),host_(host),port_(port),b_stop(false)
	{
		for (int i = 0; i < poolSize_; i++) {
			std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
				grpc::InsecureChannelCredentials());
			connections_.push(StatusService::NewStub(channel));
		}
	}
	std::unique_ptr<StatusService::Stub> getConnection()
	{
		std::unique_lock<std::mutex>lock(mutex_);
		cond_.wait(lock,[this] {
			if (b_stop) {
				return true;
			}
			return !connections_.empty();
			});
		if (b_stop) {
			return nullptr;
		}
		auto con = std::move(connections_.front());
		connections_.pop();
		return con;
	}

	void returnConnection(std::unique_ptr<StatusService::Stub> con) {
		std::unique_lock<std::mutex>lock(mutex_);
		if (b_stop) {
			return;
		}
		connections_.push(std::move(con));
		cond_.notify_one();
	}
	~StatusConPool() {
		std::unique_lock<std::mutex>lock(mutex_);
		Close();
		while (!connections_.empty()) {
			connections_.pop();
		}
	}
	void Close() {
		b_stop = true;
		cond_.notify_all();
	}

private:
	std::size_t poolSize_;
	std::string host_;
	std::string port_;
	std::atomic<bool> b_stop;
	std::condition_variable cond_;
	std::mutex mutex_;
	std::queue<std::unique_ptr<StatusService::Stub>> connections_;

};


class StatusGrpcClient:public Singleton<StatusGrpcClient>
{
	friend class Singleton<StatusGrpcClient>;
public:
	~StatusGrpcClient();
	GetChatServerRsp GetChatServer(int uid);
	LoginRsp Login(int uid, std::string token);

private:
	StatusGrpcClient();
	std::unique_ptr<StatusConPool>pool_;

};

