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
