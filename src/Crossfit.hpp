#ifndef CROSSFIT_HPP_
#define CROSSFIT_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

class DataManager;

class Crossfit : public QObject
{
    Q_OBJECT
public:
    Crossfit(bb::cascades::Application *app);
    virtual ~Crossfit() {}

private:
    DataManager* m_dataManager;
};


#endif // CROSSFIT_HPP_
