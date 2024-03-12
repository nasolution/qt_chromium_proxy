#ifndef WEBPAGEEXPORTER_H
#define WEBPAGEEXPORTER_H

#include <QObject>
class QWebEnginePage;
class QWebEngineView;
class WebPageExporter : public QObject
{
    Q_OBJECT
public:
    explicit WebPageExporter(QObject *parent = nullptr);
    void exportToPdf(QWebEnginePage* page);
    void exportToPdf(QWebEnginePage* page, const QString& filePath);
    void exportToPng(QWebEnginePage *page);
    void exportToPng(QWebEnginePage *page, const QString& filePath);
    void setIsArchivesDebugger(bool val) { _isArchivesDebugger = val; }
private slots:
    void handlePDFExportFinished(const QString& filePath, bool success);
signals:
    void onPDFExportFinished(const QString& filePath, bool success);
    void onPNGExportFinished(const QString& filePath, bool success);
private:
    void prepareBeforeExport(QWebEnginePage* page);
    bool _pngMode = false;
    QString _pngExportPath;
    bool _finishedTriggered = false;
    bool _isArchivesDebugger = false;
};

#endif // WEBPAGEEXPORTER_H
