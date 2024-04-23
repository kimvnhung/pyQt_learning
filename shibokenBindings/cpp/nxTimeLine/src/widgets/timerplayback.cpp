#include "timerplayback.h"

#include <chrono>
#include <QQuickItem>

class TimerPlayback::Private{
public:
    Private(TimerPlayback *owner):
        owner(owner)
    {

    }

    TimerPlayback *owner;

    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};

    double mouseX;
    RulerContext* ctx;
    QVector<TimeStep*> timeSteps;


};

TimerPlayback::TimerPlayback(QObject *parent, bool isInit):
    base_type(QUrl(),parent)
{

}

TimerPlayback::~TimerPlayback()
{

}

void TimerPlayback::setDuration(std::chrono::milliseconds duration)
{

}

QQmlListProperty<TimeStep> TimerPlayback::timeSteps()
{
    return QQmlListProperty<TimeStep>(this,&d->timeSteps);
}

double TimerPlayback::mouseX() const
{
    return d->mouseX;
}

void TimerPlayback::setMouseX(double value)
{

}

double TimerPlayback::ruleWidth() const
{
    return d->ctx->width();
}

void TimerPlayback::setRuleWidth(double value)
{

}

double TimerPlayback::viewWidth() const
{
    return d->ctx->visibleWidth();
}

void TimerPlayback::setViewWidth(double value)
{

}

double TimerPlayback::viewX() const
{
    return d->ctx->x();
}

void TimerPlayback::setViewX(double value)
{

}

QQuickWidget *TimerPlayback::widget() const
{
    return d->widget.get();
}

bool TimerPlayback::isMaximumScale() const
{
    return false;
}

//protected
void TimerPlayback::registerQmlType()
{

}

void TimerPlayback::setUpQmlUrl(const QUrl &component)
{

}

QObject* TimerPlayback::rootObject() const
{
    return d->widget.get()->rootObject();
}



