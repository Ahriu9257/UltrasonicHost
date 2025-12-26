#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QSplitter>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_serialPort(new SerialPortHandler(this))
    , m_dataManager(new DataManager(this))
    , m_chartWidget(new ChartWidget(this))
    , m_isChartPaused(false)
    , m_statisticsTimer(new QTimer(this))
{
    setupUI();

    // 初始化数据库
    if (!m_dataManager->initialize()) {
        QMessageBox::critical(this, "Error", "Failed to initialize database!");
    }

    // 连接信号槽
    connect(m_serialPort, &SerialPortHandler::distanceReceived,
            this, &MainWindow::onDistanceReceived);
    connect(m_serialPort, &SerialPortHandler::connectionStatusChanged,
            this, &MainWindow::onConnectionStatusChanged);
    connect(m_serialPort, &SerialPortHandler::errorOccurred,
            this, &MainWindow::onErrorOccurred);

    // 统计信息定时器
    connect(m_statisticsTimer, &QTimer::timeout, this, &MainWindow::updateStatistics);
    m_statisticsTimer->start(1000);

    // 刷新可用串口
    onRefreshPortsClicked();

    logMessage("Application started successfully");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("Ultrasonic Distance Measurement System");
    resize(1200, 800);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 创建所有UI组件
    createSerialPortGroup();
    createDisplayGroup();
    createChartGroup();
    createDataManagementGroup();

    // 创建日志组件
    m_logTextEdit = new QTextEdit();
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setMaximumHeight(120);

    // 顶部控制区域
    QHBoxLayout *topLayout = new QHBoxLayout();

    QGroupBox *serialGroup = new QGroupBox("Serial Port Control");
    QVBoxLayout *serialLayout = new QVBoxLayout();

    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(new QLabel("Port:"));
    portLayout->addWidget(m_portComboBox);
    portLayout->addWidget(m_refreshPortsButton);

    QHBoxLayout *baudLayout = new QHBoxLayout();
    baudLayout->addWidget(new QLabel("Baud Rate:"));
    baudLayout->addWidget(m_baudRateSpinBox);

    serialLayout->addLayout(portLayout);
    serialLayout->addLayout(baudLayout);
    serialLayout->addWidget(m_connectButton);
    serialLayout->addWidget(m_connectionStatusLabel);
    serialGroup->setLayout(serialLayout);

    QGroupBox *displayGroup = new QGroupBox("Real-time Display");
    QVBoxLayout *displayLayout = new QVBoxLayout();

    QHBoxLayout *distanceLayout = new QHBoxLayout();
    distanceLayout->addWidget(m_currentDistanceLabel);
    distanceLayout->addWidget(m_currentDistanceValue);
    distanceLayout->addStretch();

    displayLayout->addLayout(distanceLayout);
    displayLayout->addWidget(m_lastUpdateLabel);
    displayGroup->setLayout(displayLayout);

    topLayout->addWidget(serialGroup);
    topLayout->addWidget(displayGroup, 1);

    // 中间区域 - 分割器
    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    // 左侧 - 图表
    QWidget *chartContainer = new QWidget();
    QVBoxLayout *chartContainerLayout = new QVBoxLayout(chartContainer);
    chartContainerLayout->addWidget(m_chartWidget);

    QHBoxLayout *chartControlLayout = new QHBoxLayout();
    chartControlLayout->addWidget(m_pauseChartButton);
    chartControlLayout->addWidget(m_clearChartButton);
    chartControlLayout->addStretch();
    chartContainerLayout->addLayout(chartControlLayout);

    // 右侧 - 数据管理
    QGroupBox *dataGroup = new QGroupBox("Data Management");
    QVBoxLayout *dataGroupLayout = new QVBoxLayout();

    dataGroupLayout->addWidget(m_autoSaveCheckBox);

    QHBoxLayout *buttonLayout1 = new QHBoxLayout();
    buttonLayout1->addWidget(m_saveDataButton);
    buttonLayout1->addWidget(m_queryDataButton);

    QHBoxLayout *buttonLayout2 = new QHBoxLayout();
    buttonLayout2->addWidget(m_exportCSVButton);
    buttonLayout2->addWidget(m_exportTXTButton);

    dataGroupLayout->addLayout(buttonLayout1);
    dataGroupLayout->addLayout(buttonLayout2);
    dataGroupLayout->addWidget(m_clearDataButton);

    // 统计信息
    QGroupBox *statsGroup = new QGroupBox("Statistics");
    QVBoxLayout *statsLayout = new QVBoxLayout();
    statsLayout->addWidget(m_totalRecordsLabel);
    statsLayout->addWidget(m_avgDistanceLabel);
    statsLayout->addWidget(m_minDistanceLabel);
    statsLayout->addWidget(m_maxDistanceLabel);
    statsGroup->setLayout(statsLayout);
    dataGroupLayout->addWidget(statsGroup);

    dataGroupLayout->addWidget(new QLabel("Recent Data:"));
    dataGroupLayout->addWidget(m_dataTableWidget);

    dataGroup->setLayout(dataGroupLayout);

    splitter->addWidget(chartContainer);
    splitter->addWidget(dataGroup);
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    // 底部日志区域
    QGroupBox *logGroup = new QGroupBox("Log");
    QVBoxLayout *logLayout = new QVBoxLayout();
    logLayout->addWidget(m_logTextEdit);
    logGroup->setLayout(logLayout);
    logGroup->setMaximumHeight(150);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(splitter, 1);
    mainLayout->addWidget(logGroup);

    createStatusBar();
}

void MainWindow::createSerialPortGroup()
{
    m_portComboBox = new QComboBox();
    m_baudRateSpinBox = new QSpinBox();
    m_baudRateSpinBox->setRange(1200, 115200);
    m_baudRateSpinBox->setValue(9600);
    m_baudRateSpinBox->setSingleStep(1200);

    m_connectButton = new QPushButton("Connect");
    m_refreshPortsButton = new QPushButton("Refresh");
    m_connectionStatusLabel = new QLabel("Disconnected");
    m_connectionStatusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");

    connect(m_connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
    connect(m_refreshPortsButton, &QPushButton::clicked, this, &MainWindow::onRefreshPortsClicked);
}

void MainWindow::createDisplayGroup()
{
    m_currentDistanceLabel = new QLabel("Current Distance:");
    m_currentDistanceValue = new QLabel("-- cm");
    m_currentDistanceValue->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #0078D7; }");
    m_lastUpdateLabel = new QLabel("Last Update: --");
}

void MainWindow::createDataManagementGroup()
{
    m_autoSaveCheckBox = new QCheckBox("Auto Save Data");
    m_autoSaveCheckBox->setChecked(true);

    m_saveDataButton = new QPushButton("Save Current");
    m_queryDataButton = new QPushButton("Query All");
    m_exportCSVButton = new QPushButton("Export CSV");
    m_exportTXTButton = new QPushButton("Export TXT");
    m_clearDataButton = new QPushButton("Clear All Data");

    m_dataTableWidget = new QTableWidget();
    m_dataTableWidget->setColumnCount(3);
    m_dataTableWidget->setHorizontalHeaderLabels({"ID", "Time", "Distance(cm)"});
    m_dataTableWidget->horizontalHeader()->setStretchLastSection(true);
    m_dataTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_dataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_totalRecordsLabel = new QLabel("Total: 0");
    m_avgDistanceLabel = new QLabel("Average: -- cm");
    m_minDistanceLabel = new QLabel("Min: -- cm");
    m_maxDistanceLabel = new QLabel("Max: -- cm");

    connect(m_saveDataButton, &QPushButton::clicked, this, &MainWindow::onSaveDataClicked);
    connect(m_queryDataButton, &QPushButton::clicked, this, &MainWindow::onQueryDataClicked);
    connect(m_exportCSVButton, &QPushButton::clicked, this, &MainWindow::onExportCSVClicked);
    connect(m_exportTXTButton, &QPushButton::clicked, this, &MainWindow::onExportTXTClicked);
    connect(m_clearDataButton, &QPushButton::clicked, this, &MainWindow::onClearDataClicked);
}

void MainWindow::createChartGroup()
{
    m_clearChartButton = new QPushButton("Clear Chart");
    m_pauseChartButton = new QPushButton("Pause");

    connect(m_clearChartButton, &QPushButton::clicked, this, &MainWindow::onClearChartClicked);
    connect(m_pauseChartButton, &QPushButton::clicked, this, &MainWindow::onPauseChartClicked);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Ready");
}

void MainWindow::onConnectButtonClicked()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->closePort();
    } else {
        QString portName = m_portComboBox->currentText();
        int baudRate = m_baudRateSpinBox->value();

        if (portName.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please select a serial port!");
            return;
        }

        if (m_serialPort->openPort(portName, baudRate)) {
            logMessage(QString("Connected to %1 at %2 baud").arg(portName).arg(baudRate));
        }
    }
}

void MainWindow::onRefreshPortsClicked()
{
    m_portComboBox->clear();
    QStringList ports = m_serialPort->getAvailablePorts();

    if (ports.isEmpty()) {
        logMessage("No serial ports found");
        QMessageBox::information(this, "Info", "No serial ports detected");
    } else {
        m_portComboBox->addItems(ports);
        logMessage(QString("Found %1 serial port(s)").arg(ports.size()));
    }
}

void MainWindow::onDistanceReceived(double distance)
{
    // 更新显示
    m_currentDistanceValue->setText(QString("%1 cm").arg(distance, 0, 'f', 2));
    m_lastUpdateLabel->setText(QString("Last Update: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));

    // 更新图表
    m_chartWidget->addDataPoint(distance);

    // 自动保存
    if (m_autoSaveCheckBox->isChecked()) {
        m_dataManager->saveData(distance);
    }

    logMessage(QString("Received: %1 cm").arg(distance, 0, 'f', 2));
}

void MainWindow::onSaveDataClicked()
{
    QString text = m_currentDistanceValue->text().remove(" cm");
    bool ok;
    double distance = text.toDouble(&ok);

    if (ok && distance >= 0) {
        if (m_dataManager->saveData(distance)) {
            logMessage("Data saved manually");
            loadRecentData();
        }
    } else {
        QMessageBox::warning(this, "Warning", "No valid distance data to save!");
    }
}

void MainWindow::onQueryDataClicked()
{
    loadRecentData();
    logMessage("Data queried");
}

void MainWindow::onExportCSVClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export to CSV", "ultrasonic_data.csv", "CSV Files (*.csv)");

    if (!fileName.isEmpty()) {
        if (m_dataManager->exportToCSV(fileName)) {
            QMessageBox::information(this, "Success", "Data exported successfully!");
            logMessage("Data exported to CSV: " + fileName);
        } else {
            QMessageBox::critical(this, "Error", "Failed to export data!");
        }
    }
}

void MainWindow::onExportTXTClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export to TXT", "ultrasonic_data.txt", "Text Files (*.txt)");

    if (!fileName.isEmpty()) {
        if (m_dataManager->exportToTXT(fileName)) {
            QMessageBox::information(this, "Success", "Data exported successfully!");
            logMessage("Data exported to TXT: " + fileName);
        } else {
            QMessageBox::critical(this, "Error", "Failed to export data!");
        }
    }
}

void MainWindow::onClearDataClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm",
        "Are you sure you want to clear all data?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_dataManager->clearAll()) {
            m_dataTableWidget->setRowCount(0);
            logMessage("All data cleared");
            updateStatistics();
        }
    }
}

void MainWindow::onClearChartClicked()
{
    m_chartWidget->clearData();
    logMessage("Chart cleared");
}

void MainWindow::onPauseChartClicked()
{
    m_isChartPaused = !m_isChartPaused;
    m_chartWidget->setPaused(m_isChartPaused);
    m_pauseChartButton->setText(m_isChartPaused ? "Resume" : "Pause");
    logMessage(m_isChartPaused ? "Chart paused" : "Chart resumed");
}

void MainWindow::onConnectionStatusChanged(bool connected)
{
    updateConnectionButton(connected);

    if (connected) {
        m_connectionStatusLabel->setText("Connected");
        m_connectionStatusLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
        statusBar()->showMessage("Serial port connected");
    } else {
        m_connectionStatusLabel->setText("Disconnected");
        m_connectionStatusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
        statusBar()->showMessage("Serial port disconnected");
    }
}

void MainWindow::onErrorOccurred(const QString &error)
{
    logMessage("ERROR: " + error);
    QMessageBox::warning(this, "Error", error);
}

void MainWindow::updateStatistics()
{
    m_totalRecordsLabel->setText(QString("Total: %1").arg(m_dataManager->getTotalRecords()));
    m_avgDistanceLabel->setText(QString("Average: %1 cm").arg(m_dataManager->getAverageDistance(), 0, 'f', 2));
    m_minDistanceLabel->setText(QString("Min: %1 cm").arg(m_dataManager->getMinDistance(), 0, 'f', 2));
    m_maxDistanceLabel->setText(QString("Max: %1 cm").arg(m_dataManager->getMaxDistance(), 0, 'f', 2));
}

void MainWindow::logMessage(const QString &message)
{
    QString timeStamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_logTextEdit->append(QString("[%1] %2").arg(timeStamp, message));
}

void MainWindow::updateConnectionButton(bool connected)
{
    if (connected) {
        m_connectButton->setText("Disconnect");
        m_portComboBox->setEnabled(false);
        m_baudRateSpinBox->setEnabled(false);
    } else {
        m_connectButton->setText("Connect");
        m_portComboBox->setEnabled(true);
        m_baudRateSpinBox->setEnabled(true);
    }
}

void MainWindow::loadRecentData()
{
    QVector<DistanceRecord> records = m_dataManager->queryRecent(20);

    m_dataTableWidget->setRowCount(records.size());

    for (int i = 0; i < records.size(); ++i) {
        m_dataTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(records[i].id)));
        m_dataTableWidget->setItem(i, 1, new QTableWidgetItem(records[i].timestamp.toString("yyyy-MM-dd hh:mm:ss")));
        m_dataTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(records[i].distance, 'f', 2)));
    }
}
