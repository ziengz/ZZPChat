
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
    void setSearchInfo(std::shared_ptr<UserInfo> user_info);
    int GetUid();
    QString GetName();
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();
    bool CheckFriendById(int uid);
    void AddApplyList(std::shared_ptr<ApplyInfo>apply_list);
    bool AlreadyApply(int uid);
    void AddFriend(std::shared_ptr<AuthRsp> auth_rsp);
    void AddFriend(std::shared_ptr<AuthInfo> auth_info);



private:
    UserMgr();
    QString _name;
    QString _token;
    int _uid;
    std::shared_ptr<UserInfo>_user_info;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
    QMap<int,std::shared_ptr<FriendInfo>>_friend_map;
};

#endif // USERMGR_H
