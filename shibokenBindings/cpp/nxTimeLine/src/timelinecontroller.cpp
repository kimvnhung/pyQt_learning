#include "timelinecontroller.h"
#include "widgets/timerplayback.h"

class TimeLineController::Private {
public:
    Private(TimeLineController *owner):
        owner(owner),
        timerPlayback(new TimerPlayback(owner))
    {

    }

    TimeLineController *owner;

    TimerPlayback *timerPlayback;
};

TimeLineController::TimeLineController():
    d(new Private(this))
{
    connect(d->timerPlayback, &TimerPlayback::positionChanged,this,&TimeLineController::positionChanged);
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
    d->timerPlayback->setDuration(duration);
}

void TimeLineController::initData(qint64 endTime, qint64 duration)
{

}

void TimeLineController::setMinimumSize(int w,int h)
{
    d->timerPlayback->widget()->setMinimumSize(QSize(w,h));
}

QWidget* TimeLineController::widget()
{
    return d->timerPlayback->widget();
}

//slots
void TimeLineController::setPosition(qint64 position)
{
    d->timerPlayback->setPosition(position);
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
