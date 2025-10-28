QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
DESTDIR = ./bin

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adduseritem.cpp \
    applyfriend.cpp \
    bubbleframe.cpp \
    chatdialog.cpp \
    chatitembase.cpp \
    chatpage.cpp \
    chatuserlist.cpp \
    chatuserwid.cpp \
    chatview.cpp \
    clickedbtn.cpp \
    clickedlabel.cpp \
    clickedoncelabel.cpp \
    customizeedit.cpp \
    findsuccessdlg.cpp \
    friendlabel.cpp \
    global.cpp \
    httpmgr.cpp \
    listitembase.cpp \
    loadingdlg.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    messagetextedit.cpp \
    picturebubble.cpp \
    registerdialog.cpp \
    resetdialog.cpp \
    searchlist.cpp \
    statewidget.cpp \
    tcpmgr.cpp \
    textbubble.cpp \
    timerbtn.cpp \
    userdata.cpp \
    usermgr.cpp

HEADERS += \
    adduseritem.h \
    applyfriend.h \
    bubbleframe.h \
    chatdialog.h \
    chatitembase.h \
    chatpage.h \
    chatuserlist.h \
    chatuserwid.h \
    chatview.h \
    clickedbtn.h \
    clickedlabel.h \
    clickedoncelabel.h \
    customizeedit.h \
    findsuccessdlg.h \
    friendlabel.h \
    global.h \
    httpmgr.h \
    listitembase.h \
    loadingdlg.h \
    logindialog.h \
    mainwindow.h \
    messagetextedit.h \
    picturebubble.h \
    registerdialog.h \
    resetdialog.h \
    searchlist.h \
    singleton.h \
    statewidget.h \
    tcpmgr.h \
    textbubble.h \
    timerbtn.h \
    userdata.h \
    usermgr.h

FORMS += \
    adduseritem.ui \
    applyfriend.ui \
    chatdialog.ui \
    chatpage.ui \
    chatuserwid.ui \
    findsuccessdlg.ui \
    friendlabel.ui \
    loadingdlg.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    resetdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    config.ini

CONFIG(debug, debug|release) {
    message("debug mode")

    OutputDir = $${OUT_PWD}/bin
    OutputDir = $$absolute_path($$OutputDir)
    message("OutputDir: $$OutputDir")

    TargetConfig = $$PWD/config.ini
    exists($$TargetConfig) {
        QMAKE_POST_LINK += $$QMAKE_COPY \"$$TargetConfig\" \"$$OutputDir\" $$escape_expand(\n\t)
    }

    # Static 文件夹 - 直接复制内容到 bin 目录
    StaticDir = $$PWD/static
    exists($$StaticDir) {
        # 复制 static 目录内的所有内容到 bin 目录（不创建 static 子文件夹）
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$$StaticDir\" \"$$OutputDir\" $$escape_expand(\n\t)
    }
} else {
    message("release mode")
    OutputDir = $${OUT_PWD}/bin
    OutputDir = $$absolute_path($$OutputDir)

    TargetConfig = $$PWD/config.ini
    exists($$TargetConfig) {
        QMAKE_POST_LINK += $$QMAKE_COPY \"$$TargetConfig\" \"$$OutputDir\" $$escape_expand(\n\t)
    }

    StaticDir = $$PWD/static
    exists($$StaticDir) {
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$$StaticDir\" \"$$OutputDir\" $$escape_expand(\n\t)
    }
}

win32-msvc*:QMAKE_CXXFLAGS += /wd"4819" /utf-8
