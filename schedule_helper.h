// schedule_helper.h
#pragma once

#include "esphome.h"
using namespace esphome;

// True if `now_mins` lies inside [start_mins, end_mins), wrap-around safe
// (start after end = window crosses midnight).
inline bool in_time_window(int now_mins, int start_minute_of_day, int end_minute_of_day) {
    if (start_minute_of_day <= end_minute_of_day)
        return now_mins >= start_minute_of_day && now_mins < end_minute_of_day;
    return now_mins >= start_minute_of_day || now_mins < end_minute_of_day;
}
