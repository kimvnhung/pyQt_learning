#include "timerplayback.h"

#include <QQuickItem>
#include <QQmlContext>
#include <QElapsedTimer>

class TimerPlayback::Private{
public:
    Private(TimerPlayback *owner):
        owner(owner),
        mouseX(0),
        ctx(new RulerContext(owner))
    {

    }

    TimerPlayback *owner;

    std::unique_ptr<QQuickWidget> widget{new QQuickWidget()};

    double mouseX;
    RulerContext* ctx;
    QVector<TimeStep*> timeSteps;

    void initTimeSteps();
    bool setMouseX(double value);
};

void TimerPlayback::Private::initTimeSteps()
{
    qDebug()<<__FUNCTION__;
    QElapsedTimer timer;
    qint64 start = timer.elapsed();
    if(ctx->width() == 0 || ctx->visibleWidth() == 0)
        return;


    if(timeSteps.empty() && ctx->highestUnit() != 0)
    {
        int highestCount = ctx->totalTime()/ctx->highestUnit();
        qDebug()<<__FUNCTION__<<"totalTime:"<<ctx->totalTime()<<"width:"<<ctx->width()<<"highestUnit:"<<ctx->highestUnit()<<"highestUnitWidth:"<<(ctx->highestUnit()*ctx->widthPerMili());
        qDebug()<<"highestCount : "<<highestCount;
        for(int i=0;i<highestCount+1;i++)
        {
            qint64 start2 = timer.elapsed();
            TimeStep *step = new TimeStep(ctx,i*ctx->highestUnit(),ctx->highestUnit(),TimeStep::EdgeType::HIGHEST);
            timeSteps.append(step);
            qDebug()<<"time to create one step: "<<(timer.elapsed()-start2);

        }
        emit owner->timeStepsChanged();
    }
    qDebug()<<"time to init step: "<<(timer.elapsed()-start);
}

bool TimerPlayback::Private::setMouseX(double value)
{
    if(mouseX == value)
        return false;

    mouseX = value;
    return true;
}

TimerPlayback::TimerPlayback(QObject *parent, bool isInit):
    base_type(QUrl("qrc:/qml/TimerPlayback.qml"),parent),
    d(new Private(this))
{
    if (isInit)
    {
        init();
    }

}

TimerPlayback::~TimerPlayback()
{
    d.clear();
}

void TimerPlayback::setDuration(qint64 duration)
{

    if(d->ctx->setTotalTime(duration))
    {
        d->timeSteps.clear();
        d->initTimeSteps();
    }
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
    if(d->setMouseX(value))
        emit mouseXChanged();

    emit hoverPositionChanged(d->ctx->positionFromMouseX(value));
}

double TimerPlayback::pressedX() const
{
    return d->ctx->relativePosition();
}

void TimerPlayback::setPressedX(double value)
{
    if(d->ctx->setPositionFromMouseX(value))
        emit positionChanged(d->ctx->position());
}

double TimerPlayback::ruleWidth() const
{
    return d->ctx->width();
}

void TimerPlayback::setRuleWidth(double value)
{
    d->ctx->setWidth(value);
}

double TimerPlayback::viewWidth() const
{
    return d->ctx->visibleWidth();
}

void TimerPlayback::setViewWidth(double value)
{
    qDebug()<<__FUNCTION__<<"viewWidth : "<<value;
    d->ctx->setVisibleWidth(value);
}

double TimerPlayback::viewX() const
{
    return d->ctx->x();
}

void TimerPlayback::setViewX(double value)
{
    d->ctx->setX(value);
}

QQuickWidget *TimerPlayback::widget() const
{
    return d->widget.get();
}

bool TimerPlayback::isMaximumScale() const
{
    return d->ctx->isMaximumScale();
}

void TimerPlayback::setPosition(qint64 position)
{
    if(d->ctx->setPosition(position))
        emit positionChanged(position);
}

QString convertTimeToString(qint64 milliseconds)
{
    qint64 seconds = milliseconds / 1000;
    qint64 days = seconds / (24 * 3600);
    seconds = seconds % (24 * 3600);
    qint64 hours = seconds / 3600;
    seconds %= 3600;
    qint64 minutes = seconds / 60;
    seconds %= 60;
    qint64 milliseconds_part = milliseconds % 1000;

    return QString("%1:%2:%3:%4:%5").arg(days, 2, 10, QChar('0'))
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'))
        .arg(milliseconds_part, 3, 10, QChar('0'));
}

QString TimerPlayback::position() const
{
    return convertTimeToString(d->ctx->position());
}

double TimerPlayback::relativePosition() const
{
    return d->ctx->relativePosition();
}

//protected
void TimerPlayback::registerQmlType()
{
    qmlRegisterType<RulerContext>("models",1,0,"RulerContext");
    qmlRegisterType<TimeStep>("models",1,0,"TimeStep");
    // qmlRegisterType<Ruler>("models", 1, 0, "Ruler");
    // qmlRegisterUncreatableType<TimeStep>("models", 1, 0, "RuleLine", "Cannot create RuleLine in QML");
    widget()->rootContext()->setContextProperty(QString("instance"), this);
}

void TimerPlayback::setUpQmlUrl(const QUrl &componentUrl)
{
    widget()->setObjectName("TimerPlayback");
    widget()->setResizeMode(QQuickWidget::SizeRootObjectToView);
    widget()->setSource(componentUrl);

    qDebug()<<__FUNCTION__<<__LINE__;
}

QObject* TimerPlayback::rootObject() const
{
    return d->widget.get()->rootObject();
}



