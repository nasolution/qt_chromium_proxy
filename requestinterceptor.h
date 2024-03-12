#ifndef REQUESTINTERCEPTOR_H
#define REQUESTINTERCEPTOR_H

#include <QWebEngineUrlRequestInterceptor>

class RequestInterceptor : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT
public:
    explicit RequestInterceptor(QObject * parent = Q_NULLPTR);
    virtual void interceptRequest(QWebEngineUrlRequestInfo & info) Q_DECL_OVERRIDE;
    void setKVParams(const QString& uuid, const QString& url);
    void setIsArchiveDebugger(bool val) { _isArchiveDebugger = val; }
signals:
    void onUrlIntercepted(const QString& url);
 private:
    QUrl buildArchiveUrl(QString dp, QString cobalt_prefix, QString arcId, QString arcHost);
    QUrl    rawUrl(QWebEngineUrlRequestInfo & info);
    void    addCustomHTTPHeaders(QWebEngineUrlRequestInfo &info);
    QString generateCobaltPrefix();
    QString generateCobaltAuth();
    QString _archiveUrl;
    QString _archiveUUID;
    QString _archiveSchemeAuthority;
    bool _isArchiveDebugger = false;
};

#endif // REQUESTINTERCEPTOR_H
