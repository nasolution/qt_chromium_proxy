#include "urlschemehandler.h"
#include "logs.h"
#include <QWebEngineUrlRequestJob>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>

UrlSchemeHandler::UrlSchemeHandler(QObject* parent): QWebEngineUrlSchemeHandler(parent)
{

}


void UrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob *job)
{
    const QByteArray method = job->requestMethod();
    const QUrl url = job->requestUrl();

    //L_TRACE("requestStarted: {} {}", method.constData(), url.toString());

    //auto requestedHeaders = job->requestHeaders();
    //for (auto it = requestedHeaders.constBegin(); it != requestedHeaders.constEnd(); ++it) {
    //    L_TRACE("requestStarted: {} {}", it.key().constData(), it.value().constData());
    //}

    mangleUrlAndRequest(job);

    //static QByteArray tmpBa = "Hello World!";
    //QBuffer *buffer = new QBuffer(&tmpBa);
    //job->reply("text/html", buffer);

    //job->fail(QWebEngineUrlRequestJob::RequestDenied);
}

void UrlSchemeHandler::setArchiveId(const QString &archiveId)
{
    this->archiveId = archiveId;
}

QUrl UrlSchemeHandler::getArchiveUrl(QUrl url, const QString &archiveId)
{
    QByteArray ba;
    ba.append(url.toString().toStdString().c_str());

    QString surl;
    surl.append("http://");
    surl.append("ken2.aleph-archives.com");  // TODO
    surl.append(':');
    surl.append(QString::number(80));        // TODO
    surl.append("/get/");
    surl.append(archiveId);        // TODO
    surl.append('/');
    surl.append(ba.toBase64());

    return surl;
}

void UrlSchemeHandler::mangleUrlAndRequest(QWebEngineUrlRequestJob *job)
{
    const QByteArray method = job->requestMethod();
    const QUrl url = job->requestUrl();

    QUrl newUrl = url;
    newUrl.setScheme("https");

    QUrl mangledUrl = getArchiveUrl(newUrl, archiveId);

    //L_INFO("mangle from {} {} to {}", job->requestMethod().constData(),
    //       newUrl.toString(), mangledUrl.toString());

    QNetworkRequest request(mangledUrl);
    //request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

    auto requestedHeaders = job->requestHeaders();
    for (auto it = requestedHeaders.constBegin(); it != requestedHeaders.constEnd(); ++it) {
        request.setRawHeader(it.key(), it.value());
    }

    QNetworkReply *reply;

    if (job->requestMethod() == "POST") {
        //reply = m_netMgr->post(request, );
        log_warning("POST currently ignored");

        job->fail(QWebEngineUrlRequestJob::RequestFailed);
        return;
    } else {
        reply = m_netMgr->get(request);
    }


    //job->setProperty("reply", QVariant::fromValue(reply));

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            job->fail(QWebEngineUrlRequestJob::RequestFailed);
            return;
        }

        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode >= 400) {
            job->fail(QWebEngineUrlRequestJob::RequestFailed);
            return;
        }

        const QByteArray contentType = reply->header(QNetworkRequest::ContentTypeHeader).toByteArray();
        if (contentType.isEmpty()) {
            job->fail(QWebEngineUrlRequestJob::RequestFailed);
            return;
        };
        job->reply(contentType, reply);
    });

    // Add a timeout for waiting the reply finished.
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    timer->start(5000);
    QObject::connect(timer, &QTimer::timeout, [=]() {
        //log_trace("timer timeout. isFinished: {}", reply->isFinished());
        /**
        if (reply->isFinished() == false) {
            log_warning("reply timeout. return request failed.");
            job->fail(QWebEngineUrlRequestJob::RequestFailed);
        }
        **/
    });

    QObject::connect(job, &QWebEngineUrlRequestJob::destroyed, [=]() {
        //QNetworkReply *reply = job->property("reply").value<QNetworkReply *>();
        //reply->disconnect();
        //reply->deleteLater();
    });
}
