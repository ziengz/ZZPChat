#include "loadingdlg.h"
#include "ui_loadingdlg.h"
#include <QMovie>

LoadingDlg::LoadingDlg(QWidget *parent,QString tip) :
    QDialog(parent),
    ui(new Ui::LoadingDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowSystemMenuHint|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);  //设置背景透明

    if(parent){
        //获取屏幕尺寸
        setFixedSize(parent->size());
    }
    if (parent) {  // 检查是否存在父窗口
        QPoint topLeft = parent->mapToGlobal(QPoint(0, 0));  // 获取父窗口左上角的全局坐标
        move(topLeft);  // 将当前窗口移动到该位置
    }

    QMovie*movie = new QMovie(":/res/loading.gif");
    ui->loading_lb->setMovie(movie);
    movie->start();
    // 3. 告诉 QMovie：将解码后的每一帧缩放到 label大小
    movie->setScaledSize(ui->loading_lb->size());
    ui->status_lb->setText(tip);
}

LoadingDlg::~LoadingDlg()
{
    delete ui;
}
