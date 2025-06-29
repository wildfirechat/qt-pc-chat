#ifndef TIMEFORMATTER_H
#define TIMEFORMATTER_H

#include <QObject>
class TimeFormatter
{
public:
    TimeFormatter();
    static QString formatMilliseconds(int64_t milliseconds);

    static QString formatDetailTime(int64_t milliseconds);
};

#endif // TIMEFORMATTER_H
