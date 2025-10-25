#pragma once
#include "Singleton.h"
#include "const.h"
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "ConfigMgr.h"

using grpc::ClientContext;
using grpc::Status;
using grpc::Channel;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

//ʹ��RPC������߲���
class RPConPool {
public:
	RPConPool(std::size_t size, std::string host, std::string port);
	~RPConPool(){
		std::lock_guard<std::mutex> lock(_mutex);
		Close();
		while (!_stubQueue.empty()) {
			_stubQueue.pop();
		}
	}

	void Close() {
		b_stop_ = true;
		cond_.notify_all();   //�ٹر�֮ǰ֪ͨ�����߳�

	}

	std::unique_ptr<VarifyService::Stub> GetStub()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		
		//����false������ȴ���ֻ�з���trueʱ�����������1���رա�2���п���stub��ʹ�ã����˳��ȴ�
		cond_.wait(lock, [this] {
			if (b_stop_) return true;      
			return !_stubQueue.empty();    
			});
		
		if (b_stop_) {
			return nullptr;
		}
		auto stub = std::move(_stubQueue.front());
		_stubQueue.pop();
		return stub;
	}
	void ReturnStub(std::unique_ptr<VarifyService::Stub> stub) {
		std::lock_guard<std::mutex> lock(_mutex);
		if (b_stop_) {
			return;
		}		
		_stubQueue.push(std::move(stub));
		cond_.notify_one();
	}



private:
	std::atomic<bool> b_stop_;   //���������ݾ������̼߳��ͬ������
	std::size_t size_;
	std::string host_;
	std::string port_;

	std::mutex _mutex;
	std::condition_variable cond_;
	std::queue<std::unique_ptr<VarifyService::Stub>> _stubQueue;
};


class VarifyGrpcClient:public Singleton<VarifyGrpcClient>
{
	friend Singleton<VarifyGrpcClient>;
public:
	GetVarifyRsp GetVarifyCode(std::string email) {
		GetVarifyReq request;
		GetVarifyRsp reply;
		ClientContext context;
		request.set_email(email);

		auto stub_ = pool_->GetStub();
		Status status = stub_->GetVarifyCode(&context, request, &reply);
		if (status.ok())
		{              //unique_ptr��֧�ֿ������죬��֧���ƶ�����
			pool_->ReturnStub(std::move(stub_));
			return reply;
		}
		else {
			pool_->ReturnStub(std::move(stub_));
			reply.set_error(Error_Codes::RPCFailed);
			return reply;
		}
	}

private:
	VarifyGrpcClient() {
		auto config = ConfigMgr::Instance();
		std::string host = config["VarifyServer"]["host"];
		std::string port = config["VarifyServer"]["port"];
		
		//ptr1.reset(); �����ԭʼ�ڴ�Ĺ���
		//ptr2.reset(new int(250)); ����ָ������ָ������ԭʼ�ڴ�
		std::cout << host << " " << port << std::endl;
		pool_.reset(new RPConPool(5, host, port));   
	}
	std::unique_ptr<RPConPool>pool_;
};

