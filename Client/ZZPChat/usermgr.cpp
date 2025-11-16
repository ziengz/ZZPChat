
#include "usermgr.h"
#include <QJsonValue>
#include <QJsonArray>

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

void UserMgr::setUserInfo(std::shared_ptr<UserInfo> user_info)
{
    _user_info = user_info;
}

QString UserMgr::GetName()
{
    return _user_info->_name;
}

std::shared_ptr<UserInfo> UserMgr::GetUserInfo()
{
    return _user_info;
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

std::shared_ptr<FriendInfo> UserMgr::GetFriendById(int uid)
{
    auto iter = _friend_map.find(uid);
    if(iter==_friend_map.end()){
        return nullptr;
    }
    return *iter;
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

void UserMgr::AppendApplyList(QJsonArray array)
{
    //遍历QJsonArray
    for(const QJsonValue &value : array){
        auto name = value["name"].toString();
        auto desc = value["desc"].toString();
        auto icon = value["icon"].toString();
        auto nick = value["nick"].toString();
        auto sex = value["sex"].toInt();
        auto uid = value["uid"].toInt();
        auto status = value["status"].toInt();
        auto info = std::make_shared<ApplyInfo>(uid, name,
                           desc, icon, nick, sex, status);
        _apply_list.push_back(info);
    }
}

void UserMgr::AppendFriendList(QJsonArray array)
{
    for(const QJsonValue&value:array){
        auto name = value["name"].toString();
        auto desc = value["desc"].toString();
        auto icon = value["icon"].toString();
        auto nick = value["nick"].toString();
        auto sex = value["sex"].toInt();
        auto uid = value["uid"].toInt();
        auto back = value["back"].toString();

        auto info = std::make_shared<FriendInfo>(uid,name,nick,icon,sex,desc,back);
        _friend_list.push_back(info);
        _friend_map.insert(uid,info);
    }
}

void UserMgr::AppendFriendChatMsg(int friend_id, std::vector<std::shared_ptr<TextChatData> > msgs)
{
    auto find_iter = _friend_map.find(friend_id);
    if(find_iter == _friend_map.end()){
        qDebug()<<"append friend id is "<<friend_id<<" not found";
        return;
    }
    find_iter.value()->AppendChatMsgs(msgs);
}

void UserMgr::UpdateChatLoadedCount()
{
    int begin = _chat_loaded;
    int end = begin+CHAT_COUNT_PRE_PAGE;
    if(begin>=_friend_list.size()){
        return;
    }
    if(end>_friend_list.size()){
        _chat_loaded = _friend_list.size();
        return;
    }
    _chat_loaded = end;
}

std::vector<std::shared_ptr<FriendInfo> > UserMgr::GetChatListPrePage()
{
    std::vector<std::shared_ptr<FriendInfo>>friend_list;
    int begin = _chat_loaded;
    int end = begin+CHAT_COUNT_PRE_PAGE;
    if(begin>=_friend_list.size()){
        return friend_list;
    }
    if(end >_friend_list.size()){
        friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin , _friend_list.end());
        return friend_list;
    }
    friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin()+begin, _friend_list.end());
    return friend_list;
}

std::vector<std::shared_ptr<FriendInfo> > UserMgr::GetConListPrePage()
{
    std::vector<std::shared_ptr<FriendInfo>> friend_list;
        int begin = _contact_loaded;
        int end = begin + CHAT_COUNT_PRE_PAGE;

        if (begin >= _friend_list.size()) {
            return friend_list;
        }

        if (end > _friend_list.size()) {
            friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin, _friend_list.end());
            return friend_list;
        }


        friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin, _friend_list.begin() + end);
        return friend_list;
}

UserMgr::UserMgr():_user_info(nullptr),_chat_loaded(0),_contact_loaded(0)
{

}
