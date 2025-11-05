#pragma once
#include <memory>
#include <grpc++/grpc++.h>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "data.h"

using grpc::Channel;
using grpc::Status;
using grpc::ServerBuilder;
using grpc::ServerContext;

using message::AddFriendReq;
using message::AddFriendRsp;
using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::ChatService;

using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;

//对于chatServer2类不仅是客户端还是服务器
class ChatServiceImpl final:public ChatService::Service
{
public:
	ChatServiceImpl();
	Status NotifyAddFriend(ServerContext* context, const AddFriendReq* request, AddFriendRsp* reply) override;
	Status NotifyAuthFriend(ServerContext* context, const AuthFriendReq* request, AuthFriendRsp* response) override;
	Status NotifyTextChatMsg(ServerContext* context, const TextChatMsgReq* request, TextChatMsgRsp* response) override;
	bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);


};

