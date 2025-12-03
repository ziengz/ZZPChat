
#ifndef USERMGR_H
#define USERMGR_H
#include <memory>
#include "singleton.h"
#include "userdata.h"
#include <vector>


class UserMgr:public QObject,public singleton<UserMgr>,
        public std::enable_shared_from_this<UserMgr>
{
    Q_OBJECT
public:
    friend class singleton<UserMgr>;
    ~UserMgr();

    void SetToken(QString token);
    void setUserInfo(std::shared_ptr<UserInfo> user_info);
    int GetUid();
    QString GetName();
    QString GetNick();
    QString GetDesc();
    QString GetIcon();
    std::shared_ptr<UserInfo>GetUserInfo();
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
    bool CheckFriendById(int uid);
    std::shared_ptr<FriendInfo>GetFriendById(int uid);
    void AddApplyList(std::shared_ptr<ApplyInfo>apply_list);
    bool AlreadyApply(int uid);
    void AddFriend(std::shared_ptr<AuthRsp> auth_rsp);
    void AddFriend(std::shared_ptr<AuthInfo> auth_info);
    //后端从数据库中读取申请列表和好友列表追加到列表中
    void AppendApplyList(QJsonArray array);
    void AppendFriendList(QJsonArray array);
    //追加好友来的消息
    void AppendFriendChatMsg(int friend_id,std::vector<std::shared_ptr<TextChatData>>msgs);
    //更新已加载用户数量
    void UpdateChatLoadedCount();
    //加载从数据库中得到的好友列表
    std::vector<std::shared_ptr<FriendInfo>>GetChatListPrePage();
    std::vector<std::shared_ptr<FriendInfo>>GetConListPrePage();



private:
    UserMgr();
    QString _name;
    QString _token;
    int _uid;
    int _chat_loaded;
    int _contact_loaded;
    std::shared_ptr<UserInfo>_user_info;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
    std::vector<std::shared_ptr<FriendInfo>>_friend_list;
    QMap<int,std::shared_ptr<FriendInfo>>_friend_map;
};

#endif // USERMGR_H
