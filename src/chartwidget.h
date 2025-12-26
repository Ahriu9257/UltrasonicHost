#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QDateTime>
#include <QVector>

/**
 * @brief 波形图显示组件，实时显示距离变化曲线
 */
class ChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();

    // 添加数据点
    void addDataPoint(double distance);

    // 设置显示参数
    void setMaxDataPoints(int count);  // 最大显示点数
    void setYAxisRange(double min, double max);  // Y轴范围

    // 清空数据
    void clearData();

    // 暂停/恢复更新
    void setPaused(bool paused);

private:
    QChartView *m_chartView;
    QChart *m_chart;
    QLineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    int m_maxDataPoints;
    int m_dataCounter;
    bool m_isPaused;

    void setupChart();
};

#endif // CHARTWIDGET_H
