#ifndef DOWNLOADDLG_H
#define DOWNLOADDLG_H

#include <QDialog>
class QLabel;
class Updater;
class QTextEdit;
class DownloadDlg : public QDialog
{
    Q_OBJECT
public:
    DownloadDlg(QWidget* parent = nullptr);
    void checkUpdate();
    void hideNotes();
signals:
    void onUpdateAvailable();
    void onUpdateRequested();
private slots:
    void handleCheckUpdateDone();
    void handleNotesRequested();
private:
    void applyUpdateStatusLogo();

    QLabel *nfoLabel;
    Updater* _updater;
    bool versionIsOutDated();
    bool checkVersionOutdated(int vMaj, int vMin, int vBuild, int svMaj, int svMin, int svBuild);
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QPushButton *btnNotes;
    QLabel *_iconLabel;
    QString versionString();
    QTextEdit *_tb;
    bool _isUpdated = true;
};

#endif // DOWNLOADDLG_H
