#include "VarifyGrpcClient.h"

RPConPool::RPConPool(std::size_t size, std::string host, std::string port)
	:size_(size),host_(host),port_(port)
{
	for (int i = 0; i < size_; i++) {
		auto channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
		_stubQueue.push(VarifyService::NewStub(channel));
	}
}
