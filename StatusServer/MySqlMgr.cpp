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

MySqlMgr::MySqlMgr()
{

}
