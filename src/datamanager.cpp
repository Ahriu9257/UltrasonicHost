#include "datamanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>

DataManager::DataManager(QObject *parent)
    : QObject(parent)
{
}

DataManager::~DataManager()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DataManager::initialize(const QString &dbPath)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        QString error = QString("Database open failed: %1").arg(m_database.lastError().text());
        emit errorOccurred(error);
        qDebug() << error;
        return false;
    }

    if (!createTables()) {
        return false;
    }

    qDebug() << "Database initialized:" << dbPath;
    return true;
}

bool DataManager::createTables()
{
    QSqlQuery query(m_database);

    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS distance_records (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            distance REAL NOT NULL
        )
    )";

    if (!query.exec(createTableSQL)) {
        QString error = QString("Table creation failed: %1").arg(query.lastError().text());
        emit errorOccurred(error);
        qDebug() << error;
        return false;
    }

    query.exec("CREATE INDEX IF NOT EXISTS idx_timestamp ON distance_records(timestamp)");
    return true;
}

bool DataManager::saveData(double distance)
{
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO distance_records (timestamp, distance) VALUES (?, ?)");
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(distance);

    if (!query.exec()) {
        QString error = QString("Data save failed: %1").arg(query.lastError().text());
        emit errorOccurred(error);
        qDebug() << error;
        return false;
    }

    int id = query.lastInsertId().toInt();
    DistanceRecord record(id, QDateTime::currentDateTime(), distance);
    emit dataAdded(record);
    return true;
}

QVector<DistanceRecord> DataManager::queryAll()
{
    QVector<DistanceRecord> records;
    QSqlQuery query(m_database);

    if (!query.exec("SELECT id, timestamp, distance FROM distance_records ORDER BY timestamp DESC")) {
        emit errorOccurred(QString("Query failed: %1").arg(query.lastError().text()));
        return records;
    }

    while (query.next()) {
        DistanceRecord record;
        record.id = query.value(0).toInt();
        record.timestamp = query.value(1).toDateTime();
        record.distance = query.value(2).toDouble();
        records.append(record);
    }
    return records;
}

QVector<DistanceRecord> DataManager::queryByDateRange(const QDateTime &start, const QDateTime &end)
{
    QVector<DistanceRecord> records;
    QSqlQuery query(m_database);

    query.prepare("SELECT id, timestamp, distance FROM distance_records WHERE timestamp BETWEEN ? AND ? ORDER BY timestamp DESC");
    query.addBindValue(start);
    query.addBindValue(end);

    if (!query.exec()) {
        emit errorOccurred(QString("Range query failed: %1").arg(query.lastError().text()));
        return records;
    }

    while (query.next()) {
        records.append(DistanceRecord(query.value(0).toInt(), query.value(1).toDateTime(), query.value(2).toDouble()));
    }
    return records;
}

QVector<DistanceRecord> DataManager::queryRecent(int count)
{
    QVector<DistanceRecord> records;
    QSqlQuery query(m_database);
    query.prepare("SELECT id, timestamp, distance FROM distance_records ORDER BY timestamp DESC LIMIT ?");
    query.addBindValue(count);

    if (query.exec()) {
        while (query.next()) {
            records.append(DistanceRecord(query.value(0).toInt(), query.value(1).toDateTime(), query.value(2).toDouble()));
        }
    }
    return records;
}

bool DataManager::deleteRecord(int id)
{
    QSqlQuery query(m_database);
    query.prepare("DELETE FROM distance_records WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

bool DataManager::clearAll()
{
    QSqlQuery query(m_database);
    if (!query.exec("DELETE FROM distance_records")) {
        return false;
    }
    query.exec("DELETE FROM sqlite_sequence WHERE name='distance_records'");
    return true;
}

bool DataManager::exportToCSV(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << "ID,Timestamp,Distance(cm)\n";

    for (const auto &record : queryAll()) {
        out << record.id << "," << record.timestamp.toString("yyyy-MM-dd hh:mm:ss") << "," << QString::number(record.distance, 'f', 2) << "\n";
    }
    file.close();
    return true;
}

bool DataManager::exportToTXT(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << "Ultrasonic Distance Measurement Data Export\n";
    out << "==========================================\n\n";

    QVector<DistanceRecord> records = queryAll();
    out << QString("Total Records: %1\n").arg(records.size());
    out << QString("Export Time: %1\n\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    for (const auto &record : records) {
        out << QString("[%1] %2 - %3 cm\n").arg(record.id, 5).arg(record.timestamp.toString("yyyy-MM-dd hh:mm:ss")).arg(record.distance, 6, 'f', 2);
    }
    file.close();
    return true;
}

int DataManager::getTotalRecords()
{
    QSqlQuery query(m_database);
    query.exec("SELECT COUNT(*) FROM distance_records");
    return query.next() ? query.value(0).toInt() : 0;
}

double DataManager::getAverageDistance()
{
    QSqlQuery query(m_database);
    query.exec("SELECT AVG(distance) FROM distance_records");
    return query.next() ? query.value(0).toDouble() : 0.0;
}

double DataManager::getMaxDistance()
{
    QSqlQuery query(m_database);
    query.exec("SELECT MAX(distance) FROM distance_records");
    return query.next() ? query.value(0).toDouble() : 0.0;
}

double DataManager::getMinDistance()
{
    QSqlQuery query(m_database);
    query.exec("SELECT MIN(distance) FROM distance_records");
    return query.next() ? query.value(0).toDouble() : 0.0;
}
