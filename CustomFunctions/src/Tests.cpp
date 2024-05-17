#include "..\inc\Tests.h"
using namespace std;

namespace Tests {
    void TimeTestsStartTest(wchar_t* testname)
    {
        timetesttemp[*testname] = DateTime::TimestampMillisecond();
    }

    uint64_t TimeTestsEndTest(wchar_t* testname)
    {
        return DateTime::TimestampMillisecond()-timetesttemp[*testname];
    }

    wstring TimeTestsEndTestString(wchar_t* testname)
    {
        if (timetesttemp.find(*testname) == timetesttemp.end()) {
            //SFT(L"AndromedaUtilities.dll Error! Not find test named " + wstring(testname));
            return L"Test Failed";
        }

        double msec = TimeTestsEndTest(testname);
        wstring ret = L"AndromedaUtilities.dll Test " + wstring(testname) + L" completed after " + to_wstring(int(msec)) + L" msec - " + to_wstring(msec / 1000) + L" sec";
        timetesttemp.erase(*testname);
        return ret;
    }
    const wchar_t* TimeTestsEndTestChar(wchar_t* testname)
    {
        return TimeTestsEndTestString(testname).c_str();
    }
};

