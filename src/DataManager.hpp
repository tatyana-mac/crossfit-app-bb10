#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <bb/cascades/GroupDataModel>

using namespace bb::cascades;

class QSqlDatabase;

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

    Q_INVOKABLE QVariantList getExerciseTypes() const;
    Q_INVOKABLE QVariantList getWodTypes() const;

private:
    // Functions to call upon initialization to setup the model and database
    void initDataModel();
    bool initData();
    void initExerciseTypes(QSqlDatabase& database);
    void initWodTypes(QSqlDatabase& database);

    static QList<QString> getDefaultExerciseTypes();
    static QList<QString> getDefaultWodTypes();

    // The getter method for the property
    bb::cascades::GroupDataModel* dataModel() const;

private:
    static const QList<QString> DEFAULT_EXERCISE_TYPES;
    static const QList<QString> DEFAULT_WOD_TYPES;

    GroupDataModel* m_dataModel; // data shown by the workout list view
};

#endif
