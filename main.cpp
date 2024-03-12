// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause


#include "appsettings.h"
#include "runguard.h"
#include "logs.h"
#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include "requestinterceptor.h"
#include "mainwindow.h"
#include <QApplication>
#include <QLoggingCategory>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QNetworkProxy>

#include "utils.h"
#include <QtWebEngineQuick/qtwebenginequickglobal.h>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtGui/QGuiApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QtCore/QLoggingCategory>
#include <QTranslator>
#include <QtWebEngineCore>



using namespace Qt::StringLiterals;

#define PROXY_HOST "127.0.0.1"
#define PROXY_PORT 8084
AppSettings* globalSettings;
QList<QString> locales;

/**
QUrl commandLineUrlArgument()
{
    const QStringList args = QCoreApplication::arguments();
    for (const QString &arg : args.mid(1)) {
        if (!arg.startsWith(u'-'))
            return QUrl::fromUserInput(arg);
    }
    return QUrl( QStringLiteral(ARCHIVE_URL) );
}






static QUrl startupUrl(const QCommandLineParser &parser)
{
    if (!parser.positionalArguments().isEmpty()) {
        const QUrl url = Utils::fromUserInput(parser.positionalArguments().constFirst());
        if (url.isValid())
            return url;
    }
    return QUrl(globalSettings->kenIp());
}


int nanoMain(int argc, char **argv)
{
    QCoreApplication::setApplicationName("KV Browser");
    QCoreApplication::setOrganizationName("KV");

    QtWebEngineQuick::initialize();

    QGuiApplication app(argc, argv);


    QLoggingCategory::setFilterRules(QStringLiteral("qt.webenginecontext.debug=true"));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "The URL to open.");
    parser.process(app);

    QQmlApplicationEngine appEngine;
    QUrl mainUrl = QUrl("qrc:/ApplicationRoot.qml");
    appEngine.load(mainUrl);
    if (appEngine.rootObjects().isEmpty())
        qFatal("Failed to load sources");

    const QUrl url(globalSettings->kenIp());
    QMetaObject::invokeMethod(appEngine.rootObjects().constFirst(),
                              "load", Q_ARG(QVariant, url));

    return app.exec();
}


**/


void generateInitialServerSettings()
{
    if (globalSettings->kenIp().length() < 1 || globalSettings->kenPort() < 1) {
        //lets make a new settings if its not set yet
        globalSettings->setKenIp("127.0.0.1");
        globalSettings->setKenPort(2356);
        globalSettings->setKenSecure(false);
        globalSettings->setCobaltIp("127.0.0.1");
        globalSettings->setCobaltPort(4567);
        globalSettings->setCobaltSameHost(true);
    }
    if (globalSettings->viewPortH() == 0 || globalSettings->viewPortW() == 0) {
        globalSettings->setViewPortW(1280);
        globalSettings->setViewPortH(720);
    }

    //Set default locale to en_us
    if (globalSettings->localeIndex() > -1)
        return;
    QList<QLocale> allLocales = QLocale::matchingLocales(
        QLocale::AnyLanguage,
        QLocale::AnyScript,
        QLocale::AnyCountry);
    for (int i = 0; i < allLocales.size(); ++i) {
        QLocale locale = allLocales[i];
        if (locale.country() == QLocale::UnitedStates && locale.language() == QLocale::English) {
            globalSettings->setLocaleIndex(i);
            break;
        }
    }
    for(const QLocale &locale : allLocales) {
        QString localeStr = QLocale::languageToString( locale.language() );
        localeStr.append( "/" );
        localeStr.append( QLocale::countryToString( locale.country() ) );
        localeStr.append( " (" );
        localeStr.append( QLocale::languageToCode( locale.language()) );
        localeStr.append("_");
        localeStr.append(QLocale::countryToCode(locale.country()));
        localeStr.append(")");
        //if (!locales.contains(localeStr)) {
        locales.push_back(localeStr);
        //}
    }
}

bool isKenValidUrl()
{
    QString kenIp = globalSettings->kenIp();
    return kenIp.startsWith("http");
}

int main(int argc, char **argv)
{

    RunGuard guard( "lkjfdslkj3294392" );
    if ( !guard.tryToRun() ) {
        log_error( "Another instance is running" );
        //return 0;
    }


    globalSettings = new AppSettings();
    globalSettings->load();
    generateInitialServerSettings();

    //bool kenIsUrl = isKenValidUrl();
    //if (kenIsUrl)
    //    return nanoMain(argc, argv);



    //qputenv("QT_FATAL_WARNINGS","1");
    QCoreApplication::setOrganizationName("KV Browser");
    QCoreApplication::setOrganizationName("KV");
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(u":AppLogoColor.png"_s));

    if (globalSettings->language() == "fr") {
        log_info("Setting app language to french!");
        QTranslator *translator = new QTranslator;
        bool succ = translator->load(":/lang_fr");
        if (succ) {
            app.installTranslator(translator);
        } else {
            log_error("Failed loading translation");
        }
    }

    if (globalSettings->enableLocale() && globalSettings->localeIndex() > -1) {
        QList<QLocale> allLocales = QLocale::matchingLocales(
            QLocale::AnyLanguage,
            QLocale::AnyScript,
            QLocale::AnyCountry);
        if (allLocales.count() >= globalSettings->localeIndex() ) {
            QLocale locale = allLocales.at(globalSettings->localeIndex() );
            QLocale::setDefault(locale);
            log_info(  "Setting Global Locale to " + QLocale::languageToString( locale.language() ) +
                     "_" + QLocale::countryToString( locale.country() ) );
        }
    }


#ifdef DEBUG_JAKE
    log_info(  "Debug Jake Enabled" );
#endif
#ifdef DEBUG_NO_INTERCEPT
    log_info(  "Debug No intercept Enabled" );
#endif
#ifdef DEBUG_SHOW_SUBWINDOW_ONLOAD
    log_info(  "Debug Sub Window Enabled" );
#endif

    MainWindow* win = new MainWindow();
    win->load();
    win->show();


    return app.exec();


}
