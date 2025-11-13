#include "MySqlMgr.h"
MySqlMgr::~MySqlMgr(){

}

int MySqlMgr::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
	return dao_.RegUser(name, email, pwd);
}

bool MySqlMgr::CheckEmail(const std::string& name, const std::string& email)
{
	return dao_.CheckEmail(name, email);
}

bool MySqlMgr::UpdatePwd(const std::string& name, const std::string& newpwd) {
	return dao_.UpdatePwd(name, newpwd);
}

bool MySqlMgr::CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo)
{
	return dao_.CheckPwd(name, pwd, userInfo);
}

bool MySqlMgr::AddFriendApply(const int& from, const int& to)
{
	return dao_.AddFriendApply(from,to);
}

bool MySqlMgr::AuthFriendApply(const int& from, const int& to) {
	return dao_.AuthFriendApply(from, to);
}

bool MySqlMgr::AddFriend(const int& from, const int& to, std::string back_name)
{
	return dao_.AddFriend(from, to, back_name);
}

std::shared_ptr<UserInfo> MySqlMgr::GetUser(int uid) {
	return dao_.GetUser(uid);
}
std::shared_ptr<UserInfo> MySqlMgr::GetUser(std::string name) {
	return dao_.GetUser(name);
}

MySqlMgr::MySqlMgr()
{

}
