// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QWebEngineFindTextResult>
#include <QWebEnginePage>
class KvWebuiExtension;

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

class WebView;

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWebEngineProfile *profile, QWidget *parent = nullptr, bool isMain = true);

    WebView *currentWebView() const;

signals:
    // current tab/page signals
    void linkHovered(const QString &link);
    void loadProgress(int progress);
    void titleChanged(const QString &title);
    void urlChanged(const QUrl &url);
    void favIconChanged(const QIcon &icon);
    void webActionEnabledChanged(QWebEnginePage::WebAction action, bool enabled);
    void devToolsRequested(QWebEnginePage *source);
    void findTextFinished(const QWebEngineFindTextResult &result);
    void onViewArchiveRequested(const QString& url, const QString& archiveId, const QString& jsonText);
public slots:
    // current tab/page slots
    void setUrl(const QUrl &url);
    void triggerWebPageAction(QWebEnginePage::WebAction action);

    WebView *createTab();
    WebView *createBackgroundTab();
    void closeTab(int index);
    void nextTab();
    void previousTab();
private slots:
    void handleCurrentChanged(int index);
    void handleContextMenuRequested(const QPoint &pos);
    void cloneTab(int index);
    void closeOtherTabs(int index);
    void reloadAllTabs();
    void reloadTab(int index);
    void onViewArchiveRequestedByKv(const QString& url, const QString& archiveId, const QString& jsonText);
private:
    WebView *webView(int index) const;
    void setupView(WebView *webView);

    QWebEngineProfile *m_profile;
    bool _isMain;
    KvWebuiExtension* _webExt = nullptr;
};

#endif // TABWIDGET_H
