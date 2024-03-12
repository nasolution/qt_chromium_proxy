#ifndef WEBENGINEVIEW_H
#define WEBENGINEVIEW_H

#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QKeySequence>
#include <QWebEnginePage>

class QAction;
class WebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    WebEngineView(QWidget *parent = nullptr);
    WebEngineView(QWebEngineProfile* profile, QWidget *parent = nullptr);
    WebEngineView(bool isUtil, QWidget *parent = nullptr);
    void ignoreCertErrors(bool value);
    void setForKVWindow() { _forKVWindow = true; }
    static QString CtrlModifier();
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;
private:
    void setShortcuts();
    QAction *createAction(QKeySequence shortCut);
    QAction *reloadAction;
    QAction *backAction;
    QAction *forwardAction;
    QAction *homeAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *zoomReset;
    bool _ignoreCertErrors = false;
    bool _forKVWindow = false;
    bool _isUtil = false;
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
signals:
    void onDevToolsRequested();
    void onHomeRequested();
};

#endif // WEBENGINEVIEW_H
