#include "settingsserver.h"
#include "ui_settingsserver.h"
#include "settingsjson.h"
#include "appsettings.h"
#include <QFileDialog>
#include <QtGui>

//

SettingsServer::SettingsServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsServer)
{
    ui->setupUi(this);
    ui->formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    ui->formLayout_2->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    ui->labelSecure->setText("");
    ui->labelUseKen->setText("");
    loadSettings();
}

SettingsServer::~SettingsServer()
{
    delete ui;
}

void SettingsServer::saveSettings()
{
    QString kenIp = ui->lineKenIP->text();
    kenIp = kenIp.replace(" ", "");
    QString kenPort = ui->lineKenPort->text();
    kenPort = kenPort.replace(" ", "");

    QString cobaltIp = ui->lineCobaltIP->text();
    cobaltIp = cobaltIp.replace(" ", "");
    QString cobaltPort = ui->lineCobaltPort->text();
    cobaltPort = cobaltPort.replace(" ", "");

    globalSettings->setKenIp(kenIp);
    globalSettings->setKenPort(kenPort.toInt());
    globalSettings->setCobaltIp(cobaltIp);
    globalSettings->setCobaltPort(cobaltPort.toInt());
    globalSettings->setKenSecure(ui->cbKenHttps->isChecked());
    globalSettings->setCobaltSameHost(ui->cbCobaltUseKen->isChecked());
    globalSettings->setCobaltSecure(ui->cbCobaltSecure->isChecked() );
}

void SettingsServer::on_buttonSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("JSON (*.json)"));
    globalSettings->exportSettings(fileName);
}


void SettingsServer::on_buttonImport_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Save File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("JSON (*.json)"));
    globalSettings->importSettings(fileName);
    emit onImported();
}


void SettingsServer::on_SettingsServer_destroyed()
{

}

void SettingsServer::loadSettings()
{
    ui->lineKenIP->setText(globalSettings->kenIp());
    ui->lineKenPort->setText( QString::number( globalSettings->kenPort() ) );
    ui->lineCobaltIP->setText(globalSettings->cobaltIp() );
    ui->lineCobaltPort->setText( QString::number(globalSettings->cobaltPort()) );
    ui->cbCobaltSecure->setChecked( globalSettings->cobaltSecure() );
    ui->cbCobaltUseKen->setChecked( globalSettings->cobaltSameHost() );
    ui->cbKenHttps->setChecked(globalSettings->kenSecure());
    if (ui->cbCobaltUseKen->isChecked()) {
        ui->lineCobaltIP->setEnabled(false);
    }
}


void SettingsServer::on_cbCobaltUseKen_clicked(bool checked)
{
    if ( checked ) {
        ui->lineCobaltIP->setText( ui->lineKenIP->text() );
        ui->lineCobaltIP->setEnabled(false);
    } else {
        ui->lineCobaltIP->setEnabled(true);
    }
}


void SettingsServer::on_lineKenIP_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if ( ui->cbCobaltUseKen->isChecked() ) {
        ui->lineCobaltIP->setText(ui->lineKenIP->text() );
    }
}

