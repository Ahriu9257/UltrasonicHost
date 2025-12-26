#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QDateTime>
#include <QVector>

/**
 * @brief 数据记录结构
 */
struct DistanceRecord {
    int id;
    QDateTime timestamp;
    double distance;

    DistanceRecord() : id(-1), distance(0.0) {}
    DistanceRecord(int i, const QDateTime &dt, double d)
        : id(i), timestamp(dt), distance(d) {}
};

/**
 * @brief 数据管理类，负责数据的保存、查询和导出
 */
class DataManager : public QObject {
    Q_OBJECT

public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

    // 初始化数据库
    bool initialize(const QString &dbPath = "ultrasonic_data.db");

    // 保存数据
    bool saveData(double distance);

    // 查询数据
    QVector<DistanceRecord> queryAll();
    QVector<DistanceRecord> queryByDateRange(const QDateTime &start, const QDateTime &end);
    QVector<DistanceRecord> queryRecent(int count = 100);

    // 删除数据
    bool deleteRecord(int id);
    bool clearAll();

    // 导出数据
    bool exportToCSV(const QString &filePath);
    bool exportToTXT(const QString &filePath);

    // 统计信息
    int getTotalRecords();
    double getAverageDistance();
    double getMaxDistance();
    double getMinDistance();

signals:
    void dataAdded(const DistanceRecord &record);
    void errorOccurred(const QString &error);

private:
    QSqlDatabase m_database;
    bool createTables();
};

#endif // DATAMANAGER_H
