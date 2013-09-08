#include "DataManager.hpp"

#include <bb/data/SqlConnection>
#include <bb/data/SqlDataAccess>

#include <QtSql/QtSql>
#include <QDebug>

using namespace bb::cascades;
using namespace bb::data;

const QList<QString> DataManager::DEFAULT_EXERCISE_TYPES = DataManager::getDefaultExerciseTypes();
const QList<QString> DataManager::DEFAULT_WOD_TYPES = DataManager::getDefaultWodTypes();

DataManager::DataManager()
    : m_dataModel(0)
{
    initDataModel();
    initData();
    readRecords();
}

QList<QString> DataManager::getDefaultExerciseTypes()
{
    QList<QString> exTypes;
    exTypes.append(tr("Front Squat"));
    exTypes.append(tr("High Bar Back Squat"));
    exTypes.append(tr("Clean"));
    exTypes.append(tr("Hang Snatch"));
    exTypes.append(tr("Split Snatch"));
    exTypes.append(tr("Jerk"));
    exTypes.append(tr("Push Jerk"));
    exTypes.append(tr("Overhead Squat"));
    exTypes.append(tr("Push Press"));
    exTypes.append(tr("Bench Press"));
    exTypes.append(tr("Thruster"));
    exTypes.append(tr("Snatch"));
    exTypes.append(tr("Deadlift"));
    return exTypes;
}

QList<QString> DataManager::getDefaultWodTypes()
{
    QList<QString> wodTypes;
    wodTypes.append(tr("Claudia"));
    wodTypes.append(tr("Cindy"));
    wodTypes.append(tr("Fran"));
    return wodTypes;
}

bool sortByName(const QVariant& val1, const QVariant& val2)
{
    return val1.toMap()["name"].toString().compare(val2.toMap()["name"].toString()) < 0;
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
        qDebug() << "Workout table created.";
    }
    else
    {
        const QSqlError error = query.lastError();
        qDebug() << "Create workout table error: " + error.text();
        return false;
    }

    initExerciseTypes(database);
    initWodTypes(database);

    return true;
}

void DataManager::initExerciseTypes(QSqlDatabase& database)
{
    QSqlQuery query(database);
    /*if (query.exec("DROP TABLE IF EXISTS exerciseTypes"))
    {
        qDebug() << "Exercise table dropped.";
    }
    else
    {
        const QSqlError error = query.lastError();
        qDebug() << "Drop table error: " + error.text();
    }*/

    QString request = "CREATE TABLE IF NOT EXISTS exerciseTypes "
                      "  (exerciseID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "  name VARCHAR);";
    if (query.exec(request))
    {
        qDebug() << "Exercise table created.";
    }
    else
    {
        const QSqlError error = query.lastError();
        qDebug() << "Create exercise table error: " + error.text();
        return;
    }
    // see if the table is empty, populate with defaults if yes
    int exCount = 0;
    if (query.exec("SELECT COUNT(*) FROM exerciseTypes;"))
    {
        query.next();
        exCount = query.value(0).toInt();
        qDebug() << "Existing exercise number: " << exCount;
    }
    if (exCount == 0)
    {
        for (int i = 0; i < DEFAULT_EXERCISE_TYPES.size(); ++i)
        {
            query.prepare("INSERT INTO exerciseTypes"
                          "    (name) "
                          "    VALUES (:name)");
            query.bindValue(":name", DEFAULT_EXERCISE_TYPES[i]);
            if (!query.exec())
                qDebug() << "Insert exercise type - SQL error: " + query.lastError().text();
        }
    }
}

void DataManager::initWodTypes(QSqlDatabase& database)
{
    QSqlQuery query(database);
    /*if (query.exec("DROP TABLE IF EXISTS wodTypes"))
    {
        qDebug() << "WOD table dropped.";
    }
    else
    {
        const QSqlError error = query.lastError();
        qDebug() << "Drop table error: " + error.text();
    }*/

    QString request = "CREATE TABLE IF NOT EXISTS wodTypes "
                      "  (wodID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "  name VARCHAR);";
    if (query.exec(request))
    {
        qDebug() << "WOD table created.";
    }
    else
    {
        const QSqlError error = query.lastError();
        qDebug() << "Create WOD table error: " + error.text();
        return;
    }
    // see if the table is empty, populate with defaults if yes
    int exCount = 0;
    if (query.exec("SELECT COUNT(*) FROM wodTypes;"))
    {
        query.next();
        exCount = query.value(0).toInt();
        qDebug() << "Existing WOD number: " << exCount;
    }
    if (exCount == 0)
    {
        for (int i = 0; i < DEFAULT_WOD_TYPES.size(); ++i)
        {
            query.prepare("INSERT INTO wodTypes"
                          "    (name) "
                          "    VALUES (:name)");
            query.bindValue(":name", DEFAULT_WOD_TYPES[i]);
            if (!query.exec())
                qDebug() << "Insert WOD type - SQL error: " + query.lastError().text();
        }
    }
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

QVariantList DataManager::getExerciseTypes() const
{
    QSqlDatabase database = QSqlDatabase::database();
    QSqlQuery query(database);
    QVariantList result;

    if (query.exec("SELECT exerciseID, name FROM exerciseTypes"))
    {
        while (query.next())
        {
            QVariantMap exerciseType;
            exerciseType["id"] = query.value(0).toInt();
            exerciseType["name"] = query.value(1).toString();
            result.append(exerciseType);
        }
    }
    else
        qDebug() << "Reading exercise types error: " << query.lastError().text();

    qSort(result.begin(), result.end(), sortByName);
    return result;
}

QVariantList DataManager::getWodTypes() const
{
    QSqlDatabase database = QSqlDatabase::database();
    QSqlQuery query(database);
    QVariantList result;

    if (query.exec("SELECT wodID, name FROM wodTypes"))
    {
        while (query.next())
        {
            QVariantMap wodType;
            wodType["id"] = query.value(0).toInt();
            wodType["name"] = query.value(1).toString();
            result.append(wodType);
        }
    }
    else
        qDebug() << "Reading WOD types error: " << query.lastError().text();

    qSort(result.begin(), result.end(), sortByName);
    return result;
}

GroupDataModel* DataManager::dataModel() const
{
    return m_dataModel;
}
