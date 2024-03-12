#ifndef SETTINGSSERVER_H
#define SETTINGSSERVER_H

#include <QWidget>

namespace Ui {
class SettingsServer;
}

class SettingsServer : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsServer(QWidget *parent = nullptr);
    ~SettingsServer();
    void saveSettings();
    void loadSettings();
private slots:
    void on_buttonSave_clicked();

    void on_buttonImport_clicked();

    void on_SettingsServer_destroyed();

    void on_cbCobaltUseKen_clicked(bool checked);
    void on_lineKenIP_textChanged(const QString &arg1);

signals:
    void onImported();
private:
    Ui::SettingsServer *ui;
};

#endif // SETTINGSSERVER_H
