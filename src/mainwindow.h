#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QGroupBox>
#include <QTimer>

#include "serialport.h"
#include "datamanager.h"
#include "chartwidget.h"

/**
 * @brief 主窗口类，整合所有功能模块
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 串口控制
    void onConnectButtonClicked();
    void onRefreshPortsClicked();

    // 数据接收
    void onDistanceReceived(double distance);

    // 数据管理
    void onSaveDataClicked();
    void onQueryDataClicked();
    void onExportCSVClicked();
    void onExportTXTClicked();
    void onClearDataClicked();

    // 图表控制
    void onClearChartClicked();
    void onPauseChartClicked();

    // 状态更新
    void onConnectionStatusChanged(bool connected);
    void onErrorOccurred(const QString &error);

    // 定时更新统计信息
    void updateStatistics();

private:
    // UI组件
    void setupUI();
    void createSerialPortGroup();
    void createDisplayGroup();
    void createDataManagementGroup();
    void createChartGroup();
    void createStatusBar();

    // 核心组件
    SerialPortHandler *m_serialPort;
    DataManager *m_dataManager;
    ChartWidget *m_chartWidget;

    // 串口控制组件
    QComboBox *m_portComboBox;
    QSpinBox *m_baudRateSpinBox;
    QPushButton *m_connectButton;
    QPushButton *m_refreshPortsButton;
    QLabel *m_connectionStatusLabel;

    // 实时显示组件
    QLabel *m_currentDistanceLabel;
    QLabel *m_currentDistanceValue;
    QLabel *m_lastUpdateLabel;

    // 数据管理组件
    QCheckBox *m_autoSaveCheckBox;
    QPushButton *m_saveDataButton;
    QPushButton *m_queryDataButton;
    QPushButton *m_exportCSVButton;
    QPushButton *m_exportTXTButton;
    QPushButton *m_clearDataButton;
    QTableWidget *m_dataTableWidget;

    // 图表控制组件
    QPushButton *m_clearChartButton;
    QPushButton *m_pauseChartButton;
    bool m_isChartPaused;

    // 统计信息组件
    QLabel *m_totalRecordsLabel;
    QLabel *m_avgDistanceLabel;
    QLabel *m_minDistanceLabel;
    QLabel *m_maxDistanceLabel;

    // 日志显示
    QTextEdit *m_logTextEdit;

    // 定时器
    QTimer *m_statisticsTimer;

    // 辅助方法
    void logMessage(const QString &message);
    void updateConnectionButton(bool connected);
    void loadRecentData();
};

#endif // MAINWINDOW_H
