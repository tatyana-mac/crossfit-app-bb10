#ifndef CROSSFIT_HPP_
#define CROSSFIT_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

class DataManager;

class Crossfit : public QObject
{
    Q_OBJECT

    Q_PROPERTY(DataManager* dataManager READ dataManager CONSTANT)

public:
    Crossfit(bb::cascades::Application *app);
    virtual ~Crossfit() {}

private:
    DataManager* dataManager() const;

private:
    DataManager* m_dataManager;
};


#endif // CROSSFIT_HPP_
