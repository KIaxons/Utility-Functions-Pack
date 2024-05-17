#include "..\inc\DateTime.h"

using namespace std;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

namespace DateTime {
    int GetNowYear()
    {
        time_t t = std::time(nullptr);
        tm* now = std::localtime(&t);
        return now->tm_year + 1900;
    }
    int GetNowMonth()
    {
        time_t t = std::time(nullptr);
        tm* now = std::localtime(&t);
        return now->tm_mon + 1;
    }
    int GetNowDays()
    {
        time_t t = std::time(nullptr);
        tm* now = std::localtime(&t);

        return now->tm_mday;
    }
    int GetNowHours()
    {
        time_t t = std::time(nullptr);
        tm* now = std::localtime(&t);

        return now->tm_hour;
    }
    uint32_t Timestamp()
    {
        uint32_t sec = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        return sec;
    }
    uint64_t TimestampMillisecond()
    {
        uint64_t millisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        return millisec;
    }
    const wchar_t* TimestampChar()
    {
        return to_wstring(DateTime::Timestamp()).c_str();
    }
    const wchar_t* TimestampMillisecondChar()
    {
        return to_wstring(DateTime::TimestampMillisecond()).c_str();
    }
};