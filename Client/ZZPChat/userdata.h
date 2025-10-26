
#ifndef USERDATA_H
#define USERDATA_H

#include <QString>
#include <memory>
#include <vector>
#include <QJsonArray>
#include <QJsonObject>

class SearchInfo{
public:
    SearchInfo(int uid,QString name,QString nick,QString desc,int sex);
    int _uid;
    QString _name;
    QString _nick;
    QString _desc;
    int _sex;
    QString _icon;
};



#endif // USERDATA_H
