#ifndef KVWINDOW_H
#define KVWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QLabel>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>
#include <QMenu>
#include <QProgressBar>
#include <QNetworkCookie>

class WebEngineView;
class MainWindow;
class KVWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit KVWindow(const QString& url, const QString& id, const QString& json, const QList<QNetworkCookie> &cookies, QWidget *parent = nullptr, bool doIntercept = true, bool archivesDebugger = false);
    void applyKVParamsToLabels();

signals:
    void onClosing();
private slots:
    void handleLoadProgress(int progress);
    void handleDevToolsRequested();
    void handlePNGExport(bool triggered);
    void handlePDFExport(bool triggered);
    void handleInterceptorUrlChanged(const QString& url);
private:
    void closeEvent(QCloseEvent *event) override;

    void bringToFront();
    void onFailedExport();
    void openExported(const QString& path);

    QStatusBar* generateFooter();
    QToolBar *generateToolBar();
    MainWindow* createDevToolsWindow();

    QString _kvUrl;
    QString _kvId;
    QString _kvJson;
    bool _archivesDebugger = false;
    QLabel* _labelKVId;
    QLabel* _labelKVsiteName;
    QLabel* _labelKVDate;
    WebEngineView* _webView;
    QWebEngineProfile* m_profile;
    QAction* m_historyBackAction;
    QAction* m_historyForwardAction;
    QAction* m_favAction;
    QAction* m_stopReloadAction;
    QAction* pngAction;
    QAction* pdfAction;
    QLineEdit* m_urlLineEdit;
    QProgressBar *m_progressBar;
    bool _initialInterceptorUrlHandled = false;
    
    void addActions();
    void handleLoadFinished();
    void handleLoadStarted();
    void applyLocale();
    void createEngineProfile(const QString &url, const QString &id, bool doIntercept, const QList<QNetworkCookie> &cookies);
    void handleSignals(const QString &url);
};

#endif // KVWINDOW_H
