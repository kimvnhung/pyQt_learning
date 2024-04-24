#include "rulercontext.h"

#include <QDateTime>
#include <QDebug>
#include <QTimer>

qint64 DELAY_FOR_UPDATE_X = 100; // ms
double CHIGHEST_VISABLE_W = 250;
double CMAXIMUM_SCALE_W = 800;
qint64 CMS_LEVELS[] = {
    5000,     // 5s
    10000,    // 10s
    30000,    // 30s
    60000,    // 1min
    300000,   // 5min
    600000,   // 10min
    1800000,  // 30min
    3600000,  // 1h
    10800000, // 3h
    43200000, // 12h
    86400000, // 1d
    604800000 // 1w
};

int CMS_LEVELS_SIZE = sizeof(CMS_LEVELS) / sizeof(qint64);

int CDELEGATE_STATES[][3] = {
    {2, 5, 10}, // 1s
    {2, 3, 2},  // 1any
    {5, 2, 5},  // 5s
    {5, 2, 3},  // 5min
    {2, 5, 2},  // 10any
    {3, 2, 5},  // 30any
    {3, 2, 3},  // 3h
    {4, 3, 2},  // 12h
    {2, 4, 3},  // 1d
    {7, 2, 4}   // 1w
};

RulerContext::RulerContext(QObject *parent, qint64 totalTime, qint64 highestUnit, double width, double x) : QObject{parent},
                                                                                                            m_totalTime(totalTime),
                                                                                                            m_highestUnit(highestUnit),
                                                                                                            m_width(width),
                                                                                                            m_x(x),
                                                                                                            m_position(0),
                                                                                                            m_delayTimer(new QTimer(this))
{
    connect(this, &RulerContext::totalTimeChanged, this, &RulerContext::contextChanged);
    connect(this, &RulerContext::visibleWidthChanged, this, &RulerContext::contextChanged);
    connect(this, &RulerContext::xChanged, this, &RulerContext::contextChanged);
    connect(this, &RulerContext::widthChanged, this, &RulerContext::contextChanged);

    m_delayTimer->setInterval(DELAY_FOR_UPDATE_X);
    connect(m_delayTimer, &QTimer::timeout, [this]
            {
        emit widthChanged();
        emit xChanged();
        emit widthPerMiliChanged(); });
}

RulerContext::~RulerContext()
{
    if (m_delayTimer->isActive())
        m_delayTimer->stop();

    delete m_delayTimer;
}

double RulerContext::widthPerMili() const
{
    return m_widthPerMili;
}

void RulerContext::setWidthPerMili(double value)
{
    qDebug() << __FUNCTION__ << "value: " << value << "toalTime:" << m_totalTime << "width" << m_width;
    if (m_widthPerMili == value)
        return;

    m_widthPerMili = value;
    updateUnits();
    refreshVisibleRange();
    if (!m_delayTimer->isActive())
        m_delayTimer->start();
}

qint64 RulerContext::totalTime() const
{
    return m_totalTime;
}

bool RulerContext::setTotalTime(qint64 newTotalTime)
{
    qDebug() << __FUNCTION__ << "newTotalTime:" << newTotalTime;
    if (m_totalTime == newTotalTime)
        return false;

    m_totalTime = newTotalTime;
    setWidthPerMili(m_width / m_totalTime);
    updateUnits();
    emit totalTimeChanged();
    return true;
}

double RulerContext::width() const
{
    return m_width;
}

void RulerContext::setWidth(double newWidth)
{
    qDebug() << __FUNCTION__ << "newWidth: " << newWidth;
    if (m_width == newWidth)
        return;

    m_width = newWidth;
    if (!m_delayTimer->isActive())
        m_delayTimer->start();
    setWidthPerMili(newWidth / m_totalTime);
}

double RulerContext::x() const
{
    return m_x;
}

void RulerContext::setX(double newX)
{
    qDebug() << __FUNCTION__ << "newX:" << newX;
    if (m_x == newX)
        return;

    m_x = newX;
    refreshVisibleRange();
    if (!m_delayTimer->isActive())
        m_delayTimer->start();
}

qint64 RulerContext::normalUnit() const
{
    return m_normalUnit;
}

void RulerContext::setNormalUnit(qint64 newNormalUnit)
{
    m_normalUnit = newNormalUnit;
}

qint64 RulerContext::smallUnit() const
{
    return m_smallUnit;
}

void RulerContext::setSmallUnit(qint64 newSmallUnit)
{
    m_smallUnit = newSmallUnit;
}

qint64 RulerContext::smallestUnit() const
{
    return m_smallestUnit;
}

void RulerContext::setSmallestUnit(qint64 newSmallestUnit)
{
    m_smallestUnit = newSmallestUnit;
}

qint64 RulerContext::highestUnit() const
{
    return m_highestUnit;
}

void RulerContext::setHighestUnit(qint64 newHighestUnit)
{
    qDebug() << __FUNCTION__ << "newHighestUnit:" << newHighestUnit;
    m_highestUnit = newHighestUnit;
    refreshVisibleRange();
}

double RulerContext::visibleWidth() const
{
    return m_visibleWidth;
}

bool RulerContext::isRoundedBy(qint64 target, qint64 unit)
{
    return target % unit == 0;
}

qint64 RulerContext::roundedBy(qint64 target, qint64 unit)
{
    return target - (target % unit);
}

void RulerContext::setVisibleWidth(double newVisibleWidth)
{
    if (m_visibleWidth == newVisibleWidth)
        return;

    m_visibleWidth = newVisibleWidth;
    CHIGHEST_VISABLE_W = (int)(m_visibleWidth / 4);
    qDebug() << "CHIGHIEST_VISIBLE_W" << CHIGHEST_VISABLE_W;
    refreshVisibleRange();
    emit visibleWidthChanged();
}

void RulerContext::refreshVisibleRange()
{
    if (widthPerMili() == 0 || highestUnit() == 0)
        return;

    qint64 startedValue = roundedBy(abs(m_x) / widthPerMili(), highestUnit());
    qint64 stopedValue = roundedBy((abs(m_x) + m_visibleWidth) / widthPerMili(), highestUnit()) + highestUnit();
    if (startedValue != m_visibleRange[0] || stopedValue != m_visibleRange[1])
    {
        m_visibleRange[0] = startedValue;
        m_visibleRange[1] = stopedValue;
        emit visibleRangeChanged();
    }
    m_absoluteVisibleRange[0] = abs(m_x) / widthPerMili();
    m_absoluteVisibleRange[1] = (abs(m_x) + m_visibleWidth) / widthPerMili() + 1;
    qDebug() << "visible range: " << m_visibleRange[0] << m_visibleRange[1] << "x: " << m_x << "width: " << m_width;
}

bool RulerContext::isVisible(qint64 value)
{
    return m_visibleRange[0] <= value && value <= m_visibleRange[1];
}

double RulerContext::relativeWidth(qint64 value)
{
    if (value < m_visibleRange[0] || value > m_visibleRange[1])
        return 0;

    if (value - m_visibleRange[0] < highestUnit())
        return (value - m_absoluteVisibleRange[0]) * widthPerMili();

    if (m_visibleRange[1] - value < highestUnit())
        return (m_absoluteVisibleRange[1] - value) * widthPerMili();

    return highestUnit() * widthPerMili();
}

double RulerContext::relativePosition()
{
    if (m_position <= m_absoluteVisibleRange[0] || m_absoluteVisibleRange[1] <= m_position)
        return 0;

    return (m_position - m_absoluteVisibleRange[0]) * widthPerMili();
}

qint64 RulerContext::startValueByUnit(qint64 unit)
{
    return roundedBy(abs(m_x) / widthPerMili(), unit);
}

qint64 RulerContext::stopValueByUnit(qint64 unit)
{
    return roundedBy((abs(m_x) + m_visibleWidth) / widthPerMili(), unit) + unit;
}

qint64 RulerContext::absoluteStart() const
{
    return m_absoluteVisibleRange[0];
}

qint64 RulerContext::absoluteStop() const
{
    return m_absoluteVisibleRange[1];
}

double RulerContext::startPosition() const
{
    return m_visibleRange[0] * widthPerMili();
}

double RulerContext::stopPosition() const
{
    return m_visibleRange[1] * widthPerMili();
}

bool RulerContext::isMaximumScale() const
{
    return m_highestUnit == CMS_LEVELS[0] && m_highestUnit * m_widthPerMili > CMAXIMUM_SCALE_W;
}

bool RulerContext::setPosition(qint64 position)
{
    if (m_position == position)
        return false;

    if (0 <= position && position <= m_totalTime)
        m_position = position;
    else if (position < 0)
        m_position = 0;
    else
        m_position = m_totalTime;

    return true;
}

qint64 RulerContext::position()
{
    return m_position;
}

void RulerContext::updateUnits()
{
    qDebug() << __FUNCTION__;
    if (m_widthPerMili == 0)
        return;

    for (int i = 0; i < CMS_LEVELS_SIZE - 1; i++)
    {
        qDebug() << __FUNCTION__ << "widthPerMili*CMS_LEVE" << m_widthPerMili * CMS_LEVELS[i];
        if (m_widthPerMili * CMS_LEVELS[i] < CHIGHEST_VISABLE_W && CHIGHEST_VISABLE_W < m_widthPerMili * CMS_LEVELS[i + 1])
        {
            setHighestUnit(CMS_LEVELS[i + 1]);
            break;
        }

        if (i == CMS_LEVELS_SIZE - 2)
        {
            if (m_widthPerMili * CMS_LEVELS[0] > CHIGHEST_VISABLE_W)
                setHighestUnit(CMS_LEVELS[0]);
            else
                setHighestUnit(CMS_LEVELS[CMS_LEVELS_SIZE - 1]);
        }
    }
    qDebug() << __FUNCTION__ << "highestUnitWidth:" << (m_highestUnit * m_widthPerMili);

    int *delegateState;

    int secs = m_highestUnit / 1000;
    int mins = m_highestUnit / 60000;
    int hours = m_highestUnit / (3600000);
    int ds = m_highestUnit / (86400000);
    int ws = m_highestUnit / (604800000);

    if (secs == 1)
    {
        delegateState = CDELEGATE_STATES[0];
    }
    else if (secs == 5)
    {
        delegateState = CDELEGATE_STATES[2];
    }
    else if (mins == 5)
    {
        delegateState = CDELEGATE_STATES[3];
    }
    else if (hours == 3)
    {
        delegateState = CDELEGATE_STATES[6];
    }
    else if (secs == 10 || mins == 10 || hours == 10)
    {
        delegateState = CDELEGATE_STATES[4];
    }
    else if (secs == 30 || mins == 30 || hours == 30)
    {
        delegateState = CDELEGATE_STATES[5];
    }
    else if (mins == 1 || hours == 1)
    {
        delegateState = CDELEGATE_STATES[1];
    }
    else if (hours == 12)
    {
        delegateState = CDELEGATE_STATES[7];
    }
    else if (ds == 1)
    {
        delegateState = CDELEGATE_STATES[8];
    }
    else if (ws == 1)
    {
        delegateState = CDELEGATE_STATES[9];
    }

    int delegate0 = *(delegateState);
    int delegate1 = *(delegateState + 1);
    int delegate2 = *(delegateState + 2);

    setSmallestUnit(m_highestUnit / (delegate0 * delegate1 * delegate2));
    setSmallUnit(m_highestUnit / (delegate0 * delegate1));
    setNormalUnit(m_highestUnit / (delegate0));

    emit unitsChanged();
}
