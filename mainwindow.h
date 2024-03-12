#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QMenu>
#include <QTranslator>
#include <QWebEngineCookieStore>
#include <QNetworkCookie>

class QWebEnginePage;
class KvWebuiExtension;
class WebEngineView;
class QFrame;
class DownloadManagerWidget;
class QCheckBox;
class QAction;
class QEvent;
class DownloadDlg;
class QTimer;
class AboutWindow;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(bool isUtil, QWidget *parent = nullptr);
    void load();
    QWebEnginePage* page() const;
    QWebEngineView* webEngineView() const;
    void init();
    static int const EXIT_CODE_REBOOT = -3310;
signals:
    void onClosing();
private slots:
    void onViewArchiveRequested(const QString& url, const QString& archiveId, const QString& jsonText);
    void handleCreateSettingsWindow();
    void handleDevToolsRequested();
    void handleOnUpdateAvailable();
    void handleAutoUpdateRequested(bool autoUpdate);
    void handleUpdateApplicationRequested();
    void handleCookieAdded(const QNetworkCookie &cookie);
    void handleCookieRemoved(const QNetworkCookie &cookie);
private:

    QFrame *createDebugSettings(QDialog *dlg, QCheckBox *cbDisableCache);
    void closeEvent(QCloseEvent *event) override;
    MainWindow* createDevToolsWindow();
    QMenu* createViewMenu();
    WebEngineView* _webView;
    KvWebuiExtension* _webExt;
    void addActions();
    bool _isUtil = false;
    QTranslator m_translator;
    QTranslator m_translatorQt;
    bool askRestart();
    DownloadDlg *_downloadDlg;
    void createAutoUpdateObjects();
    QTimer *_timer;
    void handleCheckUpdateWindowRequested();
    void handleAboutWindowRequested();
    AboutWindow* _aboutWindow = nullptr;
    QList<QNetworkCookie> _cookies;
};
    extern DownloadManagerWidget* m_downloadManagerWidget;
#endif // MAINWINDOW_H
