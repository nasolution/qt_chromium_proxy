// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef BROWSER_H
#define BROWSER_H

#include "downloadmanagerwidget.h"

#include <QList>
#include <QWebEngineProfile>

class BrowserWindow;

class Browser: public QObject
{
    Q_OBJECT
public:
    Browser(QObject* parent = nullptr);

    QList<BrowserWindow*> windows() { return m_windows; }

    BrowserWindow *createHiddenWindow(bool offTheRecord = false, bool isMain = true);
    BrowserWindow *createWindow(bool offTheRecord = false);
    BrowserWindow *createDevToolsWindow();

    DownloadManagerWidget &downloadManagerWidget() { return m_downloadManagerWidget; }
private slots:
    void onViewArchiveRequested(const QString& url, const QString& archiveId, const QString& jsonText);
private:
    QList<BrowserWindow*> m_windows;
    DownloadManagerWidget m_downloadManagerWidget;
    QScopedPointer<QWebEngineProfile> m_profile;
};
#endif // BROWSER_H
