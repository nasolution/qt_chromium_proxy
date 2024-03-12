#include "appsettings.h"
#include "KVVersion.h"

AppSettings::AppSettings(QObject *parent)
    : QSettings{parent}
{

}

void AppSettings::save()
{
#ifdef WIN32
    exportSettings("appSettings.json");
#endif
}

void AppSettings::load()
{
#ifdef WIN32
    importSettings("appSettings.json");
    return;
#endif
    _kenIp = value("kenIp").toString();
    _kenPort = value("kenPort").toInt();
    _kenSecure = value("kenSecure").toBool();
    _cobaltIp = value("cobaltIp").toString();
    _cobaltPort = value("cobaltPort").toInt();
    _cobaltSecure = value("cobaltSecure").toBool();
    _cobaltSameHost = value("cobaltSameHost").toBool();
    _enableDebug = value("enableDebug").toBool();
    _enableHarmonizer = value("enableHarmonizer").toBool();
    _enhancedPDF = value("enhancedPDF").toBool();
    _pdfTempDir = value("pdfTempDir").toString();
    _viewPortH = value("viewPortH").toInt();
    _viewPortW = value("viewPortW").toInt();
    _localeIndex = value("localeIndex").toInt();
    _useCustomUA = value("useCustomUA").toBool();
    _customUA = value("customUA").toString();
    _enableLocale = value("enableLocale").toBool();
    _disableSimpleBrowserCache = value("disableSimpleBrowserCache").toBool();
    _language = value("language").toString();
}

void AppSettings::exportSettings(const QString &path)
{
    SettingsJSON json;
    json.insert("kenIp", _kenIp);
    json.insert("kenPort", _kenPort);
    json.insert("kenSecure", _kenSecure);
    json.insert("cobaltIp", _cobaltIp);
    json.insert("cobaltPort", _cobaltPort);
    json.insert("cobaltSecure", _cobaltSecure);
    json.insert("cobaltSameHost", _cobaltSameHost);
    json.insert("enableDebug", _enableDebug);
    json.insert("enableHarmonizer", _enableHarmonizer);
    json.insert("enhancedPDF", _enhancedPDF);
    json.insert("pdfTempDir", _pdfTempDir);
    json.insert("viewPortH", _viewPortH);
    json.insert("viewPortW", _viewPortW);
    json.insert("localeIndex", _localeIndex);
    json.insert("useCustomUA", _useCustomUA);
    json.insert("customUA", _customUA);
    json.insert("enableLocale", _enableLocale);
    json.insert("disableSimpleBrowserCache", _disableSimpleBrowserCache);
    json.insert("language", _language);
    json.exportTo(path, false);
}

void AppSettings::importSettings(const QString &path)
{
    SettingsJSON json;
    json.importFrom(path, false);
    _kenIp = json.getValue("kenIp").toString();
    _kenPort = json.getValue("kenPort").toInt();
    _kenSecure = json.getValue("kenSecure").toBool();
    _cobaltIp = json.getValue("cobaltIp").toString();
    _cobaltPort = json.getValue("cobaltPort").toInt();
    _cobaltSecure = json.getValue("cobaltSecure").toBool();
    _cobaltSameHost = json.getValue("cobaltSameHost").toBool();
    _enableDebug = json.getValue("enableDebug").toBool();
    _enableHarmonizer = json.getValue("enableHarmonizer").toBool();
    _enhancedPDF = json.getValue("enhancedPDF").toBool();
    _pdfTempDir = json.getValue("pdfTempDir").toString();
    _viewPortH = json.getValue("viewPortH").toInt();
    _viewPortW = json.getValue("viewPortW").toInt();
    _localeIndex = json.getValue("localeIndex").toInt();
    _useCustomUA = json.getValue("useCustomUA").toBool();
    _customUA = json.getValue("customUA").toString();
    _enableLocale = json.getValue("enableLocale").toBool();
    _disableSimpleBrowserCache = json.getValue("disableSimpleBrowserCache").toBool();
    _language = json.getValue("language").toString();
}
