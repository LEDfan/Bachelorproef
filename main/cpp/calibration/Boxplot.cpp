#include "Boxplot.h"

#include <QtCharts/QBoxPlotSeries>
#include <QtCharts/QBoxSet>
#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <algorithm>

QT_CHARTS_USE_NAMESPACE

namespace calibration {

/*
 * Inspired by https://doc.qt.io/qt-5.10/qtcharts-boxplotchart-example.html
 * */

Boxplot::Boxplot(std::map<std::string, std::vector<unsigned int>> results) : results(results) {}

double Boxplot::FindMedian(int begin, int end, std::string testcase)
{
        std::sort(results[testcase].begin(), results[testcase].end());
        int count = end - begin;
        if (count % 2) {
                return results[testcase].at(count / 2 + begin);
        } else {
                double right = results[testcase].at(count / 2 + begin);
                double left  = results[testcase].at(count / 2 - 1 + begin);
                return (right + left) / 2.0;
        }
}

void Boxplot::Display(int argc, char* argv[])
{
        for (auto testcasepair : results) {
                QApplication    a(argc, argv);
                QBoxPlotSeries* boxplot = new QBoxPlotSeries();
                unsigned int    count   = results[testcasepair.first].size();
                QBoxSet*        box     = new QBoxSet(testcasepair.first.c_str());
                box->setValue(QBoxSet::LowerExtreme,
                              *std::min_element(testcasepair.second.begin(), testcasepair.second.end()));
                box->setValue(QBoxSet::UpperExtreme,
                              *std::max_element(testcasepair.second.begin(), testcasepair.second.end()));
                box->setValue(QBoxSet::Median, FindMedian(0, count, testcasepair.first));
                box->setValue(QBoxSet::LowerQuartile, FindMedian(0, count / 2.0, testcasepair.first));
                box->setValue(QBoxSet::UpperQuartile, FindMedian(count / 2.0 + (count % 2), count, testcasepair.first));
                boxplot->append(box);

                QChart* chart = new QChart();
                chart->addSeries(boxplot);
                chart->setTitle("Boxplot of results");
                chart->setAnimationOptions(QChart::SeriesAnimations);
                chart->createDefaultAxes();
                chart->legend()->setVisible(true);
                chart->legend()->setAlignment(Qt::AlignBottom);

                QChartView* chartView = new QChartView(chart);
                chartView->setRenderHint(QPainter::Antialiasing);

                QMainWindow window;
                window.setCentralWidget(chartView);
                window.resize(800, 600);
                window.show();
                a.exec();
        }
}

} // namespace calibration
