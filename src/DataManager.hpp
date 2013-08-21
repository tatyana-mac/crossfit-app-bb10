#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <bb/cascades/GroupDataModel>

using namespace bb::cascades;

class DataManager: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bb::cascades::DataModel* dataModel READ dataModel CONSTANT)

public:
    DataManager();

    Q_INVOKABLE bool createRecord(const QString &firstName, const QString &lastName);
    Q_INVOKABLE void readRecords();
    Q_INVOKABLE bool updateRecord(const QString &key, const QString &firstName, const QString &lastName);
    Q_INVOKABLE bool deleteRecord(const QString &key);

private:
    // Functions to call upon initialization to setup the model and database
    void initDataModel();
    bool initDatabase();

    // The getter method for the property
    bb::cascades::GroupDataModel* dataModel() const;

    // The data shown by the list view.
    GroupDataModel* m_dataModel;
};

#endif
