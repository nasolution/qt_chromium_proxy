#include "requestinterceptor.h"
#include "logs.h"
#include "appsettings.h"
#include "logs.h"
#include "KVVersion.h"
#include <QDebug>

using namespace std;

RequestInterceptor::RequestInterceptor(QObject * parent) : QWebEngineUrlRequestInterceptor(parent)
{}

// QUrl RequestInterceptor::buildArchiveUrl(QString dp, QString cobalt_prefix, QString arcId, QString arcHost)
// {
//     /*
//      * old: "patents/static/images/pg_logo.png"
//      * old: "https://www.pg.com/patents/static/images/pg_logo.png"
//      *  ->
//      * new: QUrl("https://cobalt.aleph-archives.com:8443/get/6c33efd6-a6f1-4001-81cf-01bd8710652a/https://www.pg.com/patents/static/images/pg_logo.png"
//      */

//     QString surl;
//     surl.append(cobalt_prefix);
//     surl.append(arcId);

//     if(0 == dp.length() || '/' != dp.at(0)) surl.append("/");
//     surl.append(arcHost);
//     surl.append(dp);

// #ifdef DEBUG_HIDE_INTERCEPTED_URL
//     QString origUrl;
//     if(0 == dp.length() || '/' != dp.at(0)) origUrl.append("/");
//     origUrl.append(arcHost);
//     origUrl.append(dp);
//     if (origUrl.startsWith("/")) origUrl = origUrl.right(origUrl.size() - 1);
//     emit onUrlIntercepted(origUrl);
// #endif

//     return QUrl::fromUserInput(surl);
// }

QUrl RequestInterceptor::buildArchiveUrl(QString dp, QString cobalt_prefix, QString arcId, QString arcHost)
{
    /*
     * old: "patents/static/images/pg_logo.png"
     * old: "https://www.pg.com/patents/static/images/pg_logo.png"
     *  ->
     * new: QUrl("https://cobalt.aleph-archives.com:8443/get/6c33efd6-a6f1-4001-81cf-01bd8710652a/https://www.pg.com/patents/static/images/pg_logo.png"
     */

    QString surl;
    surl.append(cobalt_prefix);
    surl.append(arcId);

    if(0 == dp.length() || '/' != dp.at(0)) surl.append("/");
    surl.append(arcHost);
    surl.append(dp);

#ifdef DEBUG_HIDE_INTERCEPTED_URL
    QString origUrl;
    if(0 == dp.length() || '/' != dp.at(0)) origUrl.append("/");
    origUrl.append(arcHost);
    origUrl.append(dp);
    if (origUrl.startsWith("/")) origUrl = origUrl.right(origUrl.size() - 1);
    emit onUrlIntercepted(origUrl);
#endif

    return QUrl::fromUserInput(surl);
}

QString dropCobaltPrefix(const string &str, const char *cobaltAuth)
{
    /*
     * old: "https://cobalt.aleph-archives.com:8443/patents/static/images/pg_logo.png"
     *  ->
     * new: "patents/static/images/pg_logo.png"
     */
    const string delimiter(cobaltAuth);
    size_t found = str.rfind(delimiter);
    if (found != string::npos)
        return QString::fromStdString(str.substr(found + delimiter.length(), str.length() - 1));
    else
        return QString::fromStdString(str);
}

bool shouldInterceptRequest(QWebEngineUrlRequestInfo &info)
{
    const QString url = info.requestUrl().toString().toLower();

    // free pass to DevTools
    if (url.contains("devtools://")) return false;

    // free pass to about
    if (url.contains("about://")) return false;

    // free pass to chrome extension (ex. PDF Viewer)
    if (url.contains("chrome://")       ||
        url.contains("chrome-error://") ||
        url.contains("chrome-extension://")) return false;

   return true;
}

void RequestInterceptor::addCustomHTTPHeaders(QWebEngineUrlRequestInfo &info)
{
    if (globalSettings->useCustomUA() && !_isArchiveDebugger) {
        const QByteArray uaHeader = globalSettings->customUA().toUtf8();
        const QByteArray uaName = "User-Agent";
        info.setHttpHeader(uaName, uaHeader);
    }

    if (globalSettings->enableLocale() && globalSettings->localeIndex() > -1 && !_isArchiveDebugger) {
        QList<QLocale> allLocales = QLocale::matchingLocales(
            QLocale::AnyLanguage,
            QLocale::AnyScript,
            QLocale::AnyCountry);
        if (allLocales.count() >= globalSettings->localeIndex() ) {
            const QByteArray lName = "Accept-Language";
            const QLocale locale = allLocales.at(globalSettings->localeIndex() );
            QString lHeader = QLocale::languageToCode( locale.language() );
            lHeader.append("-");
            lHeader.append( QLocale::countryToCode(locale.country() ) );
            info.setHttpHeader( lName, lHeader.toUtf8() );
        }
    }

    // always append Cobalt's custom HTTP headers
    const QByteArray httpHeaderName1  = "X-ARCHIVE-ID";
    const QByteArray httpHeaderValue1 = _archiveUUID.toUtf8();
    info.setHttpHeader(httpHeaderName1, httpHeaderValue1);

    const QByteArray httpHeaderName2  = "X-ARCHIVE-HOST";
    const QByteArray httpHeaderValue2 = _archiveUrl.toUtf8();
    info.setHttpHeader(httpHeaderName2, httpHeaderValue2);
}

QUrl RequestInterceptor::rawUrl(QWebEngineUrlRequestInfo &info)
{
    // browsers usually implicitely hide default port from URL: 443 | 80
    QUrl url = info.requestUrl();
    // default port detected, use it explicitely
    if( -1 == url.port() ) url.setPort(globalSettings->cobaltPort());
    return url;
}

void RequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    if( false == shouldInterceptRequest(info) ) return;

    addCustomHTTPHeaders(info); // add custom KV HTTP headers for COBALT

    const QString cobaltPrefix = generateCobaltPrefix();
    const QString cobaltAuth   = generateCobaltAuth();

    const QString raw = rawUrl(info).toString();
    if ( raw.contains(cobaltPrefix) ) return;

    const QString arcHost = raw.contains(cobaltAuth) ? _archiveSchemeAuthority : QString("");
    const QString arcId   = _archiveUUID;

    const QUrl originalUrl = info.requestUrl();
    const QString dp = dropCobaltPrefix(originalUrl.toString().toStdString(), cobaltAuth.toStdString().c_str());

    const QUrl redirectUrl = buildArchiveUrl(dp, cobaltPrefix, arcId, arcHost);

    log_info(  "------------------------" );
    //qDebug() << ":::Host: " << arcHost;
    //qDebug() << ":::  dp: " << dp;
    log_info(  "::: original: " + originalUrl.toString() );
    log_info(  "::: redirect: " + redirectUrl.toString() );

    // perform HTTP redirect
    // info.redirect(redirectUrl);
}

void RequestInterceptor::setKVParams(const QString &uuid, const QString &url)
{
    _archiveUrl  = url;
    _archiveUUID = uuid;

   //          userinfo       host      port
   //           ┌──┴───┐ ┌──────┴──────┐ ┌┴┐
   //   https://john.doe@www.example.com:123/forum/questions/?tag=networking&order=newest#top
   //   └─┬─┘   └─────────────┬────────────┘└───────┬───────┘ └────────────┬────────────┘ └┬┘
   //   scheme          authority                  path                  query           fragment

   const string str = url.toStdString();
   const size_t found = str.find_first_of('/', 8); // make it work for http:// and https://
   if ( found != string::npos )
     _archiveSchemeAuthority = QString::fromStdString(str.substr(0, found+1));
   else
     _archiveSchemeAuthority = url;
}

QString RequestInterceptor::generateCobaltPrefix()
{
    QString url;
    globalSettings->cobaltSecure() ? url.append("https://") : url.append("http://");
    url.append(generateCobaltAuth());
    url.append("get/");
    return url;
}

QString RequestInterceptor::generateCobaltAuth()
{
    QString url;
    globalSettings->cobaltSameHost() ? url.append(globalSettings->kenIp()) : url.append(globalSettings->cobaltIp());
    url.append(":");
    url.append(QString::number(globalSettings->cobaltPort()));
    url.append("/");
    return url;
}
