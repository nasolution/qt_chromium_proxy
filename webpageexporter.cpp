#include "webpageexporter.h"
#include <QWebEnginePage>
#include "appsettings.h"
#include "logs.h"
#include <QGraphicsScene>
#include <QWebEngineView>
#include <QGraphicsView>
#include <QPainter>
#include <QPdfDocument>
#include <QDir>
#include <QSizeF>
#include <QRect>
#include <QImage>
#include <QPixmap>

WebPageExporter::WebPageExporter(QObject *parent)
    : QObject{parent}
{

}

QString generateTempPDFPath()
{
    QString tempPath = QDir::currentPath();
    tempPath.append(QDir::separator());
    tempPath.append("temp.pdf");
    return tempPath;
}

void WebPageExporter::exportToPdf(QWebEnginePage *page)
{
    QString filePath = globalSettings->pdfTempDir();
    filePath.append(QDir::separator() + page->title());
    filePath.append(".pdf");
    exportToPdf(page, filePath);
}

void WebPageExporter::exportToPdf(QWebEnginePage *page, const QString &filePath)
{
    connect(page, &QWebEnginePage::pdfPrintingFinished,
            this, &WebPageExporter::handlePDFExportFinished);
    prepareBeforeExport(page);
    page->printToPdf(filePath);
}

void WebPageExporter::exportToPng(QWebEnginePage *page)
{
    QString filePath = globalSettings->pdfTempDir();
    filePath.append(QDir::separator() + page->title());
    filePath.append(".png");

    exportToPng(page, filePath);
}

void WebPageExporter::exportToPng(QWebEnginePage *page, const QString &filePath)
{
    _pngMode = true;
    _pngExportPath = filePath;
    QString tempPath = generateTempPDFPath();
    connect(page, &QWebEnginePage::pdfPrintingFinished,
            this, &WebPageExporter::handlePDFExportFinished);
    page->printToPdf(tempPath);
}



void WebPageExporter::handlePDFExportFinished(const QString &filePath, bool success)
{
    if (_finishedTriggered) return;
    _finishedTriggered = true;

    if (false == _pngMode) { // PDF
        emit onPDFExportFinished(filePath, success);
        return;
    }

    // PNG
    if (false == success) {
        emit onPNGExportFinished(filePath, false);
        return;
    }

    QPdfDocument doc;
    doc.load(filePath);

    if (doc.pageCount() <= 0) { // empty PDF, not usable to generate a PNG
        _finishedTriggered = false;
        return;
    }
    
    QList<QImage> pageImages;
    int finalH = 0;
    int width  = 0;
    for (int i = 0; i < doc.pageCount(); ++i) {
        QSizeF pSize = doc.pagePointSize(i);
        QSize iSize(pSize.width(), pSize.height());
        QImage img = doc.render(i, iSize);
        pageImages.push_back(img);
        finalH += pSize.height();
        if (pSize.width() > width)
            width = pSize.width();
    }

    QImage finalImage(width, finalH, QImage::Format_ARGB32);
    QPainter painter(&finalImage);
    for (int i = 0, yNow = 0; i < pageImages.count(); ++i) {
        QImage imgNow = pageImages[i];
        painter.drawImage(0, yNow, imgNow);
        yNow += imgNow.height();
    }

    bool isSaved = finalImage.save(_pngExportPath);
    emit onPNGExportFinished(_pngExportPath, isSaved);
}

void WebPageExporter::prepareBeforeExport(QWebEnginePage *page)
{
    if (globalSettings->enhancedPDF() && !_isArchivesDebugger) {
        log_info(  "Enhancing before Export" );
        const QString scrollJS = "window.scrollTo(0, document.body.scrollHeight);";
        page->runJavaScript(scrollJS);
    }
    /**
    QWebElement document=page->mainFrame()->documentElement();

    // Save the current viewport size to restore it later
    pageViewportSizeBeforeExport=page->viewportSize();

    // check if we have any nasty horizontal scrollbar
    QSizeF contentSize,
        viewPortSize(pageViewportSizeBeforeExport);

    QVariant diff;



    qDebug() << "Viewport size before export " << viewPortSize;

    for (int i=0;i<10;i++) {
        contentSize=page->mainFrame()->contentsSize();
        diff=document.evaluateJavaScript("document.body.scrollWidth-document.body.clientWidth;");
        qDebug() << "Before resize" << document.evaluateJavaScript("document.body.scrollWidth-document.body.clientWidth;");
        if (diff.toInt()==0)  break;
        viewPortSize.setWidth(contentSize.width()+diff.toInt()+30);
        //page->setViewportSize(viewPortSize);
    }
    //diff = document.evaluateJavaScript("document.querySelector('section.active div.content-holder').offsetHeight;");
    //viewPortSize.setHeight(diff.toInt());
    //page->setViewportSize(viewPortSize);

    qDebug() << "Export viewport size is " << viewPortSize;
    QFile f(":/html/html/cleanstat.js");
    f.open(QIODevice::ReadOnly);
    document.evaluateJavaScript(f.readAll()+"('"+cont+"','"+archiveName.replace('\'',' ')+"');");
    f.close();
    **/
}


