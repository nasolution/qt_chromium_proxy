#include "jsobjectinjector.h"

#include <QDebug>
#include <QFile>

#define M_WEBCHANNEL_JS_PATH  ":/qwebchannel.js"

JsObjectInjector::JsObjectInjector(QObject *parent)
    : QObject{parent}
{

}

int JsObjectInjector::InjectObject(QWebEnginePage *page, const QString &name, QObject *objToInject)
{
    Q_ASSERT(page);
    Q_ASSERT(!name.isEmpty());
    Q_ASSERT(objToInject);

    if (m_webChannel != nullptr) {
        // Already injected.
        return -1;
    }

    m_webChannel = new QWebChannel(this);
    page->setWebChannel(m_webChannel);
    m_webChannel->registerObject(name, objToInject);

    QFile file(M_WEBCHANNEL_JS_PATH);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Resources not found.
        return -2;
    }

    QString jsContent = file.readAll();

    //qDebug() << "js content:\n" << jsContent;

    //jsContent = R"( console.log("hello kv"); )";

    page->runJavaScript(jsContent, [](const QVariant &v) {
        Q_UNUSED(v);
        // After run.
        //qDebug() << "after injection:" << v.toString();
    });

    return 0;
}
