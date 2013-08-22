#include "Crossfit.hpp"
#include "DataManager.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

Crossfit::Crossfit(bb::cascades::Application *app)
: QObject(app)
{
    m_dataManager = new DataManager();

    // register types to be used in QML
    qmlRegisterType<DataManager>("crossfit.app", 1, 0, "DataManager");

    // set QML scene
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    app->setScene(root);
}

DataManager* Crossfit::dataManager() const
{
    return m_dataManager;
}
