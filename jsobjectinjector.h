#ifndef JSOBJECTINJECTOR_H
#define JSOBJECTINJECTOR_H

#include <QObject>
#include <QWebChannel>
#include <QWebEnginePage>

/// This can be used to set up a object to js "window", which may later be used to
///   communicate from js to c++.
/// Each JsObjectInjector should only inject 1 object at most.
class JsObjectInjector : public QObject
{
    Q_OBJECT
public:
    explicit JsObjectInjector(QObject *parent = nullptr);

    // @return 0 on success, others on failure.
    int InjectObject(QWebEnginePage* page, const QString &name, QObject *objToInject);

private:
    QWebChannel *m_webChannel = nullptr;
};

#endif // JSOBJECTINJECTOR_H
