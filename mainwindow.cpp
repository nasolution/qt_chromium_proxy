#include "mainwindow.h"
#include "jsobjectinjector.h"
#include "kvwindow.h"
#include "settingsserver.h"
#include "kvwebuiextension.h"
#include "settingscontent.h"
#include "appsettings.h"
#include "webengineview.h"
#include "aboutwindow.h"
#include "logs.h"
#include "downloaddlg.h"
#include "downloadmanagerwidget.h"
#include "downloaddlg.h"
#include "KVVersion.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QContextMenuEvent>
#include <QMenu>
#include <QGroupBox>
#include <QPushButton>
#include <QFrame>
#include <QCheckBox>
#include <QTranslator>
#include <QTimer>
#include <QDir>
#include <QProcess>
#include "../KVMaintenanceTool/kvmtmainwindow.h"

DownloadManagerWidget* m_downloadManagerWidget = nullptr;

void MainWindow::createAutoUpdateObjects()
{
    if (!_isUtil) {
        _downloadDlg = new DownloadDlg(this);
        connect(_downloadDlg, &DownloadDlg::onUpdateAvailable, this, &MainWindow::handleOnUpdateAvailable);
        connect(_downloadDlg, &DownloadDlg::onUpdateRequested, this, &MainWindow::handleUpdateApplicationRequested);
        _timer = new QTimer(this);
        _timer->setInterval( ( (60 * 1000) * 60) * KV_CHECK_UPDATE );
        connect(_timer, &QTimer::timeout, _downloadDlg, &DownloadDlg::checkUpdate);
        if ( globalSettings->runAutoUpdate() ) {
            _timer->start();
        } else {
            _downloadDlg->checkUpdate();
        }
    }
}


void MainWindow::init()
{
    log_info(  "Windows width:" + QString::number(globalSettings->viewPortW()) + " Windows height:" + QString::number(globalSettings->viewPortH()) );
    this->resize(globalSettings->viewPortW(), globalSettings->viewPortH());

    _webView = new WebEngineView(_isUtil, this);
    connect(_webView, &WebEngineView::onDevToolsRequested, this,
            &MainWindow::handleDevToolsRequested);
    connect(_webView->page()->profile()->cookieStore(), &QWebEngineCookieStore::cookieAdded,
            this, &MainWindow::handleCookieAdded);
    connect(_webView->page()->profile()->cookieStore(), &QWebEngineCookieStore::cookieRemoved,
            this, &MainWindow::handleCookieRemoved);


    if (!_isUtil && m_downloadManagerWidget == nullptr) {
        m_downloadManagerWidget = new DownloadManagerWidget();
        m_downloadManagerWidget->setAttribute(Qt::WA_QuitOnClose, false);
    }
    QObject::connect(_webView->page()->profile(), &QWebEngineProfile::downloadRequested,
                     m_downloadManagerWidget, &DownloadManagerWidget::downloadRequested);


    this->setCentralWidget(_webView);
    menuBar()->addMenu(createViewMenu());
    //this->centralWidget()->setLayout(new QVBoxLayout() );
    //this->centralWidget()->layout()->addWidget(_view);

    QWebEnginePage* page = _webView->page();
    connect(page, &QWebEnginePage::loadFinished, [page, this](bool ok) {
        log_info(  "main page load finished. ok:" + QString::number(ok) );
            // TODO Pass a settings here in later phase.
        _webExt = new KvWebuiExtension(this);
        QObject::connect(_webExt, &KvWebuiExtension::onViewArchiveRequested, this, &MainWindow::onViewArchiveRequested);
        JsObjectInjector *jsInjector = new JsObjectInjector(this);
        jsInjector->InjectObject(page, "_kv_", _webExt);
    });


    addActions();
    createAutoUpdateObjects();


#ifdef DEBUG_SHOW_SUBWINDOW_ONLOAD
    if (!_isUtil) {
        KVWindow* window = new KVWindow("", "", "");
        window->show();
    }
#endif
#ifdef DEBUG_JAKE

#endif
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    init();
}

MainWindow::MainWindow(bool isUtil, QWidget *parent)
    : QMainWindow{parent}, _isUtil(isUtil)
{
    init();
}




void MainWindow::load()
{
#ifdef DEBUG_JAKE
    //_webView->load( QUrl("https://www.qt.io/download-open-source") );
    //return;
#endif
    QString urlStr = "";
    if (globalSettings->kenSecure()) {
        urlStr.append("https://");
    } else {
        urlStr.append("http://");
    }
    urlStr.append(globalSettings->kenIp());
    urlStr.append(":");
    urlStr.append(QString::number(globalSettings->kenPort()));
    _webView->load(urlStr);
}

QWebEnginePage *MainWindow::page() const
{
    return _webView->page();
}

QWebEngineView *MainWindow::webEngineView() const
{
    return _webView;
}

void MainWindow::addActions()
{
    QAction* action = new QAction();
    action->setShortcut( QKeySequence(Qt::CTRL | Qt::Key_W) );
    connect(action, &QAction::triggered, [=]() {
        this->close();
    });
    this->addAction(action);
}



void MainWindow::onViewArchiveRequested(const QString &url, const QString &archiveId, const QString &jsonText)
{
    KVWindow* window = new KVWindow(url, archiveId, jsonText, _cookies);
    window->show();
}



QFrame *MainWindow::createDebugSettings(QDialog* dlg, QCheckBox *cbDisableCache)
{
    cbDisableCache->setChecked(globalSettings->disableSimpleBrowserCache());
    QFrame* frame = new QFrame(this);
    QVBoxLayout* layout = new QVBoxLayout(frame);
    frame->setLayout(layout);


    QGroupBox* bg = new QGroupBox(this);
    QVBoxLayout* bgLayout = new QVBoxLayout(bg);
    bg->setMaximumHeight(100);
    bg->setLayout(bgLayout);
    QPushButton* btnArchiveDebug = new QPushButton( tr("Run Archives Debugger") );
    btnArchiveDebug->setIcon( QIcon(":/3rdparty/play-32.png") );
    connect(btnArchiveDebug, &QPushButton::clicked, [=]() {
        KVWindow* window = new KVWindow(globalSettings->kenIp(), "", "", _cookies, this, false, true);
        //window->setWindowTitle( tr("Archives Debugger") );
        dlg->close();
        window->show();
    });
    bgLayout->addWidget(btnArchiveDebug);
    frame->layout()->addWidget(bg);

    QGroupBox *gbCache = new QGroupBox(tr("Browser's Cache") );
    QHBoxLayout *cLayout = new QHBoxLayout();
    gbCache->setLayout(cLayout);
    QPushButton *btnClearCache = new QPushButton( tr("Clear Browser Cache") );
    btnClearCache->setIcon( QIcon(":/3rdparty/clear-cache-32.png") );
    btnClearCache->setEnabled( !globalSettings->disableSimpleBrowserCache() );
    cLayout->addWidget(cbDisableCache);
    cLayout->addWidget(btnClearCache);
    cLayout->addStretch();
    layout->addWidget(gbCache);

    connect(cbDisableCache, &QCheckBox::clicked, [=]() {
        btnClearCache->setEnabled( !cbDisableCache->isChecked() );
    });
    connect(btnClearCache, &QPushButton::clicked, [=]() {
        QMessageBox msgBox(this);
        msgBox.setText( tr("Clear Cache") );
        msgBox.setInformativeText( tr("Clear Browser Cache?") );
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        if (msgBox.exec() == QMessageBox::Ok) {
            _webView->page()->profile()->cookieStore()->deleteAllCookies();
            _webView->page()->profile()->clearHttpCache();
            log_info(  "Clearing Browser Cache Requested" );
        }
    });

    layout->addStretch();

    return frame;
}



void MainWindow::handleCreateSettingsWindow()
{
#ifdef DEBUG_JAKE
    KVWindow* window = new KVWindow("", "", "", this);
    window->show();
    return;
#endif

    QDialog* dlg = new QDialog(this);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);


    dlg->setWindowTitle( tr("Settings") );
    dlg->setModal(true);
    dlg->setWindowFlags(dlg->windowFlags() | Qt::Popup);
    dlg->setWindowModality(Qt::WindowModal);
    dlg->resize(450,550);
    dlg->setMaximumWidth(450);
    dlg->setMaximumHeight(550);
    dlg->setLayout(new QVBoxLayout());

    QTabWidget* tab = new QTabWidget();

    dlg->layout()->addWidget(tab);
    dlg->layout()->addWidget(buttonBox);


    SettingsServer* serverW = new SettingsServer();
    SettingsContent* contentW = new SettingsContent();
    connect(serverW, &SettingsServer::onImported, [=]() {
        contentW->loadSettings();
        serverW->loadSettings();
    });

    connect(contentW, &SettingsContent::onViewPortChanged, [=]() {
        this->resize(globalSettings->viewPortW(), globalSettings->viewPortH());
    });

    tab->addTab(serverW, tr("Server") );
    tab->addTab(contentW, tr("Content") );

    QCheckBox *cbDisableCache = new QCheckBox( tr("Disable Cache") );
    tab->addTab(createDebugSettings(dlg, cbDisableCache), tr("Debug") );
    dlg->show();


    connect(buttonBox, &QDialogButtonBox::accepted, [=]() {
        serverW->saveSettings();
        contentW->saveSettings();
        globalSettings->setDisableSimpleBrowserCache(cbDisableCache->isChecked());
        globalSettings->save();
        this->load();
        dlg->close();
    });
    connect(buttonBox, &QDialogButtonBox::rejected, [=]() {
        dlg->close();
    });
}

void MainWindow::handleDevToolsRequested()
{
    if (_webView->page()->devToolsPage() != nullptr) {
        return;
    }
    MainWindow* window = createDevToolsWindow();
    _webView->page()->setDevToolsPage( window->page() );
    _webView->page()->triggerAction(QWebEnginePage::InspectElement);
}

void MainWindow::handleOnUpdateAvailable()
{
    _downloadDlg->show();
}

void MainWindow::handleAutoUpdateRequested(bool autoUpdate)
{
    log_info("Auto update requested: " + QString::number(autoUpdate));
    globalSettings->setRunAutoUpdate(autoUpdate);
    if (autoUpdate) {
        _timer->start();
    } else {
        _timer->stop();
    }
}

void MainWindow::handleUpdateApplicationRequested()
{

#if defined(Q_OS_DARWIN) | defined(Q_OS_MACOS) | defined(Q_OS_WINDOWS) | defined(Q_OS_LINUX)
    KVMTMainWindow *w = new KVMTMainWindow(this);

    connect(w, &KVMTMainWindow::onClosingAfterUpdate, [=]() {
        this->close();
    });

    w->setWindowModality(Qt::WindowModal);
    w->show();
#else

    QString updaterPath = QDir::currentPath() + QDir::separator() + "KVMaintenanceTool";
    updaterPath = "/Users/jakeferrer/Documents/Upwork/younes4/upwork/build-simplebrowser-Qt_6_6_0_for_macOS-Debug/kv.app/Contents/MacOS/KVMaintenanceTool.app/";
    QStringList arguments;
    QProcess *myProcess = new QProcess(qApp);
    log_info("Running process: " + updaterPath);
    myProcess->startDetached(updaterPath, arguments);

    myProcess->start("open", {updaterPath});
    myProcess->waitForFinished();

//this->close();



#endif //defined(Q_OS_MAC) || defined(Q_OS_DARWIN)
}

void MainWindow::handleCookieAdded(const QNetworkCookie &cookie)
{
    QNetworkCookie cCookie = cookie;
    _cookies.push_back(cCookie);
}

void MainWindow::handleCookieRemoved(const QNetworkCookie &cookie)
{
    for (int i = 0; i < _cookies.size(); ++i) {
        QNetworkCookie cok = _cookies.at(i);
        if (cok.domain() == cookie.domain() &&
            cok.name() == cookie.name() &&
            cok.value() == cookie.value() &&
            cok.path() == cookie.path() )
            _cookies.removeAt(i);
        log_info( "Cookies remove: " + cok.toRawForm() );
        break;
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    _webView->deleteLater();
    emit onClosing();
    event->accept();
}


MainWindow *MainWindow::createDevToolsWindow()
{
    MainWindow* window = new MainWindow(this);
    //connect(window, &MainWindow::onClosing, [=]() {
    //_webView->page()->setDevToolsPage(nullptr);
    //window->deleteLater();
    //});
    window->show();
    return window;
}

bool MainWindow::askRestart()
{
    bool ret = false;
    QMessageBox msgBox(this);
    msgBox.setText( tr("Restart Application!") );
    msgBox.setInformativeText( tr("You need to restart to change language! Close Application now?") );
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if (msgBox.exec() == QMessageBox::Ok) {
        ret = true;
    }
    return ret;
}

void MainWindow::handleCheckUpdateWindowRequested()
{
    this->_downloadDlg->checkUpdate();
    this->_downloadDlg->hideNotes();
    this->_downloadDlg->show();
}

void MainWindow::handleAboutWindowRequested()
{
    if (_aboutWindow == nullptr) {
        _aboutWindow = new AboutWindow(this);
        connect(_aboutWindow, &AboutWindow::onAutoUpdateRequested, this, &MainWindow::handleAutoUpdateRequested);
        connect(_aboutWindow, &AboutWindow::onUpdateRequested, this, &MainWindow::handleCheckUpdateWindowRequested);
    }
    _aboutWindow->showCenter();
}

QMenu *MainWindow::createViewMenu()
{
    QMenu *viewMenu = new QMenu(tr("Options"));
    QAction *viewSettingsAction = new QAction( tr("Settings") );
    //viewSettingsAction->setShortcut(tr("Ctrl+s"));
    viewSettingsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_I));
    connect(viewSettingsAction, &QAction::triggered, this, &MainWindow::handleCreateSettingsWindow );
    viewMenu->addAction(viewSettingsAction);

    viewMenu->addSeparator();

    QMenu *languageMenu = new QMenu( tr("Language") );
    viewMenu->addMenu(languageMenu);
    QAction *enAction = new QAction( tr("English") );
    enAction->setIcon( QIcon(":/icons/flag_uk_16.png") );
    QAction *frAction = new QAction( tr("French") );
    frAction->setIcon( QIcon(":/icons/flag_france_16.png") );
    if (globalSettings->language() == "fr") {
        frAction->setCheckable(true);
        frAction->setChecked(true);
        frAction->setEnabled(false);
    } else {
        enAction->setCheckable(true);
        enAction->setChecked(true);
        enAction->setEnabled(false);
    }
    languageMenu->addAction(frAction);
    languageMenu->addAction(enAction);

    viewMenu->addSeparator();
    QAction *updateAction = new QAction( tr("Check for updates") );
    updateAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_U) );
    connect(updateAction, &QAction::triggered, [=]() {
        handleCheckUpdateWindowRequested();
    });
    viewMenu->addAction(updateAction);



    connect(enAction, &QAction::triggered, [=]() {
        if (!enAction->isChecked()) {
            if ( askRestart() ) {
                globalSettings->setLanguage("en");
                globalSettings->save();
                this->close();
                //qApp->exit(EXIT_CODE_REBOOT);
            }
        }
    });
    connect(frAction, &QAction::triggered, [=]() {
        if ( !frAction->isChecked() ) {
            if ( askRestart() ) {
                globalSettings->setLanguage("fr");
                globalSettings->save();
                this->close();
                //qApp->exit(EXIT_CODE_REBOOT);
            }
        }
    });

    viewMenu->addSeparator();

    QAction* aboutAction = new QAction( tr("About") );
    aboutAction->setShortcut(Qt::CTRL | Qt::Key_A);
    connect(aboutAction, &QAction::triggered, [=]() {
        handleAboutWindowRequested();
    });
    viewMenu->addAction(aboutAction);


    return viewMenu;
}
