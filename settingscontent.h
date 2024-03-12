#ifndef SETTINGSCONTENT_H
#define SETTINGSCONTENT_H

#include <QWidget>

namespace Ui {
class SettingsContent;
}

class SettingsContent : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsContent(QWidget *parent = nullptr);
    ~SettingsContent();
    void loadSettings();
    void saveSettings();
private slots:
    void on_pushButton_3_clicked();
    void on_cbDisableSimpleBrowserCache_clicked(bool checked);

signals:
    void onClearCacheRequested();
    void onViewPortChanged();
private:
    Ui::SettingsContent *ui;
};

#endif // SETTINGSCONTENT_H
