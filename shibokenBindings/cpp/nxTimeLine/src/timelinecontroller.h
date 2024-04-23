#ifndef TIMELINECONTROLLER_H
#define TIMELINECONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include "macros.h"

class BINDINGS_API TimeLineController : public QObject
{
    Q_OBJECT
public:
    TimeLineController();
    TimeLineController(const TimeLineController &other);
    TimeLineController& operator=(const TimeLineController &other);
    TimeLineController(TimeLineController &&other);
    TimeLineController& operator=(TimeLineController &&other);

    ~TimeLineController();

    void initData(qint64 duration);
    void initData(qint64 endTime, qint64 duration);

    QWidget* widget();

public Q_SLOTS:
    void setPosition();
    void play();
    void pause();
    void resume();

private:
    class Private;
    QSharedPointer<Private> d;
};

#endif // TIMELINECONTROLLER_H
