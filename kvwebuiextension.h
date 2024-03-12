#ifndef KVWEBUIEXTENSION_H
#define KVWEBUIEXTENSION_H

#include <QObject>
#include <QSettings>


class KvWebuiExtension : public QObject
{
    Q_OBJECT
public:
    explicit KvWebuiExtension(QObject *parent = 0);
public slots:
    void loadArchive(QString, QString, QString);
    void doRegistration();
    void finishedArchiveExportPreparation(QString);
    void bulkPDFExportArchive(QString, QString);
    void bulkPNGExportArchive(QString, QString);
    void bulkExportArchive(QString uuid, QString archiveName);
    QString getArchiveUrlOf(QString url);
signals:
    void finishedExportPreparation(QString);
    void onViewArchiveRequested(const QString& url, const QString& archiveId, const QString& jsonText);
public slots:
private:
    void bulkExportArchive(QString, QString, QString&);
    QSettings* settings;
};

#endif // KVWEBUIEXTENSION_H
