#ifndef SETTINGSJSON_H
#define SETTINGSJSON_H

#include <QObject>
#include <QVariantList>
#include <QJsonObject>
#include <QSettings>

class SettingsJSON;

class SettingsJSON : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList  data    READ data     WRITE setData     NOTIFY dataChanged)
    Q_PROPERTY(bool          result  READ result   WRITE setResult   NOTIFY resultChanged)
    Q_PROPERTY(int           length  READ length   WRITE setLength   NOTIFY lengthChanged)
public:

        SettingsJSON();

        QVariantList data () const;
        bool result       () const;
        int  length       () const;
        bool fileExists(QString path);
        Q_INVOKABLE void importFrom(QString path, bool fromQml = true);
        Q_INVOKABLE void save();
        Q_INVOKABLE void exportTo(QString path, bool fromQml = true);
        Q_INVOKABLE void load();

        //SLOTS
    public slots:
        void setData(const QVariantList& data);
        void setResult(bool result);
        void setLength(int length);
        void insert(QString key, QVariant value);
        QVariant getValue(QString key);

        //SIGNALS
    signals:
        void dataChanged(const QVariantList& data);
        void resultChanged(bool result);
        void lengthChanged(int length);

    private:
        QVariantList  m_data;
        bool          m_result = {false};
        int           m_length = {0};
        QJsonObject json;
};



#endif // SETTINGSJSON_H
