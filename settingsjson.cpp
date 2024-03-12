#include "settingsjson.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVariantList>
#include <QCoreApplication>

SettingsJSON::SettingsJSON()
{

}



bool SettingsJSON::fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes, Is it really a file!
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

QVariantList SettingsJSON::data() const
{
    return m_data;
}

void SettingsJSON::setData(const QVariantList& data)
{
    if (m_data == data)
        return;
    m_data = data;
    emit dataChanged(m_data);
}

int SettingsJSON::length() const
{
    return m_length;
}

void SettingsJSON::setLength(int length)
{
    if (m_length == length)
        return;
    m_length = length;
    emit lengthChanged(m_length);
}

void SettingsJSON::insert(QString key, QVariant value)
{
    QJsonValue val = QJsonValue::fromVariant(value);
    json.insert(key, val);
}

QVariant SettingsJSON::getValue(QString key)
{
    QVariant ret = json.value(key).toVariant();
    return ret;
}

bool SettingsJSON::result() const
{
    return m_result;
}

void SettingsJSON::setResult(bool result)
{
    if (m_result == result)
        return;

    m_result = result;
    emit resultChanged(m_result);
}

void SettingsJSON::importFrom(QString path, bool fromQml) {

    QString rawData;
    QFile file;
    QDir dir(".");

    if (fromQml) {
        QUrl pathUrl(path);
        path = pathUrl.toLocalFile(); //Have to do this as qml filedialog is in url form(adds file://) which qfile dont recognize
    }
    if(fileExists(path)) {
        {
            file.setFileName(path);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            rawData = file.readAll();
            file.close();
            QJsonDocument document   =   { QJsonDocument::fromJson(rawData.toUtf8()) };
            QJsonObject jsonObject = document.object();
            foreach (const QString& key, jsonObject.keys() ) {
                QJsonValue val = jsonObject.value(key);
                insert(key, val.toVariant() );
            }
            path = "";
        }

    } else {
        qWarning() << "There is no any file in this path!";
    }
}

void SettingsJSON::exportTo(QString path, bool fromQml)
{
    QJsonDocument document(json);
    if (fromQml) {
        QUrl pathUrl(path);
        path = pathUrl.toLocalFile(); //Have to do this as qml filedialog is in url form(adds file://) which qfile dont recognize
    }
    QFile jsonFile(path);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}


void SettingsJSON::save()
{
    QString path = QCoreApplication::applicationDirPath();
    path = path + "settings.json";
    exportTo(path, false);
}

void SettingsJSON::load()
{
    QString path = QCoreApplication::applicationDirPath();
    path = path + "settings.json";
    importFrom(path);
}
