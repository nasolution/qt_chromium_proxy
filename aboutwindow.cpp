#include "aboutwindow.h"
#include "logs.h"
#include "appsettings.h"
#include <QRect>
#include <QGuiApplication>
#include <QScreen>
#include <QFrame>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QDesktopServices>
#include <QCheckBox>

AboutWindow::AboutWindow(QWidget *parent): QMainWindow(parent)
{
    QWidget* widget = new QWidget(this);
    this->setCentralWidget(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    widget->setLayout(vLayout);

    QLabel *logoLabel = new QLabel("");
    QLabel *titleLabel = new QLabel( tr("KEN Viewer") );
    QLabel *copyrightLabel = new QLabel("Copyright 2010-2024 Aleph Archives. All Rights Reserved");
    copyrightLabel->setWordWrap(true);
    QCheckBox* cbAutoUpdate = new QCheckBox("Automatic Check for Updates");
    QImage img;
    if (!img.load(":/icons/AA_mark.png")) {
        log_error("cant load image");
    }
    logoLabel->setPixmap( QPixmap::fromImage(img.scaled(150, 125)) );
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(logoLabel);
    QVBoxLayout *cbUpdateLayout = new QVBoxLayout();
    cbUpdateLayout->setContentsMargins(0, 20, 12, 12);
    cbUpdateLayout->addWidget(cbAutoUpdate);
    cbUpdateLayout->addStretch();
    topLayout->addLayout(cbUpdateLayout);

    QPushButton *okButton = new QPushButton( tr("OK") );
    QPushButton *webButton = new QPushButton( tr("Website") );
    QPushButton *btnUpdate = new QPushButton( tr("Check for Updates") );
    btnUpdate->hide();


    QHBoxLayout* btnLayout = new QHBoxLayout();
    //btnLayout->addWidget(btnUpdate);
    btnLayout->addStretch();
    btnLayout->addWidget(webButton);
    btnLayout->addWidget(okButton);

    vLayout->addWidget(titleLabel);
    vLayout->addWidget(copyrightLabel);
    vLayout->addLayout(topLayout);
    vLayout->addLayout(btnLayout);

    this->resize(380, 200);

    cbAutoUpdate->setChecked(globalSettings->runAutoUpdate());
    connect(cbAutoUpdate, &QCheckBox::clicked, [=](bool checked) {
        emit onAutoUpdateRequested(checked);
    });

    connect(okButton, &QPushButton::clicked, [=]() {
        this->hide();
    });
    connect(webButton, &QPushButton::clicked, [=]() {
        QDesktopServices::openUrl(QUrl("https://ken-webarchiving.com/"));
    });
    connect(btnUpdate, &QPushButton::clicked, [=]() {
        this->hide();
        emit onUpdateRequested();
    });

    setWindowTitle( tr("About") );
}


void AboutWindow::showCenter()
{
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    x += width() / 4;
    int y = (screenGeometry.height() - height()) / 2;
    y += height() / 4;
    move(x, y);


    this->setWindowModality(Qt::WindowModal);
    this->setMaximumHeight(this->height());
    this->setMaximumWidth(this->width());

    this->show();
}
