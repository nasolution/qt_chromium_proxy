#include "downloaddlg.h"
#include "../KVMaintenanceTool/updater.h"
#include "KVVersion.h"
#include "logs.h"
#include <QObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>
#include <QGroupBox>

DownloadDlg::DownloadDlg(QWidget *parent): QDialog(parent)
{
    this->setWindowTitle("Updates");
    this->resize(300, 200);
    this->setMaximumHeight(500);
    this->setMaximumWidth(350);
    this->setWindowModality(Qt::WindowModal);

    this->setLayout(new QVBoxLayout());
    QGroupBox* gb = new QGroupBox();
    gb->setTitle( tr("Software Update Status") );
    QVBoxLayout* layout = new QVBoxLayout();
    //this->setLayout(layout);
    gb->setLayout(layout);
    this->layout()->addWidget(gb);

    nfoLabel = new QLabel( tr("Checking for Updates...") );
    nfoLabel->setWordWrap(true);
    this->_iconLabel = new QLabel();


    QHBoxLayout* nfoLayout = new QHBoxLayout();
    nfoLayout->addWidget(nfoLabel);
    nfoLayout->addStretch();
    nfoLayout->addWidget(_iconLabel);
    nfoLayout->addSpacing(30);
    layout->addLayout(nfoLayout);


    btnNotes = new QPushButton( tr("Release Notes") );
    btnOk = new QPushButton( tr("Update Now") );
    btnCancel = new QPushButton( tr("Skip") );
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnNotes);
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);
    layout->addLayout(btnLayout);
    this->_updater = new Updater(this);

    _tb = new QTextEdit();
    layout->addWidget(_tb);
    _tb->hide();
    _tb->setReadOnly(true);


    connect(btnNotes, &QPushButton::clicked, this, &DownloadDlg::handleNotesRequested);
    connect(btnCancel, &QPushButton::clicked, [=]() {
        this->hide();
    });
    connect(btnOk, &QPushButton::clicked, [=]() {
        this->hide();
        emit onUpdateRequested();
    });
}

void DownloadDlg::checkUpdate()
{
    log_info("Checking for update");
    this->nfoLabel->setText( tr("Checking for updates...") );
    connect(_updater, &Updater::onCheckUpdateDone, this, &DownloadDlg::handleCheckUpdateDone);
    _updater->checkUpdate();
}





bool DownloadDlg::versionIsOutDated()
{
    return checkVersionOutdated(KV_VERSION_MAJOR, KV_VERSION_MINOR, KV_VERSION_BUILD,
                                 _updater->versionMajor(), _updater->versionMinor(),
                                 _updater->versionBuild() );
}

bool DownloadDlg::checkVersionOutdated(int vMaj, int vMin, int vBuild, int svMaj, int svMin, int svBuild)
{
    if (vMaj < svMaj) {
        return true;
    } else if(vMaj == svMaj) {
        if (vMin < svMin) {
            return true;
        } else {
            if (vMin == svMin) {
                if (vBuild < svBuild) {
                    return true;
                }
            }
        }
    }
    return false;
}

QString DownloadDlg::versionString()
{
    QString str = QString::number(KV_VERSION_MAJOR) + ".";
    str.append(QString::number(KV_VERSION_MINOR) + ".");
    str.append(QString::number(KV_VERSION_BUILD));
    return str;
}

void DownloadDlg::handleCheckUpdateDone()
{
    if (versionIsOutDated()) {
        this->_isUpdated = false;
        QString str = tr("Application is not up to date\r\n\r\n");
        str.append( tr("Current Version: ") + this->versionString() + "\r\n\r\n");
        str.append( tr("Latest Version: ") + _updater->versionStr() );
        _tb->setText(_updater->releaseNotes());
        nfoLabel->setText( str );
        emit onUpdateAvailable();
    } else {
        this->_isUpdated = true;
        QString str = tr("Application is up to date!\r\nYou're using the latest version: ");
        str.append(_updater->versionStr());
        nfoLabel->setText( str );
        btnOk->hide();
        btnCancel->setText("Ok");
        btnNotes->hide();
    }
    this->applyUpdateStatusLogo();
}

void DownloadDlg::handleNotesRequested()
{
    this->setFixedWidth(UPDATE_NOTES_WINDOW_WIDTH);
    this->setFixedHeight(UPDATE_NOTES_WINDOW_HEIGHT);
    _tb->show();
}

void DownloadDlg::hideNotes()
{
    this->setFixedHeight(200);
    this->setFixedWidth(400);
    _tb->hide();
}

void DownloadDlg::applyUpdateStatusLogo()
{
    QImage img;
    if (_isUpdated) {
        img.load(":/icons/version_updated.webp");
    } else {
        img.load(":/icons/version_outdated.png");
    }
    _iconLabel->setPixmap( QPixmap::fromImage(img.scaled(80,80)) );
}

/**

{
    "arch": "x64",
    "exec": "kv.exe",
    "link": "/dl/kv",
    "md5": "768f5ad69ed980b283c8c3c99049355d",
    "name": "kv",
    "notes": "/notes/kv",
    "os": "windows",
    "version": "3.8.0"
}


**/
