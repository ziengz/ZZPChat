#include "StatusGrpcClient.h"

StatusGrpcClient::StatusGrpcClient()
{
	auto& config = ConfigMgr::Instance();
	auto host = config["StatusServer"]["Host"];
	auto port = config["StatusServer"]["Port"];
	pool_.reset(new StatusConPool(5, host, port));
}

StatusGrpcClient::~StatusGrpcClient()
{
}

GetChatServerRsp StatusGrpcClient::GetChatServer(int uid)
{
	GetChatServerReq request;
	GetChatServerRsp reply;
	ClientContext context;
	request.set_uid(uid);
	auto con = pool_->getConnection();
	Status status = con->GetChatServer(&context, request, &reply);
	
	Defer defer([this, &con] {
		pool_->returnConnection(std::move(con));
		});

	if (status.ok()) {
		return reply;
	}
	else{
		reply.set_error(Error_Codes::RPCFailed);
		return reply;
	}
}

LoginRsp StatusGrpcClient::Login(int uid, std::string token)
{
	ClientContext context;
	LoginReq request;
	LoginRsp reply;
	request.set_uid(uid);
	request.set_token(token);
	
	auto stub = pool_->getConnection();
	Status status = stub->Login(&context, request, &reply);
	Defer defer([&stub,this] {
		pool_->returnConnection(std::move(stub));
		});
	if (status.ok()) {
		return reply;
	}
	else {
		reply.set_error(Error_Codes::RPCFailed);
		return reply;
	}
}

