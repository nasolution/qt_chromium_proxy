#include "kvwebuiextension.h"
//#include "arcviewdlg.h"
#include "logs.h"
#include <QDebug>
#include <QDesktopServices>
//#include "mainwindow.h"
//#include "bulkpdfexport.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

//#include "mylog.h"

KvWebuiExtension::KvWebuiExtension(QObject *parent) :
    QObject(parent)
{
}

void KvWebuiExtension::loadArchive(QString archiveId, QString url, QString jsonData)
{
    log_info("::: loadArchive: " + archiveId + " | " + url + " | " + jsonData);

    //QMessageBox::information(nullptr, "loadArchive", "archiveId: " + archiveId);
    emit onViewArchiveRequested(url, archiveId, jsonData);
    //L_INFO("loadArchive: {} {} {}", archiveId, url, jsonData);
    /**
    ArcViewDlg *w = new ArcViewDlg(
        settings.value("cobalt_host_ip","127.0.0.1").toString(),
        settings.value("cobalt_host_port",4567).toInt()
        );
    w->show();
    w->load(archiveId, QUrl::fromUserInput(url), jsonData);
    **/
}

void KvWebuiExtension::doRegistration()
{
    // TODO

    //QDesktopServices::openUrl(static_cast<MainWindow*>(parent())->getWebUIRoot()+"activate");
}

void KvWebuiExtension::finishedArchiveExportPreparation(QString continuation)
{
    qDebug()<< "Finished preparation will send this continuation :" << continuation;
    emit finishedExportPreparation(continuation);
}

void KvWebuiExtension::bulkPDFExportArchive(QString uuid, QString archiveName){
    QString type("pdf");
    bulkExportArchive(uuid, archiveName, type);
}

void KvWebuiExtension::bulkPNGExportArchive(QString uuid, QString archiveName){
    QString type("png");
    bulkExportArchive(uuid, archiveName, type);
}

void KvWebuiExtension::bulkExportArchive(QString uuid, QString archiveName) {
    QString type("pdf");
    bulkExportArchive(uuid,archiveName, type);
}

QString KvWebuiExtension::getArchiveUrlOf(QString url)
{
    Q_UNUSED(url);
    // TODO
    return "";

    //return static_cast<ArcViewDlg*>(parent())->nam->getArchiveUrl(QUrl(url)).toString(QUrl::None);
}

void KvWebuiExtension::bulkExportArchive(QString uuid, QString archiveName, QString& type)
{
    Q_UNUSED(uuid);
    Q_UNUSED(archiveName);
    Q_UNUSED(type);
    // TODO

    //MainWindow *w=static_cast<MainWindow*>(parent());

    //QString dirName(QFileDialog::getExistingDirectory(w, "Select temporary location for enhanced pdf working directory",QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)));
    //qDebug() << "Will export to " << dirName;
    //if (dirName.isEmpty()) return;
    //dirName.append("/");
    //dirName.append(uuid);
    //qDebug() << "Will export to " << dirName;

    //BulkPdfExport::exportType exType=(type=="png")?BulkPdfExport::EXPORT_PNG:BulkPdfExport::EXPORT_PDF;

    //BulkPdfExport *dlg=new BulkPdfExport(static_cast<QWidget*>(parent()),
    //                                     exType,
    //                                     dirName,
    //                                     uuid,
    //                                     archiveName,
    //                                     settings.value("cobalt_host_ip","127.0.0.1").toString(),
    //                                     settings.value("cobalt_host_port",4567).toInt(),
    //                                     w->getMainWindowNAM()
    //                                     );
    //dlg->show();
    //dlg->startExport();
}
