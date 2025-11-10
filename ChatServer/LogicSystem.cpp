#include "LogicSystem.h"
#include "StatusGrpcClient.h"
#include "MySqlMgr.h"
#include "CServer.h"
#include "UserMgr.h"
#include "RedisMgr.h"
#include "ChatGrpcClient.h"


LogicSystem::LogicSystem():_b_stop(false)
{
	RegisterCallBack();
	_work_thread = std::thread(&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem()
{
	std::lock_guard<std::mutex>lock(mutex_);
	_b_stop = true;
	cond_.notify_all();
}

void LogicSystem::RegisterCallBack()
{
	_fun_callbacks[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	_fun_callbacks[ID_SEARCH_USER_REQ] = std::bind(&LogicSystem::SearchInfo, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	_fun_callbacks[ID_ADD_FRIEND_REQ] = std::bind(&LogicSystem::AddFriendApply, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> msg)
{
	std::unique_lock<std::mutex>lock(mutex_);
	_msg_que.push(msg);
	if (_msg_que.size() == 1) {
		lock.unlock();
		cond_.notify_one();
	}
}

void LogicSystem::DealMsg()
{
	while (true) {
		std::unique_lock<std::mutex>lock(mutex_);
		//如果队列为空且服务器没停止则等待激活
		while (_msg_que.empty() && !_b_stop) {
			cond_.wait(lock);
		}
		//如果停止，则检查队列中是否还有数据
		if (_b_stop) {
			while (!_msg_que.empty()) {
				auto msgnode = _msg_que.front();
				std::cout << "recv_msg id is " << msgnode->_recvnode->msg_id_ << std::endl;
				auto call_back_iter = _fun_callbacks.find(msgnode->_recvnode->msg_id_);
				if (call_back_iter == _fun_callbacks.end()) {
					_msg_que.pop();
					continue;
				}
				call_back_iter->second(msgnode->_session, msgnode->_recvnode->msg_id_,
					std::string(msgnode->_recvnode->data_,msgnode->_recvnode->cur_len_));
				_msg_que.pop();
			}
			break;
		}
		//如果没有停服，则继续读取
		auto msgnode = _msg_que.front();
		std::cout << "recv msg id is " << msgnode->_recvnode->msg_id_ << std::endl;
		auto call_back_iter = _fun_callbacks.find(msgnode->_recvnode->msg_id_);
		if (call_back_iter == _fun_callbacks.end()) {
			_msg_que.pop();
			std::cout << "msg id [" << msgnode->_recvnode->msg_id_ << "] handler not found" << std::endl;
			continue;
		}
		call_back_iter->second(msgnode->_session, msgnode->_recvnode->msg_id_,
			std::string(msgnode->_recvnode->data_, msgnode->_recvnode->cur_len_));
		_msg_que.pop();
	}
}

bool LogicSystem::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo)
{
	std::string info_str = "";
	bool get_success = RedisMgr::GetIntance()->Get(base_key, info_str);
	if (get_success) {
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str,root);
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
		//如果redis中不存在，就在mysql中查找，并将结果写入redis中
		std::shared_ptr<UserInfo> user_info = MySqlMgr::GetIntance()->GetUser(uid);
		if (user_info == nullptr) {
			return false;
		}
		userinfo = user_info;
		
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

void LogicSystem::LoginHandler(std::shared_ptr<Session> session, const short& msg_id, const std::string &msg_data)
{
	Json::Reader reader;
	Json::Value src_root;
	reader.parse(msg_data, src_root);
	auto uid = src_root["uid"].asInt();
	auto token = src_root["token"].asString();
	std::cout << "user login uid is " << uid << " user token is "
		<< token << std::endl;

	Json::Value returnValue;
	//该函数调用完之后会自动调用以下
	Defer defer([this,&returnValue,session]() {
		std::string return_str = returnValue.toStyledString();
		session->Send(return_str, MSG_CHAT_LOGIN_RSP);
		});
	
	//从redis中读取数据判断token是否正确
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";
	bool b_success = RedisMgr::GetIntance()->Get(token_key, token_value);
	if (!b_success)
	{
		returnValue["error"] = Error_Codes::UidInvalid;
		return;
	}
	if (token_value != token) {
		returnValue["error"] = Error_Codes::TokenInvalid;
		return;
	}
	std::string base_key = USER_BASE_INFO + uid_str;
	auto user_info = std::make_shared<UserInfo>();
	bool b_base = GetBaseInfo(base_key, uid, user_info);
	if (!b_base) {
		returnValue["error"] = Error_Codes::UidInvalid;
		return;
	}
	returnValue["error"] = Error_Codes::Success;

	returnValue["uid"] = uid;
	returnValue["pwd"] = user_info->pwd;
	returnValue["name"] = user_info->name;
	returnValue["email"] = user_info->email;
	returnValue["nick"] = user_info->nick;
	returnValue["desc"] = user_info->desc;
	returnValue["sex"] = user_info->sex;
	returnValue["icon"] = user_info->icon;

	//从数据库中获取申请列表

	//获取好友列表

	auto server_name = ConfigMgr::Instance().GetValue("SelfServer", "Name");
	//将登陆数量增加
	auto rd_res = RedisMgr::GetIntance()->HGet(LOGIN_COUNT, server_name);
	int count = 0;
	if (!rd_res.empty()) {
		count = std::stoi(rd_res);
	}
	count++;
	std::string count_str = std::to_string(count);
	//更新完之后写入redis
	RedisMgr::GetIntance()->HSet(LOGIN_COUNT, server_name, count_str);

	//session绑定用户uid
	session->SetUserId(uid);
	//为用户设置登录IP server名字
	std::string ipkey = USERIPPREFIX + uid_str;
	RedisMgr::GetIntance()->Set(ipkey, server_name);
	
	//uid和session绑定，方便后续踢人操作
	UserMgr::GetIntance()->SetUserSession(uid, session);
	return;
}

void LogicSystem::SearchInfo(std::shared_ptr<Session> session, const short& msg_id, const std::string& msg_data)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::string uid_str = root["uid"].asString();
	std::cout << "uid is " << uid_str << std::endl;

	Json::Value rtvalue;	
	Defer defer([&rtvalue,&session]() {
		std::string return_str = rtvalue.toStyledString();
		session->Send(return_str, ID_SEARCH_USER_RSP);
	});
	//两种查询
	bool b_digit = isPureDigit(uid_str);
	if (b_digit) {
		getUserByUid(uid_str,rtvalue);
	}
	else {
		getUserByName(uid_str,rtvalue);
	}
	
}

void LogicSystem::AddFriendApply(std::shared_ptr<Session> session, const short& msg_id, const std::string& msg_data)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	auto uid = root["uid"].asInt();
	auto applyname = root["applyname"].asString();
	auto bakname = root["bakname"].asString();
	auto touid = root["touid"].asInt();
	std::cout<<"user login uid is "<<uid<<" applyname is "<<applyname
		<< " bakname is " << bakname << " touid is " << touid << std::endl;
	
	MySqlMgr::GetIntance()->AddFriendApply(uid, touid);

	auto to_str = std::to_string(touid);
	auto to_ip_key = USERIPPREFIX + to_str;
	std::string to_ip_value = "";
	bool b_ip = RedisMgr::GetIntance()->Get(to_ip_key, to_ip_value);
	
	if (!b_ip) {
		return;
	}
	auto& cfg = ConfigMgr::Instance();
	auto self_name = cfg["SelfServer"]["Name"];
	std::cout << "host ip is " << self_name <<" name is "<<applyname
		<< " peer ip is " << to_ip_value << " name is "<<bakname << std::endl;
	//如果同一服务器，直接发送申请消息
	if (to_ip_value == self_name) {
		auto session = UserMgr::GetIntance()->getSession(touid);
		if (session) {
			Json::Value notify;
			notify["error"] = Error_Codes::Success;
			notify["applyuid"] = uid;
			notify["name"] = applyname;
			notify["desc"] = "";
			std::string return_str = notify.toStyledString();
			session->Send(return_str, ID_NOTIFY_ADD_FRIEND_REQ);
		}
		else {
			std::cout << "session is nullptr" << std::endl;
		}
		return;
	}

	std::string base_key = USER_BASE_INFO + std::to_string(uid);
	auto apply_info = std::make_shared<UserInfo>();
	bool b_info = GetBaseInfo(base_key, uid, apply_info);
	
	AddFriendReq add_req;
	add_req.set_applyuid(uid);
	add_req.set_desc("");
	add_req.set_touid(touid);
	add_req.set_name(applyname);
	if (b_info) {
		add_req.set_icon(apply_info->icon);
		add_req.set_sex(apply_info->sex);
		add_req.set_nick(apply_info->nick);
	}

	ChatGrpcClient::GetIntance()->NotifyAddFriend(to_ip_value, add_req);
}

bool LogicSystem::isPureDigit(std::string uid_str)
{
	for (char c : uid_str) {
		if (!std::isdigit(c)) {
			return false;
			break;
		}
	}
	return true;
}

void LogicSystem::getUserByUid(std::string uid_str, Json::Value& rtvalue)
{
	rtvalue["error"] = Error_Codes::Success;
	std::string base_key = USERIPPREFIX + uid_str;
	std::string info_str = "";
	bool b_success = RedisMgr::GetIntance()->Get(base_key, info_str);
	if (b_success) {
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str, root);
		auto uid = root["uid"].asInt();
		auto Name = root["Name"].asString();
		auto name = root["name"].asString();
		auto pwd = root["pwd"].asString();
		auto email = root["email"].asString();
		auto nick = root["nick"].asString();
		auto desc = root["desc"].asString();
		auto sex = root["sex"].asInt();
		auto icon = root["icon"].asString();
		std::cout << "user  uid is  " << uid << " name  is "
			<< name << " pwd is " << pwd << " email is " << email << " icon is " << icon << std::endl;

		rtvalue["uid"] = uid;
		rtvalue["pwd"] = pwd;
		rtvalue["name"] = name;
		rtvalue["email"] = email;
		rtvalue["nick"] = nick;
		rtvalue["desc"] = desc;
		rtvalue["sex"] = sex;
		rtvalue["icon"] = icon;
		return;
	}
	else {
		int uid = std::stoi(uid_str);
		std::shared_ptr<UserInfo> user_info = MySqlMgr::GetIntance()->GetUser(uid);
		if (user_info == nullptr) {
			rtvalue["error"] = Error_Codes::UidInvalid;
			return;
		}

		//将数据库内容写入redis缓存
		Json::Value redis_root;
		redis_root["uid"] = user_info->uid;
		redis_root["pwd"] = user_info->pwd;
		redis_root["name"] = user_info->name;
		redis_root["email"] = user_info->email;
		redis_root["nick"] = user_info->nick;
		redis_root["desc"] = user_info->desc;
		redis_root["sex"] = user_info->sex;
		redis_root["icon"] = user_info->icon;
		std::string redis_str = redis_root.toStyledString();
		RedisMgr::GetIntance()->Set(base_key, redis_str);

		rtvalue["uid"] = user_info->uid;
		rtvalue["pwd"] = user_info->pwd;
		rtvalue["name"] = user_info->name;
		rtvalue["email"] = user_info->email;
		rtvalue["nick"] = user_info->nick;
		rtvalue["desc"] = user_info->desc;
		rtvalue["sex"] = user_info->sex;
		rtvalue["icon"] = user_info->icon;
	}
}

void LogicSystem::getUserByName(std::string name, Json::Value& rtvalue)
{
	rtvalue["error"] = Error_Codes::Success;
	std::string base_key = NAME_INFO + name;
	std::string info_str = "";
	bool b_success = RedisMgr::GetIntance()->Get(base_key, info_str);
	if (b_success) {
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str, root);
		auto uid = root["uid"].asInt();
		auto Name = root["Name"].asString();
		auto name = root["name"].asString();
		auto pwd = root["pwd"].asString();
		auto email = root["email"].asString();
		auto nick = root["nick"].asString();
		auto desc = root["desc"].asString();
		auto sex = root["sex"].asInt();
		auto icon = root["icon"].asString();
		std::cout << "user  uid is  " << uid << " name  is "
			<< name << " pwd is " << pwd << " email is " << email << " icon is " << icon << std::endl;

		rtvalue["uid"] = uid;
		rtvalue["pwd"] = pwd;
		rtvalue["name"] = name;
		rtvalue["email"] = email;
		rtvalue["nick"] = nick;
		rtvalue["desc"] = desc;
		rtvalue["sex"] = sex;
		rtvalue["icon"] = icon;
		return;
	}
	else {
		std::shared_ptr<UserInfo> user_info = MySqlMgr::GetIntance()->GetUser(name);
		if (user_info == nullptr) {
			rtvalue["error"] = Error_Codes::UidInvalid;
			return;
		}

		//将数据库内容写入redis缓存
		Json::Value redis_root;
		redis_root["uid"] = user_info->uid;
		redis_root["pwd"] = user_info->pwd;
		redis_root["name"] = user_info->name;
		redis_root["email"] = user_info->email;
		redis_root["nick"] = user_info->nick;
		redis_root["desc"] = user_info->desc;
		redis_root["sex"] = user_info->sex;
		redis_root["icon"] = user_info->icon;
		std::string redis_str = redis_root.toStyledString();
		RedisMgr::GetIntance()->Set(base_key, redis_str);

		rtvalue["uid"] = user_info->uid;
		rtvalue["pwd"] = user_info->pwd;
		rtvalue["name"] = user_info->name;
		rtvalue["email"] = user_info->email;
		rtvalue["nick"] = user_info->nick;
		rtvalue["desc"] = user_info->desc;
		rtvalue["sex"] = user_info->sex;
		rtvalue["icon"] = user_info->icon;
	}
}
