#include "userinfopage.h"
#include "ui_userinfopage.h"
#include "usermgr.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include "ImageCropperDialog.h"

UserInfoPage::UserInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoPage)
{
    ui->setupUi(this);
    auto icon = UserMgr::getInstance()->GetIcon();
    qDebug()<<"icon is "<<icon;
    QPixmap pix(icon);
    QPixmap scaledPixmap = pix.scaled(ui->head_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->head_lb->setPixmap(scaledPixmap); //将缩放的图片设置到label中
    ui->head_lb->setScaledContents(true); //设置QLabel自动缩放图片内容以适应大小

    auto nick = UserMgr::getInstance()->GetNick();
    auto desc = UserMgr::getInstance()->GetDesc();
    auto name = UserMgr::getInstance()->GetName();
    ui->nick_ed->setText(nick);
    ui->name_ed->setText(name);
    ui->desc_ed->setText(desc);
}

UserInfoPage::~UserInfoPage()
{
    delete ui;
}

void UserInfoPage::on_up_btn_clicked()
{
    // 1. 让对话框也能选 *.webp
    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("选择图片"),
        QString(),
        tr("图片文件 (*.png *.jpg *.jpeg *.bmp *.webp)")
    );
    //使用QPixmap::load加载图片
    QPixmap inputImage;
    if(!inputImage.load(filename)){
        QMessageBox::critical(
            this,
            tr("错误"),
            tr("加载图片失败！请确认已部署WebP插件"),
            QMessageBox::Ok
        );
        return;
    }
    QPixmap image = ImageCropperDialog::getCroppedImage(filename, 600, 400, CropperShape::CIRCLE);
    if (image.isNull())
        return;

    QPixmap scaledPixmap = image.scaled(ui->head_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->head_lb->setPixmap(scaledPixmap); //将缩放的图片设置到label中
    ui->head_lb->setScaledContents(true); //设置QLabel自动缩放图片内容以适应大小

    QString storageDir = QStandardPaths::writableLocation(
                    QStandardPaths::AppDataLocation);

    // 调试输出，查看实际路径
    qDebug() << "存储目录路径:" << storageDir;

    // 确保主存储目录存在
    QDir appDataDir(storageDir);
    if (!appDataDir.exists()) {
        if (!appDataDir.mkpath(".")) {
            qWarning() << "无法创建应用数据目录：" << storageDir;
            QMessageBox::warning(
                this,
                tr("错误"),
                tr("无法创建应用数据目录，请检查权限或磁盘空间。")
            );
            return;
        }
    }

    //在其下面建一个avatars的目录
    QDir dir(storageDir);
    if(!dir.exists("avatars")){
        if(!dir.mkdir("avatars")){
            qDebug()<<"无法创建avatars目录："<<dir.filePath("avatars");
            QMessageBox::warning(
                this,
                tr("错误"),
                tr("无法创建存储目录，请检查权限或磁盘空间。")
            );
            return;
        }
    }
    //拼接最终文件名head.png
    QString filePath = dir.filePath("avatars/head.png");
    //保存scaledPixmap 为png格式
    if(!scaledPixmap.save(filePath,"PNG")){
        QMessageBox::warning(
            this,
            tr("保存失败"),
            tr("头像保存失败，请检查权限或磁盘空间。")
        );
    }else{
        qDebug()<<"头像已保存为："<<filePath;
        // 以后读取直接用同一路径：storageDir/avatars/head.png
    }

}


void UserInfoPage::on_submit_btn_clicked()
{

}

