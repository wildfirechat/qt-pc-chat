#include "timeformatter.h"
#include <QDateTime>

TimeFormatter::TimeFormatter() {}

QString TimeFormatter::formatMilliseconds(int64_t milliseconds)
{
    if(milliseconds <= 0) {
        return "";
    }
    QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(milliseconds);
    QDateTime now = QDateTime::currentDateTime();
    QDate date = timestamp.date();
    QDate currentDate = now.date();

    // 计算差值
    int days = currentDate.daysTo(date);

    if (days == 0) {
        // 今天: HH:MM
        return timestamp.toString("HH:mm");
    } else if (days == -1) {
        // 昨天: 昨天 HH:MM
        return QString("昨天 %1").arg(timestamp.toString("HH:mm"));
    } else if (days > -7 && days < 0) {
        // 本周: 星期 HH:MM
        QString weekDay;
        switch (date.dayOfWeek()) {
        case 1: weekDay = "周一"; break;
        case 2: weekDay = "周二"; break;
        case 3: weekDay = "周三"; break;
        case 4: weekDay = "周四"; break;
        case 5: weekDay = "周五"; break;
        case 6: weekDay = "周六"; break;
        case 7: weekDay = "周日"; break;
        default: weekDay = "";
        }
        // return QString("%1 %2").arg(weekDay).arg(timestamp.toString("HH:mm"));
        return weekDay;
    } else if (date.year() == currentDate.year()) {
        // 今年: 月/日
        return timestamp.toString("M/d");
    } else {
        // 往年: 年 月/日
        return timestamp.toString("yyyy M/d");
    }
}

QString TimeFormatter::formatDetailTime(int64_t milliseconds)
{
    QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(milliseconds);
    QDateTime now = QDateTime::currentDateTime();
    QDate date = timestamp.date();
    QDate currentDate = now.date();

    // 计算差值
    int days = currentDate.daysTo(date);

    if (days == 0) {
        // 今天: HH:MM
        return timestamp.toString("HH:mm");
    } else if (days == -1) {
        // 昨天: 昨天 HH:MM
        return QString("昨天 %1").arg(timestamp.toString("HH:mm"));
    } else if (days > -7 && days < 0) {
        // 本周: 星期 HH:MM
        QString weekDay;
        switch (date.dayOfWeek()) {
        case 1: weekDay = "周一"; break;
        case 2: weekDay = "周二"; break;
        case 3: weekDay = "周三"; break;
        case 4: weekDay = "周四"; break;
        case 5: weekDay = "周五"; break;
        case 6: weekDay = "周六"; break;
        case 7: weekDay = "周日"; break;
        default: weekDay = "";
        }
        return QString("%1 %2").arg(weekDay).arg(timestamp.toString("HH:mm"));
    } else if (date.year() == currentDate.year() && date.month() == currentDate.month()) {
        // 今年: 月/日
        return timestamp.toString("d HH:mm");
    } else if (date.year() == currentDate.year()) {
        // 今年: 月/日
        return timestamp.toString("M/d HH:mm");
    } else {
        // 往年: 年 月/日
        return timestamp.toString("yyyy M/d HH:mm");
    }
}
