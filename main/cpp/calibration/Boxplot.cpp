#include "Boxplot.h"

#include <QtCharts/QBoxPlotSeries>
#include <QtCharts/QBoxSet>
#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <algorithm>
#include <iostream>

QT_CHARTS_USE_NAMESPACE

namespace calibration {

/*
 * Inspired by https://doc.qt.io/qt-5.10/qtcharts-boxplotchart-example.html
 * */

Boxplot::Boxplot(std::map<std::string, std::vector<unsigned int>>& results) : results(results) {}

double Boxplot::FindMedian(unsigned long begin, unsigned long end, std::string testcase)
{
        std::sort(results[testcase].begin(), results[testcase].end());
        unsigned long count = end - begin;
        if (count % 2) {
                return results[testcase].at(count / 2 + begin);
        } else {
                double right = results[testcase].at(count / 2 + begin);
                double left  = results[testcase].at(count / 2 - 1 + begin);
                return (right + left) / 2.0;
        }
}

void Boxplot::WriteToFile() { GeneratePlots(true); }

void Boxplot::Display() { GeneratePlots(false); }

void Boxplot::GeneratePlots(bool write)
{
        for (auto testcasepair : results) {
                if (testcasepair.second.size() <= 1) {
                        std::cerr << "Warning: skipping generating the boxplot for " << testcasepair.first << std::endl;
                        continue;
                }
                int           argc = 0;
                QApplication  a(argc, nullptr);
                auto*         boxplot   = new QBoxPlotSeries();
                unsigned long count     = results[testcasepair.first].size();
                auto*         box       = new QBoxSet(testcasepair.first.c_str());
                unsigned int  min_value = *std::min_element(testcasepair.second.begin(), testcasepair.second.end());
                unsigned int  max_value = *std::max_element(testcasepair.second.begin(), testcasepair.second.end());

                box->setValue(QBoxSet::LowerExtreme, min_value);
                box->setValue(QBoxSet::UpperExtreme, max_value);
                box->setValue(QBoxSet::Median, FindMedian(0, count, testcasepair.first));
                box->setValue(QBoxSet::LowerQuartile,
                              FindMedian(0, static_cast<unsigned long>(count / 2.0), testcasepair.first));
                box->setValue(QBoxSet::UpperQuartile, FindMedian(static_cast<unsigned long>(count / 2.0 + (count % 2)),
                                                                 count, testcasepair.first));
                boxplot->append(box);

                QChart* chart = new QChart();
                chart->addSeries(boxplot);
                chart->setTitle("Boxplot of results");
                chart->createDefaultAxes();
                chart->axisY()->setMin(min_value * 0.99);
                chart->axisY()->setMax(max_value * 1.01);
                chart->legend()->setVisible(true);
                chart->legend()->setAlignment(Qt::AlignBottom);

                auto* chartView = new QChartView(chart);
                chartView->setRenderHint(QPainter::Antialiasing);

                QMainWindow window;
                window.setCentralWidget(chartView);
                if (write) {
                        window.grab().save((testcasepair.first + ".png").c_str());
                } else {
                        window.resize(800, 600);
                        window.show();
                        QApplication::exec();
                }
        }
}

} // namespace calibration
