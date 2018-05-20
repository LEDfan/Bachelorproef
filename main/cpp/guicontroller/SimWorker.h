#pragma once

#include <QtCore/QObject>
#include <memory>
#include <sim/SimRunner.h>

namespace stride {

class SimWorker : public QObject
{
        Q_OBJECT
public:
        SimWorker(std::shared_ptr<SimRunner> runner);
public slots:
        void Work();
signals:
        void Worked(unsigned int infectedCount);

private:
        std::shared_ptr<SimRunner> m_runner;
};

} // namespace stride
