QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
DESTDIR = ./bin

CONFIG += c++17
LIBS += -luser32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adduseritem.cpp \
    applyfriend.cpp \
    applyfrienditem.cpp \
    applyfriendlist.cpp \
    applyfriendpage.cpp \
    authenfriend.cpp \
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
    contactuserlist.cpp \
    conuseritem.cpp \
    customizeedit.cpp \
    findfaildlg.cpp \
    findsuccessdlg.cpp \
    friendlabel.cpp \
    global.cpp \
    grouptipitem.cpp \
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
    applyfrienditem.h \
    applyfriendlist.h \
    applyfriendpage.h \
    authenfriend.h \
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
    contactuserlist.h \
    conuseritem.h \
    customizeedit.h \
    findfaildlg.h \
    findsuccessdlg.h \
    friendlabel.h \
    global.h \
    grouptipitem.h \
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
    applyfrienditem.ui \
    applyfriendpage.ui \
    authenfriend.ui \
    chatdialog.ui \
    chatpage.ui \
    chatuserwid.ui \
    conuseritem.ui \
    findfaildlg.ui \
    findsuccessdlg.ui \
    friendlabel.ui \
    grouptipitem.ui \
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

msvc {
    # 字符编码
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8

    # 解决PDB文件冲突
    QMAKE_CXXFLAGS += /FS

    # 关键：启用__cplusplus宏的正确定义（Qt 6必需）
    QMAKE_CXXFLAGS += /Zc:__cplusplus

    # 使用正确的运行时库（与Qt匹配）
    QMAKE_CXXFLAGS_DEBUG -= /MTd
    QMAKE_CXXFLAGS_DEBUG += /MDd
    QMAKE_CXXFLAGS_RELEASE -= /MT
    QMAKE_CXXFLAGS_RELEASE += /MD

    # C++标准
    QMAKE_CXXFLAGS += /std:c++17

    # 手动添加MSVC标准库路径（已配置，保留）
    MSVC_INCLUDE = "E:\Microsoft Visual Studio\2022\Community_IDE\VC\Tools\MSVC\14.29.30133\include"
    WINSDK_INCLUDE = "C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt"
    INCLUDEPATH += $$MSVC_INCLUDE $$WINSDK_INCLUDE
}

