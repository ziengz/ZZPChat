
#ifndef USERMGR_H
#define USERMGR_H
#include <memory>
#include "singleton.h"
#include "userdata.h"


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

private:
    UserMgr();
    QString _name;
    QString _token;
    int _uid;
    std::shared_ptr<UserInfo>_user_info;
};

#endif // USERMGR_H
