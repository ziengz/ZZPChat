
#include "usermgr.h"


UserMgr::~UserMgr()
{
}

int UserMgr::GetUid()
{
    return _user_info->_uid;
}
void UserMgr::SetToken(QString token)
{
    _token = token;
}

void UserMgr::setSearchInfo(std::shared_ptr<UserInfo> user_info)
{
    _user_info = user_info;
}

QString UserMgr::GetName()
{
    return _user_info->_name;
}

std::vector<std::shared_ptr<ApplyInfo> > UserMgr::GetApplyList()
{
    return _apply_list;
}

bool UserMgr::CheckFriendById(int uid)
{

}

void UserMgr::AddApplyList(std::shared_ptr<ApplyInfo> apply)
{
    _apply_list.push_back(apply);
}

bool UserMgr::AlreadyApply(int uid)
{
    for(auto& list:_apply_list){
        if(uid == list->_uid){
            return true;
        }
    }
    return false;
}
UserMgr::UserMgr()
{
}
