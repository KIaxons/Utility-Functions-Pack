#pragma once
#include <chrono>
#include <string>

namespace DateTime {
    int GetNowYear();
    int GetNowMonth();
    int GetNowDays();
    int GetNowHours();

    uint32_t Timestamp();
    uint64_t TimestampMillisecond();
    const wchar_t* TimestampChar();
    const wchar_t* TimestampMillisecondChar();
};