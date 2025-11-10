#include "ChatServiceImpl.h"
#include "UserMgr.h"
#include "Session.h"

ChatServiceImpl::ChatServiceImpl()
{
}

Status ChatServiceImpl::NotifyAddFriend(ServerContext* context, const AddFriendReq* request, AddFriendRsp* reply)
{
	auto touid = request->touid();
	auto session = UserMgr::GetIntance()->getSession(touid);
	Defer defer([&reply,&request]() {
		reply->set_applyuid(reply->applyuid());
		reply->set_error(Error_Codes::Success);
		reply->set_touid(request->touid());
	});
	
	if (session == nullptr) {
		std::cout << "session is nullptr" << std::endl;
		return Status::OK;
	}

	Json::Value rtvalue;
	rtvalue["error"] = Error_Codes::Success;
	rtvalue["applyuid"] = request->applyuid();
	rtvalue["name"] = request->name();
	rtvalue["desc"] = request->desc();
	rtvalue["icon"] = request->icon();
	rtvalue["sex"] = request->sex();
	rtvalue["nick"] = request->nick();

	std::string json_str = rtvalue.toStyledString();
	session->Send(json_str, ID_NOTIFY_ADD_FRIEND_REQ);
	return Status::OK;
}

Status ChatServiceImpl::NotifyAuthFriend(ServerContext* context, const AuthFriendReq* request, AuthFriendRsp* response)
{
	return Status();
}

Status ChatServiceImpl::NotifyTextChatMsg(ServerContext* context, const TextChatMsgReq* request, TextChatMsgRsp* response)
{
	return Status();
}

bool ChatServiceImpl::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo)
{
	return false;
}
