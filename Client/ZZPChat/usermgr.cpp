
#include "usermgr.h"


UserMgr::~UserMgr()
{
}
void UserMgr::SetName(QString name)
{
    _name = name;
}
void UserMgr::SetUid(int uid)
{
    _uid = uid;
}
void UserMgr::SetToken(QString token)
{
    _token = token;
}

QString UserMgr::GetName()
{
    return _user_info->_name;
}
UserMgr::UserMgr()
{
}
