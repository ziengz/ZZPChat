#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "const.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include <string>

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerBuilder;
using grpc::Server;

using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;
using message::LoginReq;
using message::LoginRsp;

struct ChatServer {
	std::string host;
	std::string port;
	std::string name;
	int con_count;
};

class StatusServiceImpl final: public StatusService::Service
{
public:
	StatusServiceImpl();
	Status GetChatServer(ServerContext* context, const GetChatServerReq* request, 
		GetChatServerRsp* reply) override;

	Status Login(ServerContext* context, const LoginReq* request, LoginRsp* reply) override;

private:
	void insertToken(int  uid, std::string token);
	std::unordered_map<std::string ,ChatServer> servers_;
	//std::vector<ChatServer> servers_;
	std::unordered_map<int, std::string>tokens_;
	ChatServer getChatServer();
	std::mutex mutex_;

};

