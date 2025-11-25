#include "ChatServiceImpl.h"
#include "UserMgr.h"
#include "Session.h"
#include "MySqlMgr.h"
#include "RedisMgr.h"

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
	auto fromuid = request->fromuid();
	auto touid = request->touid();
	auto session = UserMgr::GetIntance()->getSession(touid);
	Defer defer([request, response]() {
		response->set_error(Error_Codes::Success);
		response->set_fromuid(request->fromuid());
		response->set_touid(request->touid());
		});
	//用户不在内存中直接返回
	if (session == nullptr) {
		return Status::OK;
	}
	//在内存中直接通知对方
	Json::Value rtvalue;
	rtvalue["error"] = Error_Codes::Success;
	rtvalue["fromuid"] = request->fromuid();
	rtvalue["touid"] = request->touid();

	std::string base_key = USER_BASE_INFO + std::to_string(fromuid);
	auto user_info = std::make_shared<UserInfo>();
	bool b_info = GetBaseInfo(base_key, fromuid, user_info);
	if (b_info) {
		rtvalue["name"] = user_info->name;
		rtvalue["nick"] = user_info->nick;
		rtvalue["icon"] = user_info->nick;
		rtvalue["sex"] = user_info->sex;
	}
	else {
		rtvalue["error"] = Error_Codes::UidInvalid;
	}
	std::string return_str = rtvalue.toStyledString();
	session->Send(return_str, ID_NOTIFY_AUTH_FRIEND_REQ);

	return Status::OK;
}

Status ChatServiceImpl::NotifyTextChatMsg(ServerContext* context, const TextChatMsgReq* request, TextChatMsgRsp* response)
{
	auto touid = request->touid();
	auto session = UserMgr::GetIntance()->getSession(touid);
	if (session == nullptr) {
		return Status::OK;
	}
	Json::Value rtvalue;
	rtvalue["error"] = Error_Codes::Success;
	rtvalue["fromuid"] = request->fromuid();
	rtvalue["touid"] = request->touid();

	Json::Value text_array;
	for (auto& msg : request->textmsgs()) {
		Json::Value elem;
		elem["content"] = msg.msgcontent();
		elem["msgid"] = msg.msgid();
		text_array.append(elem);
	}
	rtvalue["text_array"] = text_array;
	std::string return_str = rtvalue.toStyledString();

	session->Send(return_str, ID_NOTIFY_TEXT_CHAT_MSG_REQ);
	return Status::OK;
}

Status ChatServiceImpl::NotifyKickUser(ServerContext* context, const KickUserReq* request, KickUserRsp* response) {
	auto uid = request->uid();
	auto session = UserMgr::GetIntance()->getSession(uid);
	
	//如果不在内存中，直接返回
	if (session == nullptr) {
		return Status::OK;
	}
	Defer defer([&response,&request]() {
		response->set_error(Error_Codes::Success);
		response->set_uid(request->uid());
	});
	session->NotifyOffline(uid);
	_p_server->ClearSession(session->GetSessionid());
	return Status::OK;
}

bool ChatServiceImpl::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo)
{
	//优先查redis中查询用户信息
	std::string info_str = "";
	bool b_base = RedisMgr::GetIntance()->Get(base_key, info_str);
	if (b_base) {
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str, root);
		userinfo->uid = root["uid"].asInt();
		userinfo->name = root["name"].asString();
		userinfo->pwd = root["pwd"].asString();
		userinfo->email = root["email"].asString();
		userinfo->nick = root["nick"].asString();
		userinfo->desc = root["desc"].asString();
		userinfo->sex = root["sex"].asInt();
		userinfo->icon = root["icon"].asString();
		std::cout << "user login uid is  " << userinfo->uid << " name  is "
			<< userinfo->name << " pwd is " << userinfo->pwd << " email is " << userinfo->email << std::endl;
	}
	else {
		//redis中没有则查询mysql
		//查询数据库
		std::shared_ptr<UserInfo> user_info = nullptr;
		user_info = MySqlMgr::GetIntance()->GetUser(uid);
		if (user_info == nullptr) {
			return false;
		}

		userinfo = user_info;

		//将数据库内容写入redis缓存
		Json::Value redis_root;
		redis_root["uid"] = uid;
		redis_root["pwd"] = userinfo->pwd;
		redis_root["name"] = userinfo->name;
		redis_root["email"] = userinfo->email;
		redis_root["nick"] = userinfo->nick;
		redis_root["desc"] = userinfo->desc;
		redis_root["sex"] = userinfo->sex;
		redis_root["icon"] = userinfo->icon;
		RedisMgr::GetIntance()->Set(base_key, redis_root.toStyledString());
	}

	return true;
}

void ChatServiceImpl::RegisterServer(std::shared_ptr<CServer> pserver)
{
	_p_server = pserver;
}
