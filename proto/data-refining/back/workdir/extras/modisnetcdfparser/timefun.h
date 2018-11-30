#ifndef TIMEFUN_H
#define TIMEFUN_H

#include <cstdio>
#include <string>
#include "stringsplit.h"

const short hoursPerMonth[12] = { 31*24, 28*24, 31*24, 30*24,  31*24,  30*24,  31*24,  31*24,  30*24,  31*24,  30*24,  31*24};
const short hoursUpToMonth[12] =    { 0, 31*24, 59*24, 90*24, 120*24, 151*24, 181*24, 212*24, 243*24, 273*24, 304*24, 334*24};

const short hoursPerMonthLeap[12] = { 31*24, 29*24, 31*24, 30*24,  31*24,  30*24,  31*24,  31*24,  30*24,  31*24,  30*24,  31*24};
const short hoursUpToMonthLeap[12] =    { 0, 31*24, 60*24, 91*24, 121*24, 152*24, 182*24, 213*24, 244*24, 274*24, 305*24, 335*24};

static int asciiToHoursSince1900(const std::string &ascii)
{
    // T1 T2  T3  T4 T5 T6
    // DD.MM.YYYY*HH:MM:SS - V1
    // YYYY.MM.DD*HH:MM:SS - V2 - preferred
    if (ascii.size() < 19)
        return -1;

    int year, month, day, hour, minute, second;

    if ((6 != sscanf(ascii.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second))
        &&(6 != sscanf(ascii.c_str(), "%d.%d.%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second))
        &&(6 != sscanf(ascii.c_str(), "%*s %d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second))
        &&(6 != sscanf(ascii.c_str(), "%*s %d.%d.%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second)))
        return -1;
    if (day > 31)
    {
        if (year <= 31)
            std::swap(year, day);
        else
            return -1;
    }
    year -= 1900;
    if (year < 0)
        return -1;

    constexpr int hoursPerYear = 24*365;
    constexpr int hoursPerLeapYear = 24*366;
    constexpr int hoursPerFourYears = hoursPerYear * 3 + hoursPerLeapYear;

    int total = 0;
    total += (year / 4) * hoursPerFourYears;
    year = year % 4;

    total += (year > 0)|(month > 2) ? 24 : 0; // extra feb day
    total += year*hoursPerYear;
    total += hoursUpToMonth[month-1];
    total += (day-1)*24;
    total += hour;
    return total;
}

static std::string hoursSince1900ToAscii(int hour)
{
    if (hour < 0)
        return "undefined";
    constexpr int hoursPerYear = 24*365;
    //constexpr int64_t hoursPerLeapYear = hoursPerYear + 24;
    constexpr int hoursPerFourYears = 4*hoursPerYear + 24;
    constexpr int hoursUntil29Feb = (31 + 28)*24;
	
    int day = 1;
    int month = 1;
    int year = 1900 + 4 * (hour / hoursPerFourYears);
	hour %= hoursPerFourYears;
	
    const int leaped = hour > hoursUntil29Feb ? 24 : 0;
	hour -= leaped;
	
    int dyear = hour / hoursPerYear;

    const short *pHoursUpToMonth = dyear==0 ? hoursUpToMonthLeap : hoursUpToMonth;
    const short *pHoursPerMonth = dyear==0 ? hoursPerMonthLeap : hoursPerMonth;

    year += dyear;
    hour %= hoursPerYear;
    hour += dyear ? 0 : leaped;
	
	while (month < 12)
	{
        if (hour < pHoursUpToMonth[month])
			break;
		++month;
	}
	
    hour -= pHoursUpToMonth[month-1];
	
    if (hour >= (pHoursPerMonth[month - 1]))
	{
		month += 1;
        hour %= 24;
	}
    else
    {
        day += (hour / 24);
        hour %= 24;
    }
    const char prefix0[] = "-0";
    const char prefix1[] = "-";
    const char prefix2[] = " 0";
    const char prefix3[] = " ";
    return std::to_string(year)
           +((month<10)?prefix0:prefix1)+std::to_string(month)
           +((day<10)?prefix0:prefix1)+std::to_string(day)
           +((hour<10)?prefix2:prefix3)+std::to_string(hour)+":00:00";
}

static int hoursSince1950ToHoursSince1900(int h1900)
{
    // time is given in hours since 1950-01-01 00:00:00
    // need to convert to hours since 1900-01-01 00:00:00
    constexpr int hoursPerYear = 365*24;
    constexpr int hoursPerLeap = 24; // 29 feb
    // first leap year 1900, last - 1948 -> 13 leap years
    constexpr int delta = (1950-1900)*hoursPerYear + 13*hoursPerLeap;
    return h1900 + delta;
}

static int hoursSince1970ToHoursSince1900(int h1900)
{
    // time is given in hours since 1950-01-01 00:00:00
    // need to convert to hours since 1900-01-01 00:00:00
    constexpr int hoursPerYear = 365*24;
    constexpr int hoursPerLeap = 24; // 29 feb
    // first leap year 1900, last - 1968 -> 18 leap years
    constexpr int delta = (1970-1900)*hoursPerYear + 18*hoursPerLeap;
    return h1900 + delta;
}

#endif // TIMEFUN_H
