#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>
#include "settingsjson.h"

class AppSettings : public QSettings
{
public:
    explicit AppSettings(QObject *parent = nullptr);
    const QString& kenIp() { return _kenIp; }
    int kenPort() { return _kenPort; }
    bool kenSecure() { return _kenSecure; }
    const QString& cobaltIp() { return _cobaltIp; }
    int cobaltPort() { return _cobaltPort; }
    bool cobaltSecure() { return _cobaltSecure; }
    bool cobaltSameHost() { return _cobaltSameHost; }
    bool enableDebug() { return _enableDebug; }
    bool enableHarmonizer() { return _enableHarmonizer; }
    bool enhancedPDF() { return _enhancedPDF; }
    const QString& pdfTempDir() { return _pdfTempDir; }
    int viewPortH() { return _viewPortH; }
    int viewPortW() { return _viewPortW; }
    int localeIndex() { return _localeIndex; }
    bool enableLocale() { return _enableLocale; }
    bool useCustomUA() { return _useCustomUA; }
    QString customUA() { return _customUA; }
    bool disableSimpleBrowserCache() { return _disableSimpleBrowserCache; }
    QString language() { return _language; }
    bool runAutoUpdate() { return _runAutoUpdate; }

    void setKenIp(const QString& value) { _kenIp = value; setValue("kenIp", value); }
    void setKenPort(int value) { _kenPort = value; setValue("kenPort", value); }
    void setKenSecure(bool value) { _kenSecure = value; setValue("kenSecure", value); }
    void setCobaltIp(const QString& value) { _cobaltIp = value; setValue("cobaltIp", value); }
    void setCobaltPort(int value) { _cobaltPort = value; setValue("cobaltPort", value); }
    void setCobaltSecure(bool value) { _cobaltSecure = value; setValue("cobaltSecure", value); }
    void setCobaltSameHost(bool value) { _cobaltSameHost = value; setValue("cobaltSameHost", value); }
    void setEnableDebug(bool value) { _enableDebug = value; setValue("enableDebug", value); }
    void setEnableHarmonizer(bool value) { _enableHarmonizer = value; setValue("enableHarmonizer", value); }
    void setEnhancedPDF(bool value) { _enhancedPDF = value; setValue("enhancedPDF", value); }
    void setPDFTempDir(const QString&value) { _pdfTempDir = value; setValue("pdfTempDir", value); }
    void setViewPortH(int value) { _viewPortH = value; setValue("viewPortH", value); }
    void setViewPortW(int value) { _viewPortW = value; setValue("viewPortW", value); }
    void setLocaleIndex(int value) { _localeIndex = value; setValue("localeIndex", value); }
    void setUseCustomUA(bool value) { _useCustomUA = value; setValue("useCustomUA", value); }
    void setCustomUA(QString value) { _customUA = value; setValue("customUA", value); }
    void setEnableLocale(bool value) { _enableLocale = value; setValue("enableLocale", value); }
    void setDisableSimpleBrowserCache(bool value) { _disableSimpleBrowserCache = value; setValue("disableSimpleBrowserCache", value); }
    void setLanguage(const QString& value) { _language = value; setValue("language", value); }
    void setRunAutoUpdate(bool value) { _runAutoUpdate = value; setValue("runAutoUpdate", value); }

    void save();
    void load();
    void exportSettings(const QString& path);
    void importSettings(const QString& path);
private:
    QString _kenIp;
    int _kenPort;
    bool _kenSecure;
    QString _cobaltIp;
    int _cobaltPort;
    bool _cobaltSecure;
    bool _cobaltSameHost;
    bool _enableDebug;
    bool _enableHarmonizer;
    bool _enhancedPDF;
    QString _pdfTempDir;
    int _viewPortH = 0;
    int _viewPortW = 0;
    bool _enableLocale = false;
    int _localeIndex = -1;
    bool _useCustomUA = false;
    QString _customUA;
    bool _disableSimpleBrowserCache = false;
    QString _language = "en";
    bool _runAutoUpdate = false;
    //
};

extern AppSettings* globalSettings;
extern QList<QString> locales;


#endif // APPSETTINGS_H
