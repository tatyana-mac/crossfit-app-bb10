#include "DataManager.hpp"

#include <bb/data/SqlConnection>
#include <bb/data/SqlDataAccess>

#include <QtSql/QtSql>
#include <QDebug>

using namespace bb::cascades;
using namespace bb::data;

DataManager::DataManager()
    : m_dataModel(0)
{
    initDataModel();
    initData();
    readRecords();
}

void DataManager::initDataModel()
{
    // Note: The Group Data Model is joining this objects tree as a child (for memory management)
    m_dataModel = new GroupDataModel(this);
    m_dataModel->setSortingKeys(QStringList() << "workoutID");
    m_dataModel->setGrouping(ItemGrouping::None);
}

bool DataManager::initData()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("./data/crossfitDatabase.db");

    if (database.open() == false)
    {
        const QSqlError error = database.lastError();
        qDebug() << "Error opening connection to the database: " + error.text();
        return false;
    }

    QSqlQuery query(database);
    // remove this once basic operations are working
    /*if (query.exec("DROP TABLE IF EXISTS workouts"))
    {
        qDebug() << "Table dropped.";
    }
    else
    {
        const QSqlError error = query.lastError();
        qDebug() << "Drop table error: " + error.text();
    }
    */
    const QString createSQL = "CREATE TABLE IF NOT EXISTS workouts "
                              "  (workoutID INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "  strength VARCHAR, "
                              "  wod VARCHAR);";
    if (query.exec(createSQL))
    {
        qDebug() << "Table created.";
    }
    else
    {
        const QSqlError error = query.lastError();
        qDebug() << "Create table error: " + error.text();
        return false;
    }

    //initExerciseTypes();
    //initWodNames();

    return true;
}

void DataManager::initExerciseTypes()
{
    // TODO: allow the user to modify the list, put this in the database so that I can refer to these using their IDs
    m_excerciseTypeList.clear();
    m_excerciseTypeList.append(tr("Front Squat"));
    m_excerciseTypeList.append(tr("High Bar Back Squat"));
    m_excerciseTypeList.append(tr("Clean"));
    m_excerciseTypeList.append(tr("Hang Snatch"));
    m_excerciseTypeList.append(tr("Split Snatch"));
    m_excerciseTypeList.append(tr("Jerk"));
    m_excerciseTypeList.append(tr("Push Jerk"));
    m_excerciseTypeList.append(tr("Overhead Squat"));
    m_excerciseTypeList.append(tr("Push Press"));
    m_excerciseTypeList.append(tr("Push Jerk"));
    m_excerciseTypeList.append(tr("Bench Press"));
    m_excerciseTypeList.append(tr("Thruster"));
    m_excerciseTypeList.append(tr("Snatch"));
    m_excerciseTypeList.append(tr("Deadlift"));

    qSort(m_excerciseTypeList.begin(), m_excerciseTypeList.end());
}

void DataManager::initWodNames()
{
    // TODO: allow the user to modify the list, put this in the database so that I can refer to these using their IDs
    m_wodNameList.clear();
    m_wodNameList.append(tr("Claudia"));
    m_wodNameList.append(tr("Cindy"));
    m_wodNameList.append(tr("Fran"));

    qSort(m_wodNameList.begin(), m_excerciseTypeList.end());
}

bool DataManager::createRecord(const QString& strength, const QString& wod)
{
    QSqlDatabase database = QSqlDatabase::database();
    QSqlQuery query(database);
    query.prepare("INSERT INTO workouts"
                  "    (strength, wod) "
                  "    VALUES (:strength, :wod)");
    query.bindValue(":strength", strength);
    query.bindValue(":wod", wod);

    bool success = false;
    if (query.exec())
    {
        success = true;
        // refresh the data model
        readRecords();
        qDebug() << "Entry created: " << strength << " " << wod;
    }
    else
    {
        const QSqlError error = query.lastError();
        qDebug() << "Create record - SQL error: " + error.text();
    }

    database.close();
    return success;
}

bool DataManager::updateRecord(const QString& workoutID, const QString& strength, const QString& wod)
{
    bool intConversionGood = false;
    const int workoutIDNum = workoutID.toInt(&intConversionGood);
    if (!intConversionGood)
    {
        qDebug() << "Invalid workout ID " << workoutID;
        return false;
    }

    QSqlDatabase database = QSqlDatabase::database();
    QSqlQuery query(database);
    const QString sqlCommand = "UPDATE workouts "
                               "    SET strength = :strength, wod = :wod"
                               "    WHERE workoutID = :workoutID";
    query.prepare(sqlCommand);

    query.bindValue(":strength", strength);
    query.bindValue(":wod", wod);
    query.bindValue(":workoutID", workoutIDNum);

    bool updated = false;
    if (query.exec())
    {
        if (query.numRowsAffected() > 0)
        {
            updated = true;
            // refresh the data model
            readRecords();
        }
        else
            qDebug() << "Did not find workout ID " << workoutID;
    }
    else
    {
        qDebug() << "Update record - SQL error: " << query.lastError().text();
    }

    database.close();
    return updated;
}

bool DataManager::deleteRecord(const QString &workoutID)
{
    bool intConversionGood = false;
    const int workoutIDNum = workoutID.toInt(&intConversionGood);
    if (!intConversionGood)
    {
        qDebug() << "Invalid workout ID " << workoutID;
        return false;
    }

    QSqlDatabase database = QSqlDatabase::database(); // open the default database.
    QSqlQuery query(database);
    query.prepare("DELETE FROM workouts WHERE workoutID=:workoutID");
    query.bindValue(":workoutID", workoutIDNum);

    bool deleted = false;
    if (query.exec())
    {
        if (query.numRowsAffected() > 0)
        {
            deleted = true;
            // refresh the data model
            readRecords();
        }
        else
            qDebug() << "Did not find workout ID " << workoutID;
    }
    else
    {
        qDebug() << "Delete record - SQL error: " << query.lastError().text();
    }

    database.close();
    return deleted;
}

void DataManager::readRecords()
{
    QSqlDatabase database = QSqlDatabase::database(); // opens the default database.
    QSqlQuery query(database);
    const QString sqlQuery = "SELECT workoutID, strength, wod FROM workouts";

    if (query.exec(sqlQuery))
    {
        const int workoutIDField = query.record().indexOf("workoutID");
        const int strengthField = query.record().indexOf("strength");
        const int wodField = query.record().indexOf("wod");

        m_dataModel->clear();
        int recordsRead = 0;
        while (query.next())
        {
            QVariantMap workout;
            workout["id"] = query.value(workoutIDField).toString();
            workout["strength"] = query.value(strengthField).toString();
            workout["wod"] = query.value(wodField).toString();

            m_dataModel->insert(workout);
            recordsRead++;
        }
        qDebug() << "Read " << recordsRead << " workout records";
    }
    else
    {
        qDebug() << "Reading records error: " << query.lastError().text();
    }
    database.close();
}

GroupDataModel* DataManager::dataModel() const
{
    return m_dataModel;
}

QList<QString> DataManager::getExerciseTypes() const
{
    return m_excerciseTypeList;
}

QList<QString> DataManager::getWodNames() const
{
    return m_wodNameList;
}

int DataManager::getExerciseId(const QString& ex) const
{
    return m_excerciseTypeList.indexOf(ex);
}

int DataManager::getWodNameId(const QString& wod) const
{
    return m_wodNameList.indexOf(wod);
}
