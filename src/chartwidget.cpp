#include "chartwidget.h"
#include <QVBoxLayout>
#include <QtCharts/QChart>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
    , m_chartView(new QChartView(this))
    , m_chart(new QChart())
    , m_series(new QLineSeries())
    , m_axisX(new QValueAxis())
    , m_axisY(new QValueAxis())
    , m_maxDataPoints(100)
    , m_dataCounter(0)
    , m_isPaused(false)
{
    setupChart();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

ChartWidget::~ChartWidget()
{
}

void ChartWidget::setupChart()
{
    // 设置曲线样式
    QPen pen(QColor(0, 120, 215));
    pen.setWidth(2);
    m_series->setPen(pen);
    m_series->setName("Distance");

    // 配置图表
    m_chart->addSeries(m_series);
    m_chart->setTitle("Real-time Distance Measurement");
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    // 设置X轴（时间序列）
    m_axisX->setTitleText("Time (samples)");
    m_axisX->setLabelFormat("%d");
    m_axisX->setRange(0, m_maxDataPoints);
    m_axisX->setTickCount(11);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_series->attachAxis(m_axisX);

    // 设置Y轴（距离）
    m_axisY->setTitleText("Distance (cm)");
    m_axisY->setLabelFormat("%.1f");
    m_axisY->setRange(0, 400);
    m_axisY->setTickCount(9);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisY);

    // 配置图表视图
    m_chartView->setChart(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
}

void ChartWidget::addDataPoint(double distance)
{
    if (m_isPaused) {
        return;
    }

    // 添加数据点
    m_series->append(m_dataCounter, distance);
    m_dataCounter++;

    // 限制显示的数据点数量
    if (m_series->count() > m_maxDataPoints) {
        m_series->remove(0);
        // 更新X轴范围以显示最新数据
        m_axisX->setRange(m_dataCounter - m_maxDataPoints, m_dataCounter);
    }
}

void ChartWidget::setMaxDataPoints(int count)
{
    m_maxDataPoints = count;
    m_axisX->setRange(0, m_maxDataPoints);
}

void ChartWidget::setYAxisRange(double min, double max)
{
    m_axisY->setRange(min, max);
}

void ChartWidget::clearData()
{
    m_series->clear();
    m_dataCounter = 0;
    m_axisX->setRange(0, m_maxDataPoints);
}

void ChartWidget::setPaused(bool paused)
{
    m_isPaused = paused;
}
