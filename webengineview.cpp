#include "webengineview.h"
#include "mainwindow.h"
#include "logs.h"
#include "KVVersion.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include <QWebEngineCertificateError>

WebEngineView::WebEngineView(QWidget *parent): QWebEngineView(parent)
{
    setShortcuts();
}

WebEngineView::WebEngineView(QWebEngineProfile *profile, QWidget *parent)
    : QWebEngineView(profile, parent)
{
    //only kvwindow will instantiate with profile for now, so lets set _forKVWindow here
    _forKVWindow = true;
    setShortcuts();
}

WebEngineView::WebEngineView(bool isUtil, QWidget *parent)
    : QWebEngineView(parent), _isUtil(isUtil)
{
    //dont show menus for util(dev tools/view page source) window
    if (!isUtil)
        setShortcuts();
}

void WebEngineView::ignoreCertErrors(bool value)
{
    _ignoreCertErrors = value;
    if (!value) return;
    connect(this->page(), &QWebEnginePage::certificateError, [=](QWebEngineCertificateError error) {
        error.acceptCertificate();
    });
}

QString WebEngineView::CtrlModifier()
{
#ifdef Q_OS_DARWIN
    return "CMD";
#else
    return "CTRL";
#endif
}

QWebEngineView *WebEngineView::createWindow(QWebEnginePage::WebWindowType type)
{
    //for view source
    if ( type == QWebEnginePage::WebBrowserTab) {
        MainWindow* mainWindow = new MainWindow(true, nullptr);
        mainWindow->show();
        QWebEngineView* view = mainWindow->webEngineView();
        QWebEnginePage* page = new QWebEnginePage(this->page()->profile(), this);
        view->setPage(page);
        return mainWindow->webEngineView();
    }
    return nullptr;
}


void WebEngineView::setShortcuts()
{
    //Keyboard shortcuts
    reloadAction = createAction(QKeySequence(Qt::CTRL | Qt::Key_R));
    connect(reloadAction, &QAction::triggered, [=] {
        reload();
    });

    backAction = createAction( QKeySequence( Qt::CTRL | Qt::Key_Left ) );
    connect(backAction, &QAction::triggered, [=]() {
        this->back();
    });

    forwardAction = createAction( QKeySequence( Qt::CTRL | Qt::Key_Right) );
    connect(forwardAction, &QAction::triggered, [=]() {
        this->forward();
    });

    homeAction = createAction( QKeySequence(Qt::CTRL  | Qt::Key_H) );
    connect(homeAction, &QAction::triggered, [=]() {
        emit onHomeRequested();
    });

    QAction *devToolsAction = createAction( QKeySequence(Qt::CTRL | Qt::Key_D) );
    connect(devToolsAction, &QAction::triggered, [=]() {
       emit onDevToolsRequested();
    });

    QAction* viewSourceAction = createAction( QKeySequence(Qt::CTRL | Qt::Key_S) );
    connect(viewSourceAction, &QAction::triggered, [=]() {
        this->triggerPageAction(QWebEnginePage::ViewSource);
    });

    if (_forKVWindow || !_isUtil) {
        zoomInAction = createAction( QKeySequence(Qt::CTRL | Qt::Key_Plus) );
        connect(zoomInAction, &QAction::triggered, [=]() {
            page()->setZoomFactor( zoomFactor() + (zoomFactor() * 0.1) );
            log_info("Zooming in");
        });

        zoomOutAction = createAction (QKeySequence(Qt::CTRL | Qt::Key_Minus) );
        connect(zoomOutAction, &QAction::triggered, [=]() {
            page()->setZoomFactor( zoomFactor() - (zoomFactor() * 0.1) );
            log_info("Zooming out");
        });

        zoomReset = createAction (QKeySequence(Qt::CTRL | Qt::Key_0) );
        connect(zoomReset, &QAction::triggered, [=]() {
            page()->setZoomFactor(1.0);
            log_info("Zoom reset");
        });

    }
}

QAction *WebEngineView::createAction(QKeySequence shortCut)
{
    QAction* action = new QAction();
    action->setShortcut(shortCut);
    addAction(action);
    return action;
}


void WebEngineView::contextMenuEvent(QContextMenuEvent *event)
{
    if (_isUtil)
        return;

    QMenu *menu = createStandardContextMenu();
    const QList<QAction *> actions = menu->actions();
    for (int i = 0; i < actions.size(); ++i) {
        QAction* action = actions[i];
        if (action->text() == "Save page") {
            action->setVisible(false);
        } else if (action->text().toLower() == "view page source") {
            action->setText( tr("View Page Source") );
            action->setShortcut( Qt::CTRL | Qt::Key_S );
            action->setShortcutVisibleInContextMenu(true);
        } else if (action->text() == "Back" ) {
#ifdef Q_OS_DARWIN
            //action->setText( tr("Back") + "                                    "+QChar(0x2318) + QChar(0x2190) );
#else
            action->setText( tr("Back") + "        "+"Ctrl" +  QChar(0x2190));
#endif
            action->setShortcut( Qt::CTRL | Qt::Key_Left );
            action->setShortcutVisibleInContextMenu(true);
        } else if (action->text() == "Forward" ) {
#ifdef Q_OS_DARWIN
            //action->setText( tr("Forward") + "                             "+QChar(0x2318) + QChar(0x2192) );
#else
            action->setText( tr("Forward") + "        "+"Ctrl" + QChar(0x2192) );
#endif
            action->setShortcut( Qt::CTRL | Qt::Key_Right );
            action->setShortcutVisibleInContextMenu(true);
        } else if (action->text() == "Reload" ) {
            action->setText( tr("Reload") );
            action->setShortcut( Qt::CTRL | Qt::Key_R );
            action->setShortcutVisibleInContextMenu(true);
        }
        //if (!dontShowMenuShortcut)
            //action->setShortcutVisibleInContextMenu(true);
    }
    if (_forKVWindow || !_isUtil) {
        QAction *zoom = menu->addAction( tr("Zoom In") );
        connect(zoom, &QAction::triggered, zoomInAction, &QAction::trigger);
        zoom->setShortcut( Qt::CTRL | Qt::Key_Plus );
        zoom->setShortcutVisibleInContextMenu(true);

        QAction *zoomOut = menu->addAction( tr("Zoom Out") );
        connect(zoomOut, &QAction::triggered, zoomOutAction, &QAction::trigger);
        zoomOut->setShortcut( Qt::CTRL | Qt::Key_Minus);
        zoomOut->setShortcutVisibleInContextMenu(true);

        QAction *zoomReset = menu->addAction( tr("Zoom Reset") );
        connect(zoomReset, &QAction::triggered, zoomOutAction, &QAction::trigger);
        zoomReset->setShortcut( Qt::CTRL | Qt::Key_0);
        zoomReset->setShortcutVisibleInContextMenu(true);
    }
    menu->addSeparator();
    QAction *action = menu->addAction( tr("Open DevTools") );
    action->setShortcut( Qt::CTRL | Qt::Key_D );
    action->setShortcutVisibleInContextMenu(true);
    connect(action, &QAction::triggered, [this]() { emit onDevToolsRequested(); });
    menu->popup(event->globalPos());
}
