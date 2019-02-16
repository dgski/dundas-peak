#include <string>
#include <iomanip>
#include <sstream>
#include <functional>

#define DEFAULT_DATETIME_FORMAT "%Y/%m/%d %H:%M:%S"

class SimpleTime
{
    tm t;

public:
    SimpleTime() : t()
    {}

    template <class T>
    SimpleTime(T timeString, const char* fmt)
    {
        set(timeString, fmt);
    }

    template <class T>
    void set(T timeString, const char* fmt)
    {
        stringstream ss {timeString};
        ss >> get_time(&t, fmt);
    }

    const tm& getTimeStruct() const
    {
        return t;
    }

    string toString(const char* fmt) const
    {
        stringstream ss;
        ss << put_time(&t, fmt);
        return ss.str();
    }

    bool compare(const SimpleTime& other, function<bool(int,int)> func, bool caseRetVal) const
    {
        const tm& o = other.getTimeStruct();

        if(func(t.tm_year, o.tm_year))      return caseRetVal;
        if(func(t.tm_mon, o.tm_mon))        return caseRetVal;
        if(func(t.tm_mday, o.tm_mday))      return caseRetVal;
        if(func(t.tm_hour, o.tm_hour))      return caseRetVal;
        if(func(t.tm_min, o.tm_min))        return caseRetVal;
        if(func(t.tm_sec, o.tm_sec))        return caseRetVal;

        return !caseRetVal;
    }

    bool isEqual(const SimpleTime& other) const
    {
        return compare(other, [](auto a, auto b)
        {
            return a != b;
        }, false);
    }

    bool isBefore(const SimpleTime& other) const
    {
        return compare(other, [](auto a, auto b)
        {
            return a < b;
        }, true);
    }

    bool isAfter(const SimpleTime& other) const
    {
        return compare(other, [](auto a, auto b)
        {
            return a > b;
        }, true);
    }
};

static bool operator< (const SimpleTime& a, const SimpleTime& b)
{
    return a.isBefore(b);
}

#pragma once