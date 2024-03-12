#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QMainWindow>

class AboutWindow: public QMainWindow
{
    Q_OBJECT
public:
    AboutWindow(QWidget* parent = nullptr);
    void showCenter();
signals:
    void onUpdateRequested();
    void onAutoUpdateRequested(bool autoUpdate);
};

#endif // ABOUTWINDOW_H
