
#include "mainwindow.h"
#include "global.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug()<<"open success";
        QString style = (QString)qss.readAll();
        a.setStyleSheet(style);
        qss.close();
    }
    else{
        qDebug()<<"打开失败";
    }

//  1、获取应用程序的当前路径
    QString path = QCoreApplication::applicationDirPath();
    qDebug()<<path;
    //  2、拼接ini配置文件的完整路径
    QString filename = "config.ini";
    //QDir::separator() 返回平台特定的路径分隔符（Windows 上是 \，Linux/macOS 上是 /）
    //QDir::toNativeSeparators() 将路径中的分隔符转换为当前平台的本地格式
    //结果示例（Windows）：C:\Program Files\MyApp\config.ini
    QString config_path = QDir::toNativeSeparators(path+QDir::separator()+filename);
    qDebug()<<config_path;
//  3、使用QSettings读取INI文件
    //第二个参数指定使用ini文件格式
    QSettings settings(config_path,QSettings::IniFormat);
//  4、从配置文件中读取特定键的值
    QString gata_host = settings.value("GateServer/host").toString();
    QString gata_port = settings.value("GateServer/port").toString();
//  5、构造URL字符串
    gate_url_prefix = "http://"+gata_host+":"+gata_port;

    qDebug()<<gata_host<<" "<<gata_port;
    qDebug()<<gate_url_prefix;
    MainWindow w;
    w.show();
    return a.exec();
}
