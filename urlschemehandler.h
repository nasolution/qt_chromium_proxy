#ifndef URLSCHEMEHANDLER_H
#define URLSCHEMEHANDLER_H

#include <QWebEngineUrlSchemeHandler>

class QWebEngineUrlRequestJob;
class QNetworkAccessManager;


class UrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT
public:
    UrlSchemeHandler(QObject* parent = nullptr);
    void requestStarted(QWebEngineUrlRequestJob *job) override;
    void setArchiveId(const QString &archiveId);
    static QUrl getArchiveUrl(QUrl url, const QString &archiveId);
    QByteArray scheme;
private:
    void mangleUrlAndRequest(QWebEngineUrlRequestJob *job);

private:
    QNetworkAccessManager *m_netMgr;
    QString archiveId;
};

#endif // URLSCHEMEHANDLER_H
