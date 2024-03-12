#include "settingscontent.h"
#include "ui_settingscontent.h"
#include "appsettings.h"
#include <QFileDialog>
#include <QDesktopServices>
#include "logs.h"

SettingsContent::SettingsContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsContent)
{
    ui->setupUi(this);
    ui->gb_logs->hide();
    ui->groupBox_3->hide();
    //ui->cbEnhanceExport->hide();
    QList<QLocale> allLocales = QLocale::matchingLocales(
        QLocale::AnyLanguage,
        QLocale::AnyScript,
        QLocale::AnyCountry);
    QList<QString> locales;
    for(const QLocale &locale : allLocales) {
        QString localeStr = QLocale::languageToString( locale.language() );
        localeStr.append( "/" );
        localeStr.append( QLocale::countryToString( locale.country() ) );
        localeStr.append( " (" );
        localeStr.append( QLocale::languageToCode( locale.language()) );
        localeStr.append("_");
        localeStr.append(QLocale::countryToCode(locale.country()));
        localeStr.append(")");
        //if (!locales.contains(localeStr)) {
            locales.push_back(localeStr);
        //}
    }
    //std::sort(locales.begin(), locales.end());
    for(const QString &locale : locales) {
        ui->comboLocale->addItem(locale);
    }

    loadSettings();

}

SettingsContent::~SettingsContent()
{
    delete ui;
}

void SettingsContent::loadSettings()
{
    ui->cbEnableDebug->setChecked(globalSettings->enableDebug());
    ui->cbEnableHarmonizer->setChecked(globalSettings->enableHarmonizer());
    ui->spinWidth->setValue(globalSettings->viewPortW());
    ui->spinHeight->setValue(globalSettings->viewPortH());
    ui->comboLocale->setCurrentIndex( globalSettings->localeIndex() );
    ui->cbUseCustomUA->setChecked( globalSettings->useCustomUA() );
    ui->lineCustomUA->setText( globalSettings->customUA() );
    ui->cbEnableLocale->setChecked( globalSettings->enableLocale() );
    //ui->cbDisableSimpleBrowserCache->setChecked( globalSettings->disableSimpleBrowserCache() );
    ui->cbEnhanceExport->setChecked(globalSettings->enhancedPDF());
}

void SettingsContent::saveSettings()
{
    int oldVPH = globalSettings->viewPortH();
    int oldVPW = globalSettings->viewPortW();
    int oldLocaleIndex = globalSettings->localeIndex();
    globalSettings->setEnableDebug(ui->cbEnableDebug->isChecked());
    globalSettings->setEnableHarmonizer(ui->cbEnableHarmonizer->isChecked());
    globalSettings->setViewPortH(ui->spinHeight->value());
    globalSettings->setViewPortW(ui->spinWidth->value());
    globalSettings->setLocaleIndex( ui->comboLocale->currentIndex() );
    globalSettings->setUseCustomUA( ui->cbUseCustomUA->isChecked() );
    globalSettings->setCustomUA( ui->lineCustomUA->text() );
    globalSettings->setEnableLocale( ui->cbEnableLocale->isChecked() );
    //globalSettings->setDisableSimpleBrowserCache( ui->cbDisableSimpleBrowserCache->isChecked() );
    globalSettings->setEnhancedPDF( ui->cbEnhanceExport->isChecked() );
    if (oldLocaleIndex != globalSettings->localeIndex()) {
        if (globalSettings->localeIndex() > -1) {
            QList<QLocale> allLocales = QLocale::matchingLocales(
                QLocale::AnyLanguage,
                QLocale::AnyScript,
                QLocale::AnyCountry);
            if (allLocales.count() >= globalSettings->localeIndex() ) {
                QLocale locale = allLocales.at(globalSettings->localeIndex() );
                QLocale::setDefault(locale);
                log_info(  "Setting Global Locale to " + QLocale::languageToString( locale.language() ) +
                         "_" + QLocale::countryToString( locale.country() ) );
            }
        }
    }
    if (globalSettings->viewPortH() != oldVPH || globalSettings->viewPortW() != oldVPW)
        emit onViewPortChanged();
}





void SettingsContent::on_pushButton_3_clicked()
{
    emit onClearCacheRequested();
}


void SettingsContent::on_cbDisableSimpleBrowserCache_clicked(bool checked)
{
    if (checked) {
        ui->pushButton_3->setEnabled(false);
    } else {
        ui->pushButton_3->setEnabled(true);
    }
}

