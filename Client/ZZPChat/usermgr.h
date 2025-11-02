
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
    void SetName(QString name);
    void SetUid(int uid);
    void SetToken(QString token);
    QString GetName();
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();

private:
    UserMgr();
    QString _name;
    QString _token;
    int _uid;
    std::shared_ptr<UserInfo>_user_info;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
};

#endif // USERMGR_H
