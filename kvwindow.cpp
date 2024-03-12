#include "kvwindow.h"
#include "requestinterceptor.h"
#include "urlschemehandler.h"
#include "webpageexporter.h"
#include "webengineview.h"
#include "mainwindow.h"
#include "appsettings.h"
#include "logs.h"
#include "downloadmanagerwidget.h"
#include "KVVersion.h"
#include <QWebEngineProfile>
#include <QLabel>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QWebEngineSettings>
#include <QToolBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDesktopServices>
#include <QLocale>
#include <QCloseEvent>
#include <QNetworkProxy>





KVWindow::KVWindow(const QString &url, const QString &id, const QString &json, const QList<QNetworkCookie>& cookies, QWidget *parent, bool doIntercept, bool archivesDebugger)
    : QMainWindow{parent},
    _kvUrl(url),
    _kvId(id),
    _kvJson(json),
    _archivesDebugger(archivesDebugger)
{
    log_info("::: Windows width:  " + QString::number(globalSettings->viewPortW()) );
    log_info("::: Windows height: " + QString::number(globalSettings->viewPortH()) );
    this->setMinimumSize(100, 100);
    this->resize(globalSettings->viewPortW(), globalSettings->viewPortH());

    createEngineProfile(url, id, doIntercept, cookies);
    _webView = new WebEngineView(m_profile, this);
    _webView->setForKVWindow();
    QObject::connect(_webView->page()->profile(), &QWebEngineProfile::downloadRequested,
                     m_downloadManagerWidget, &DownloadManagerWidget::downloadRequested);

    this->setCentralWidget(_webView);
    this->addToolBar(generateToolBar());
    this->setStatusBar(generateFooter());
    applyLocale();
    applyKVParamsToLabels();
    handleSignals(url);
    //connect(_view, &QWebEngineView::favIconChanged, m_favAction, &QAction::setIcon);
    addActions();

    for (int i = 0; i < cookies.size(); ++i) {
        QNetworkCookie cook = cookies.at(i);
        _webView->page()->profile()->cookieStore()->setCookie(cook);
    }
    _webView->load(url);

}




void KVWindow::applyKVParamsToLabels()
{
    if (_archivesDebugger)
        return;
    if (_kvId.size() > 0)
        _labelKVsiteName->setText(_kvId);
    if (_kvUrl.size() > 0)
        _labelKVId->setText(_kvUrl);
    QJsonDocument doc = QJsonDocument::fromJson(_kvJson.toUtf8());
    QJsonObject obj = doc.object();
    QDateTime d;
    QString siteName = obj["name"].toString();
    if (siteName.size() > 0) {
        this->setWindowTitle(siteName);
        d.setSecsSinceEpoch((uint)obj["running_time"].toDouble());
        _labelKVDate->setText(d.toString());
    }
}


void KVWindow::handleSignals(const QString &url)
{
    Q_UNUSED(url);
    connect(_webView, &WebEngineView::onDevToolsRequested, this, &KVWindow::handleDevToolsRequested);
    connect(_webView, &WebEngineView::loadProgress, this, &KVWindow::handleLoadProgress);
    connect(_webView, &WebEngineView::loadFinished, this, [=]() {
        handleLoadFinished();
    });
    connect(_webView, &WebEngineView::loadStarted, this, [=]() {
        handleLoadStarted();
    });
    connect(_webView, &WebEngineView::urlChanged, [](const QUrl &url) {
        Q_UNUSED(url);
#ifndef DEBUG_HIDE_INTERCEPTED_URL
        m_urlLineEdit->setText(url.toDisplayString());
#endif //DEBUG_HIDE_INTERCEPTED_URL
    });
}




void KVWindow::applyLocale()
{
    if (globalSettings->localeIndex() > -1) {
        QList<QLocale> allLocales = QLocale::matchingLocales(
            QLocale::AnyLanguage,
            QLocale::AnyScript,
            QLocale::AnyCountry);
        if (allLocales.count() >= globalSettings->localeIndex() ) {
            QLocale locale = allLocales.at(globalSettings->localeIndex() );
            _webView->setLocale( locale );
            log_info("::: setting Locale to " + QLocale::languageToString( locale.language() ) +
                     "_" + QLocale::countryToString( locale.country() ) );
        } else {
            log_info( "::: selected Locale not Available" );
        }
        QLocale locale = _webView->locale();
        log_info( "::: WebView Locale: " + QLocale::languageToString( locale.language() ) +
                 "_" + QLocale::countryToString( locale.country() ) );
    }
}



void KVWindow::createEngineProfile(const QString &url, const QString &id, bool doIntercept, const QList<QNetworkCookie> &cookies)
{
    m_profile = new QWebEngineProfile(this);
#ifdef DEBUG_NO_INTERCEPT
    qWarning() << "Disabled interception for debugging";
#else
    if (doIntercept) {
        RequestInterceptor* interceptor = new RequestInterceptor();
        interceptor->setKVParams(id, url);
        m_profile->setUrlRequestInterceptor(interceptor);
        if (this->_archivesDebugger) {
            interceptor->setIsArchiveDebugger(true);
        }
#ifdef DEBUG_HIDE_INTERCEPTED_URL
        connect(interceptor, &RequestInterceptor::onUrlIntercepted, this, &KVWindow::handleInterceptorUrlChanged);
#endif //DEBUG_HIDE_INTERCEPTED_URL
    }
#endif //DEBUG_NO_INTERCEPT
    //
    if ( globalSettings->disableSimpleBrowserCache() ) {
        m_profile->setHttpCacheType(QWebEngineProfile::NoCache);
    } else {
        m_profile->setHttpCacheType(QWebEngineProfile::MemoryHttpCache);
    }
    m_profile->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    m_profile->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);
}



void KVWindow::addActions()
{
    QAction* action = new QAction();
    action->setShortcut( QKeySequence(Qt::CTRL | Qt::Key_W) );
    connect(action, &QAction::triggered, [=]() {
        this->close();
    });
    this->addAction(action);
}

void KVWindow::handleLoadFinished()
{
    handleLoadProgress(100);
    log_info( "::: Loading Finished" );
    if ( globalSettings->enableHarmonizer() && !this->_archivesDebugger) {
        QString harmonizerCode = "if (!document.getElementById('__kv_script__')) { var x=document.createElement('script');x.id='__kv_script__';x.setAttribute('src',window.location.protocol.split(':')[0]+'://keeper.of.the.gro.ve/_kv-harmonizer.js');setTimeout(function(){document.body.appendChild(x);},0);}";
        _webView->page()->runJavaScript(harmonizerCode);
        log_info( "::: ran Javascript Harmonizer code" );
    }
}

void KVWindow::handleLoadStarted()
{
    log_info( "::: Loading started" );
    this->_initialInterceptorUrlHandled = false;
    pdfAction->setEnabled(false);
    pngAction->setEnabled(false);
}


void KVWindow::handleLoadProgress(int progress)
{
    static QIcon stopIcon(":process-stop.png");
    static QIcon reloadIcon(":/icons/refresh.png");

    if (0 < progress && progress < 100) {
        m_stopReloadAction->setData(QWebEnginePage::Stop);
        m_stopReloadAction->setIcon(stopIcon);
        m_stopReloadAction->setToolTip( tr("Stop loading the current page") );
        m_progressBar->setValue(progress);
    } else {
        m_stopReloadAction->setData(QWebEnginePage::Reload);
        m_stopReloadAction->setIcon(reloadIcon);
        m_stopReloadAction->setToolTip( tr("Reload the current page") );
        m_progressBar->setValue(0);
    }
    if (progress > 99) {
        pngAction->setEnabled(true);
        pdfAction->setEnabled(true);
    }
}

void KVWindow::handleDevToolsRequested()
{
    if (_webView->page()->devToolsPage() != nullptr) {
        return;
    }
    MainWindow* window = createDevToolsWindow();
    _webView->page()->setDevToolsPage( window->page() );
    _webView->page()->triggerAction(QWebEnginePage::InspectElement);
}

MainWindow *KVWindow::createDevToolsWindow()
{
    MainWindow* window = new MainWindow(true, this);
    //connect(window, &MainWindow::onClosing, [=]() {
    //    _webView->page()->setDevToolsPage(nullptr);
    //    window->deleteLater();
    //});
    window->show();
    return window;
}

void KVWindow::closeEvent(QCloseEvent *event)
{
    _webView->deleteLater();
    emit onClosing();
    event->accept();
}

void KVWindow::openExported(const QString &path)
{
    QMessageBox msgBox(this);
    msgBox.setText( tr("Open Exported File") );
    msgBox.setInformativeText( tr("Do you want to open the exported file?") );
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if (msgBox.exec() == QMessageBox::Ok) {
        QUrl fileUrl = QUrl::fromLocalFile(path);
        QDesktopServices::openUrl(fileUrl);
    }
    bringToFront();
}





QStatusBar *KVWindow::generateFooter()
{
    QStatusBar* bar = new QStatusBar();
    QLabel* idIconLabel = new QLabel("<html><img src=':/icons/lock.png'></html>");
    _labelKVId = new QLabel(ARCHIVE_UUID_DEFAULT);
    QLabel* siteIconLabel = new QLabel("<html><img src=':/icons/file.png'></html>");
    _labelKVsiteName = new QLabel(ARCHIVE_NAME_DEFAULT);
    QLabel* dateIconLabel = new QLabel("<html><img src=':/icons/cal.png'></html>");
    _labelKVDate = new QLabel(ARCHIVE_DATE_DEFAULT);
    this->setWindowTitle(ARCHIVE_TITLE_DEFAULT);

    m_progressBar = new QProgressBar();
    bar->layout()->addWidget(m_progressBar);
    bar->layout()->addWidget(siteIconLabel);
    bar->layout()->addWidget(_labelKVsiteName);
    bar->layout()->addWidget(idIconLabel);
    bar->layout()->addWidget(_labelKVId);
    bar->layout()->addWidget(dateIconLabel);
    bar->layout()->addWidget(_labelKVDate);
    return bar;
}

void KVWindow::bringToFront()
{
    this->raise();
    this->activateWindow();
}

void KVWindow::onFailedExport()
{
    QMessageBox msgBox(this);
    msgBox.setText( tr("Failed") );
    msgBox.setInformativeText( tr("Failed exporting to PNG/PDF") );
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    bringToFront();
}

void KVWindow::handlePDFExport(bool triggered = false)
{
    Q_UNUSED(triggered);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("PDF (*.pdf)"));
    if (fileName == "") return;
    WebPageExporter* exporter = new WebPageExporter(this);
    if (this->_archivesDebugger)
        exporter->setIsArchivesDebugger(true);
    connect(exporter, &WebPageExporter::onPDFExportFinished,
            [=](const QString& filePath, bool success) {
        Q_UNUSED(filePath)
        if (success) {
            bringToFront();
            openExported(fileName);
        } else {
            onFailedExport();
        }
    });
    exporter->exportToPdf(_webView->page(), fileName);
}

void KVWindow::handleInterceptorUrlChanged(const QString &url)
{
    if (this->_initialInterceptorUrlHandled) return;
    this->_initialInterceptorUrlHandled = true;
    this->m_urlLineEdit->setText(url);
}

void KVWindow::handlePNGExport(bool triggered = false)
{
    Q_UNUSED(triggered);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("PNG Image(*.png)"));
    if (fileName == "") return;
    WebPageExporter* exporter = new WebPageExporter(this);
    if (this->_archivesDebugger)
        exporter->setIsArchivesDebugger(true);
    connect(exporter, &WebPageExporter::onPNGExportFinished,
            [=] (const QString& filePath, bool success) {
        Q_UNUSED(filePath);
        if (success) {
            bringToFront();
            openExported(fileName);
        } else {
            onFailedExport();
        }
    });
    exporter->exportToPng(_webView->page(), fileName);
}

QToolBar *KVWindow::generateToolBar()
{
    QToolBar *navigationBar = new QToolBar( tr("Navigation") );
    navigationBar->setMinimumHeight(16);

    navigationBar->setMovable(false);
    navigationBar->toggleViewAction()->setEnabled(false);

    m_historyBackAction = new QAction();
    //auto backShortcuts = removeBackspace(QKeySequence::keyBindings(QKeySequence::Back));
    // For some reason Qt doesn't bind the dedicated Back key to Back.
    //backShortcuts.append(QKeySequence(Qt::Key_Back));
    //m_historyBackAction->setShortcuts(backShortcuts);
    m_historyBackAction->setIconVisibleInMenu(false);
    m_historyBackAction->setIcon(QIcon(":/icons/go_back.png"));
    m_historyBackAction->setToolTip( tr("Go back in history") );
    connect(m_historyBackAction, &QAction::triggered, [this]() {
        _webView->triggerPageAction(QWebEnginePage::Back);
    });
    navigationBar->addAction(m_historyBackAction);

    m_historyForwardAction = new QAction();
    //auto fwdShortcuts = removeBackspace(QKeySequence::keyBindings(QKeySequence::Forward));
    //fwdShortcuts.append(QKeySequence(Qt::Key_Forward));
    //m_historyForwardAction->setShortcuts(fwdShortcuts);
    m_historyForwardAction->setIconVisibleInMenu(false);
    m_historyForwardAction->setIcon(QIcon(":/icons/go_next.png"));
    m_historyForwardAction->setToolTip( tr("Go forward in history") );
    connect(m_historyForwardAction, &QAction::triggered, [this]() {
        _webView->triggerPageAction(QWebEnginePage::Forward);
    });
    navigationBar->addAction(m_historyForwardAction);

    QAction* homeAction = new QAction(this);
    homeAction->setIcon(QIcon(":/icons/home.png"));
    homeAction->setToolTip( tr("Home") );
    navigationBar->addAction(homeAction);
    connect(homeAction, &QAction::triggered, [=]() {
        _webView->load(_kvUrl);
    });

    m_urlLineEdit = new QLineEdit(this);
    m_favAction = new QAction(this);
    m_favAction->setIcon( QIcon(":/icons/padlock.png") );
    m_favAction->setToolTip(tr("Access to this page is secured by Aleph Archives"));
    m_urlLineEdit->addAction(m_favAction, QLineEdit::LeadingPosition);
    m_urlLineEdit->setClearButtonEnabled(true);
    navigationBar->addWidget(m_urlLineEdit);


    connect(m_urlLineEdit, &QLineEdit::returnPressed, [this]() {
        _webView->load(QUrl::fromUserInput(m_urlLineEdit->text()));
    });

    m_stopReloadAction = new QAction(this);
    connect(m_stopReloadAction, &QAction::triggered, [this]() {
        _webView->triggerPageAction(QWebEnginePage::WebAction(m_stopReloadAction->data().toInt()));
    });
    navigationBar->addAction(m_stopReloadAction);

    /**
    auto downloadsAction = new QAction(this);
    downloadsAction->setIcon(QIcon(u":go-bottom.png"_s));
    downloadsAction->setToolTip(tr("Show downloads"));
    navigationBar->addAction(downloadsAction);
    connect(downloadsAction, &QAction::triggered,
            &m_browser->downloadManagerWidget(), &QWidget::show);
    **/

    pdfAction = new QAction(this);
    pdfAction->setIcon(QIcon(":/icons/pdf.png"));
    navigationBar->addAction(pdfAction);
    pdfAction->setToolTip( tr("Export PDF") );
    pdfAction->setShortcut( QKeySequence(Qt::CTRL | Qt::Key_P) );
    connect(pdfAction, &QAction::triggered, this, &KVWindow::handlePDFExport);

    pngAction = new QAction(this);
    pngAction->setIcon(QIcon(":/icons/png.png"));
    pngAction->setToolTip( tr("Export PNG") );
    pngAction->setShortcut( QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_P) );
    navigationBar->addAction(pngAction);
    connect(pngAction, &QAction::triggered, this, &KVWindow::handlePNGExport);

    /**
    QAction *reloadKeyAction = new QAction(this);
    reloadKeyAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    this->addAction(reloadKeyAction);
    navigationBar->addAction(reloadKeyAction);
    connect(reloadKeyAction, &QAction::triggered, [=] {
        _webView->reload();
    });
**/

    return navigationBar;
}