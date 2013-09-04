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

    Q_INVOKABLE bool createRecord(const QString& strength, const QString& wod);
    Q_INVOKABLE void readRecords();
    Q_INVOKABLE bool updateRecord(const QString &key, const QString& strength, const QString& wod);
    Q_INVOKABLE bool deleteRecord(const QString &key);

    Q_INVOKABLE QList<QString> getExerciseTypes() const;
    Q_INVOKABLE QList<QString> getWodNames() const;

private:
    // Functions to call upon initialization to setup the model and database
    void initDataModel();
    bool initData();
    void initExerciseTypes();
    void initWodNames();
    int getExerciseId(const QString& ex) const;
    int getWodNameId(const QString& wod) const;

    // The getter method for the property
    bb::cascades::GroupDataModel* dataModel() const;

private:
    GroupDataModel* m_dataModel; // data shown by the workout list view
    QList<QString> m_excerciseTypeList; // the list of exercise types
    QList<QString> m_wodNameList; // the list of wod names
};

#endif
