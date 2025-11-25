#include "ChatGrpcClient.h"
#include "ConfigMgr.h"
#include "MySqlMgr.h"
#include "RedisMgr.h"
#include "Session.h"
#include <string>
#include "ChatServiceImpl.h"

ChatGrpcClient::ChatGrpcClient()
{
    auto& config = ConfigMgr::Instance();
    auto server_list = config["PeerServer"]["Servers"];
    
    std::vector<std::string>words;
    std::stringstream ss(server_list);
    std::string word;
    while (std::getline(ss, word, ',')) {
        words.push_back(word);
    }
    for (auto& word : words) {
        if (config[word]["Name"].empty()) {
            continue;
        }
        _pools[config[word]["Name"]] = std::make_unique<ChatConPool>(5, config[word]["Host"], config[word]["Port"]);
    }
}

AddFriendRsp ChatGrpcClient::NotifyAddFriend(std::string server_ip, const AddFriendReq& req)
{
    AddFriendRsp rsp;
    Defer defer([&rsp, &req] {
        rsp.set_error(Error_Codes::Success);
        rsp.set_applyuid(req.applyuid());
        rsp.set_touid(req.touid());
    });

    auto find_iter = _pools.find(server_ip);
    if (find_iter == _pools.end()) {
        return rsp;
    }
    auto& pool = find_iter->second;
    ClientContext context;
    auto stub = pool->getConnection();
    Status status = stub->NotifyAddFriend(&context, req, &rsp);
    Defer defercon([&stub, this, &pool] {
        pool->returnConnection(std::move(stub));
    });
    if (!status.ok()) {
        rsp.set_error(Error_Codes::RPCFailed);
        return rsp;
    }
    return rsp;
}

AuthFriendRsp ChatGrpcClient::NotifyAuthFriend(std::string server_ip, AuthFriendReq& req)
{
    AuthFriendRsp rsp;
    Defer defer([&rsp, &req] {
        rsp.set_error(Error_Codes::Success);
        rsp.set_fromuid(req.fromuid());
        rsp.set_touid(req.touid());
    });
    auto find_iter = _pools.find(server_ip);
    if (find_iter == _pools.end()) {
        return rsp;
    }
    auto&pool = find_iter->second;
    ClientContext context;
    auto stub = pool->getConnection();
    Status status = stub->NotifyAuthFriend(&context, req, &rsp);
    Defer defercon([&stub, this,&pool]() {
        pool->returnConnection(std::move(stub));
    });
    if (!status.ok()) {
        rsp.set_error(Error_Codes::RPCFailed);
        return rsp;
    }
    return rsp;
}

TextChatMsgRsp ChatGrpcClient::NotifyTextChatMsg(std::string server_ip, const TextChatMsgReq& req, const Json::Value& rtvalue)
{
    TextChatMsgRsp rsp;
    rsp.set_error(Error_Codes::Success);
    Defer defer([&rsp, &req]() {
        rsp.set_fromuid(req.fromuid());
        rsp.set_touid(req.touid());
        for (const auto& text_data : req.textmsgs()) {
            TextChatData* new_data = rsp.add_textmsgs();
            new_data->set_msgid(text_data.msgid());
            new_data->set_msgcontent(text_data.msgcontent());
        }
    });
    auto find_iter = _pools.find(server_ip);
    if (find_iter == _pools.end()) {
        return rsp;
    }
    auto&pool = find_iter->second;
    ClientContext context;
    auto stub = pool->getConnection();
    Status status = stub->NotifyTextChatMsg(&context, req, &rsp);
    Defer defercon([&stub, &pool, this]() {
        pool->returnConnection(std::move(stub));
    });
    if (!status.ok()) {
        rsp.set_error(Error_Codes::RPCFailed);
        return rsp;
    }
    return rsp;
}

KickUserRsp ChatGrpcClient::NotifyKickUser(std::string server_ip, const KickUserReq req)
{
    KickUserRsp rsp;
    Defer defer([&rsp,&req]() {
        rsp.set_uid(req.uid());
        rsp.set_error(Error_Codes::Success);
        });
    auto find_iter = _pools.find(server_ip);
    if (find_iter == _pools.end()) {
        return rsp;
    }
    auto& pool = find_iter->second;

    auto stub = pool->getConnection();
    Defer defer2([&pool, &stub]() {
        pool->returnConnection(std::move(stub));
        });
    ClientContext context;
    Status status = stub->NotifyKickUser(&context, req, &rsp);
    if (!status.ok()) {
        rsp.set_error(Error_Codes::RPCFailed);
        return rsp;
    }
    return rsp;

}


