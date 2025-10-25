#include "loadingdlg.h"
#include "ui_loadingdlg.h"
#include <QMovie>

LoadingDlg::LoadingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowSystemMenuHint|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);  //设置背景透明
    //获取屏幕尺寸
    QMovie*movie = new QMovie(":/res/loading.gif");
    ui->loading_lb->setMovie(movie);
    movie->start();
}

LoadingDlg::~LoadingDlg()
{
    delete ui;
}
