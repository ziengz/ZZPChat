#include "StatusServiceImpl.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


StatusServiceImpl::StatusServiceImpl()
{
	auto& config = ConfigMgr::Instance();
	auto server_list = config["chatservers"]["Name"];

	std::vector<std::string> words;
	std::string word;
	std::stringstream ss(server_list);
	while (std::getline(ss, word, ',')) {
		words.push_back(word);
	}
	for(auto& word : words) {
		ChatServer server;
		server.host = config[word]["Host"];
		server.port = config[word]["Port"];
		server.name = config[word]["Name"];
		servers_[word] = server;
	}
}

std::string generate_unique_string() {
	//创建UUID对象
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	//将UUID转换为字符串
	std::string unique_string = boost::uuids::to_string(uuid);
	return unique_string;
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, 
	GetChatServerRsp* reply)
{
	const auto& server = getChatServer();
	reply->set_host(server.host);
	reply->set_port(server.port);
	reply->set_error(Error_Codes::Success);
	reply->set_token(generate_unique_string());
	insertToken(request->uid(), reply->token());
	return Status::OK;
}

Status StatusServiceImpl::Login(ServerContext* context, const LoginReq* request, LoginRsp* reply)
{
	auto uid = request->uid();
	auto token = request->token();
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";
	
	bool success = RedisMgr::GetIntance()->Get(token_key, token_value);
	if (!success) {
		reply->set_error(Error_Codes::UidInvalid);
		std::cout << "uid is invlid" << std::endl;
		return Status::OK;
	}
	if (token_value != token) {
		reply->set_error(Error_Codes::TokenInvalid);
		std::cout << "token is invlid" << std::endl;
		return Status::OK;
	}
	
	std::cout << "uid and token is all valid" << std::endl;
	reply->set_error(Error_Codes::Success);
	reply->set_token(token);
	reply->set_uid(uid);
	return Status::OK;
}

void StatusServiceImpl::insertToken(int uid, std::string token)
{
	//std::lock_guard<std::mutex>lock(mutex_);
	//tokens_[uid] = token;
	
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	RedisMgr::GetIntance()->Set(token_key, token);
}

ChatServer StatusServiceImpl::getChatServer()
{
	//选择连接数量最小的，压力最小，使得负载均衡
	std::lock_guard<std::mutex>lock(mutex_);
	//第一个作为最小基准
	auto minServer = servers_.begin()->second;

	auto lock_key = LOGIN_COUNT;
	//使用登陆数量锁
	auto identify = RedisMgr::GetIntance()->acquirLock(lock_key, LOCK_TIME_OUT, ACQUIRE_TIME_OUT);
	Defer defer([lock_key,identify, this]() {
		RedisMgr::GetIntance()->releaseLock(lock_key, identify);
		});

	auto count_str = RedisMgr::GetIntance()->HGet(LOGIN_COUNT, minServer.name);

	//如果不存在，则默认设置为最大
	if (count_str.empty()) {
		minServer.con_count = INT_MAX;
	}
	else {
		minServer.con_count = std::stoi(count_str);
	}


	for (auto& server : servers_) {
		if (server.second.name == minServer.name) {
			continue;
		}
		auto count_str = RedisMgr::GetIntance()->HGet(LOGIN_COUNT,server.second.name);
		if (count_str.empty()) {
			server.second.con_count = INT_MAX;
		}
		else {
			server.second.con_count = std::stoi(count_str);
		}

		if (server.second.con_count < minServer.con_count) {
			minServer = server.second;
		}
	}
	return minServer;
}
