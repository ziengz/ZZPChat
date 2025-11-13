
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

std::vector<std::shared_ptr<ApplyInfo>> UserMgr::GetApplyList()
{
    return _apply_list;
}

bool UserMgr::CheckFriendById(int uid)
{
    auto iter = _friend_map.find(uid);
    if(iter==_friend_map.end()){
        return false;
    }
    return true;
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

void UserMgr::AddFriend(std::shared_ptr<AuthRsp> auth_rsp)
{
    auto friend_info = std::make_shared<FriendInfo>(auth_rsp);
    _friend_map[auth_rsp->_uid] = friend_info;
}

void UserMgr::AddFriend(std::shared_ptr<AuthInfo> auth_info)
{
    auto friend_info = std::make_shared<FriendInfo>(auth_info);
    _friend_map[auth_info->_uid] = friend_info;
}
UserMgr::UserMgr()
{
}
