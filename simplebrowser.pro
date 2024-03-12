TEMPLATE = app
TARGET = kv
QT += webenginewidgets \
      pdf \
      pdfwidgets \
      gui \
      widgets \
      qml \
      quick \
      webenginequick

#all defines are moved to KVVersion.h

CONFIG += qmltypes
QML_IMPORT_NAME = BrowserUtils
QML_IMPORT_MAJOR_VERSION = 1

TRANSLATIONS += lang_fr.ts

HEADERS += \
    ../KVMaintenanceTool/kvmtmainwindow.h \
    ../KVMaintenanceTool/updater.h \
    KVVersion.h \
    aboutwindow.h \
    appsettings.h \
    browser.h \
    browserwindow.h \
    downloaddlg.h \
    downloadmanagerwidget.h \
    downloadwidget.h \
    jsobjectinjector.h \
    kvwebuiextension.h \
    kvwindow.h \
    logs.h \
    mainwindow.h \
    runguard.h \
    settingscontent.h \
    settingsserver.h \
    tabwidget.h \
    urlschemehandler.h \
    utils.h \
    webengineview.h \
    webpage.h \
    webpageexporter.h \
    webpopupwindow.h \
    webview.h \
    requestinterceptor.h \
    settingsjson.h

SOURCES += \
    ../KVMaintenanceTool/kvmtmainwindow.cpp \
    ../KVMaintenanceTool/updater.cpp \
    aboutwindow.cpp \
    appsettings.cpp \
    browser.cpp \
    browserwindow.cpp \
    downloaddlg.cpp \
    downloadmanagerwidget.cpp \
    downloadwidget.cpp \
    jsobjectinjector.cpp \
    kvwebuiextension.cpp \
    kvwindow.cpp \
    logs.cpp \
    main.cpp \
    mainwindow.cpp \
    runguard.cpp \
    settingscontent.cpp \
    settingsserver.cpp \
    tabwidget.cpp \
    urlschemehandler.cpp \
    webengineview.cpp \
    webpage.cpp \
    webpageexporter.cpp \
    webpopupwindow.cpp \
    webview.cpp \
    requestinterceptor.cpp \
    settingsjson.cpp

win32 {
    CONFIG -= embed_manifest_exe
    QMAKE_MANIFEST = $$PWD/simplebrowser.exe.manifest
}



FORMS += \
    certificateerrordialog.ui \
    passworddialog.ui \
    downloadmanagerwidget.ui \
    downloadwidget.ui \
    settingscontent.ui \
    settingsserver.ui

RESOURCES += data/simplebrowser.qrc \
    languages.qrc \
    qml.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/simplebrowser
INSTALLS += target

DISTFILES += \
    ApplicationRoot.qml \
    BrowserDialog.qml \
    BrowserWindow.qml \
    DownloadView.qml \
    FindBar.qml \
    FullScreenNotification.qml \
    WebAuthDialog.qml
