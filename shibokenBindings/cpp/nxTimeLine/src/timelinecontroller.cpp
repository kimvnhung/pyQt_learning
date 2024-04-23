#include "timelinecontroller.h"

class TimeLineController::Private {
public:
    Private()
    {

    }


};

TimeLineController::TimeLineController():
    d(new Private())
{

}

TimeLineController::TimeLineController(const TimeLineController &other):
    d(other.d)
{

}

TimeLineController& TimeLineController::operator=(const TimeLineController &other)
{
    d = other.d;
    return *this;
}

TimeLineController::TimeLineController(TimeLineController &&other):
    d(other.d)
{

}

TimeLineController& TimeLineController::operator=(TimeLineController &&other)
{
    d = other.d;
    return *this;
}

TimeLineController::~TimeLineController()
{
    d.clear();
}

void TimeLineController::initData(qint64 duration)
{

}

void TimeLineController::initData(qint64 endTime, qint64 duration)
{

}

QWidget* TimeLineController::widget()
{
    return NULL;
}

//slots
void TimeLineController::setPosition()
{

}

void TimeLineController::play()
{

}

void TimeLineController::pause()
{

}

void TimeLineController::resume()
{

}
