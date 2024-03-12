// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QWebEnginePage>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QProgressBar;
QT_END_NAMESPACE

class Browser;
class TabWidget;
class WebView;
class QLabel;

class BrowserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BrowserWindow(Browser *browser, QWebEngineProfile *profile,
                           bool forDevTools = false, bool isMain = false);
    QSize sizeHint() const override;
    TabWidget *tabWidget() const;
    WebView *currentTab() const;
    Browser *browser() { return m_browser; }
    void setKvParams(const QString& url, const QString& id, const QString& json);
protected:
    void closeEvent(QCloseEvent *event) override;
public slots:
    void onViewArchiveRequestedKv(const QString& url, const QString& archiveId, const QString& jsonText);
private slots:
    void handleNewWindowTriggered();
    void handleNewIncognitoWindowTriggered();
    void handleFileOpenTriggered();
    void handleFindActionTriggered();
    void handleShowWindowTriggered();
    void handleWebViewLoadProgress(int);
    void handleWebViewTitleChanged(const QString &title);
    void handleWebActionEnabledChanged(QWebEnginePage::WebAction action, bool enabled);
    void handleDevToolsRequested(QWebEnginePage *source);
    void handleFindTextFinished(const QWebEngineFindTextResult &result);
    void handleCreateSettingsWindow();

signals:
    void onViewArchiveRequested(const QString& url, const QString& archiveId, const QString& jsonText);
private:
    QMenu *createFileMenu(TabWidget *tabWidget);
    QMenu *createEditMenu();
    QMenu *createViewMenu(QToolBar *toolBar);
    QMenu *createWindowMenu(TabWidget *tabWidget);
    QMenu *createHelpMenu();
    QToolBar *createToolBar();
    QStatusBar *createStatusBar();

private:
    Browser *m_browser;
    QWebEngineProfile *m_profile;
    TabWidget *m_tabWidget;
    QProgressBar *m_progressBar = nullptr;
    QAction *m_historyBackAction = nullptr;
    QAction *m_historyForwardAction = nullptr;
    QAction *m_stopAction = nullptr;
    QAction *m_reloadAction = nullptr;
    QAction *m_stopReloadAction = nullptr;
    QLineEdit *m_urlLineEdit = nullptr;
    QAction *m_favAction = nullptr;
    QString m_lastSearch;
    QString _kvUrl;
    QString _kvId;
    QString _kvJson;
    QLabel* _labelKVId;
    QLabel* _labelKVsiteName;
    QLabel* _labelKVDate;
};

#endif // BROWSERWINDOW_H
