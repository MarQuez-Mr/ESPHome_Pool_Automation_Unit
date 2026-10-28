// display_helpers.h
#pragma once

#include "esphome.h"
using namespace esphome;

// app_flow states
// 0 screen blanked, 1 default/home, 2 pH calibration, 3 Rx calibration, 4 boost
#define APP_FLOW_OFF        0
#define APP_FLOW_HOME       1
#define APP_FLOW_CAL_PH     2
#define APP_FLOW_CAL_RX     3
#define APP_FLOW_BOOST      4

// Now taking 'it' as a reference
void draw_wifi_signal_strength(Display &it) {
    int wifi_signal_strength = id(wifistep).state;
    if (wifi_signal_strength == 0) {
        wifi_signal_strength = round((id(wifisignal).state + 100) / 10);
    }

    for (int i = 0; i < 5; i++) {
        int wbx = 210 + (i * 5);
        int wby = 16 - (i * 2);
        int wbw = 5;
        int wbh = (i * 2) + wbw;

        if (i < wifi_signal_strength)
            it.filled_rectangle(wbx, wby, wbw, wbh);
        else
            it.rectangle(wbx, wby, wbw, wbh);
    }
}

void draw_header(Display &it, const std::string &title) {
    it.rectangle(0, 0, it.get_width(), 30, id(my_blue));
    it.rectangle(0, 30, it.get_width(), 290, id(my_blue));
    it.print(4, 4, &id(mnuFont), id(my_white), TextAlign::TOP_LEFT, title.c_str());
}

void draw_datetime(Display &it) {
    auto time_now = id(current_time).now();
    if (time_now.is_valid()) {
        it.strftime(130, 4, &id(mnuFont), id(my_white), TextAlign::TOP_CENTER, "%H:%M:%S", time_now);
    }
}

void setup_page(Display &it) {
    draw_header(it, "P.A.U.");
    draw_datetime(it);
    draw_wifi_signal_strength(it);
}

void draw_back_button(Display &it) {
    it.image(140, 240, &id(back));
}

// setup_page + back button in the standard bottom-right position
void setup_page_back(Display &it) {
    setup_page(it);
    draw_back_button(it);
}

// Label at (6,y), value at (122,y); shows ": n/a" when value is NaN
void print_sensor_row(Display &it, int y, const char *label, float value, const char *fmt) {
    it.print(6, y, &id(mnuFont), label);
    if (isnan(value))
        it.print(122, y, &id(mnuFont), ": n/a");
    else
        it.printf(122, y, &id(mnuFont), fmt, value);
}

// Label at (6,y), ": On"/": Off" at (122,y)
void print_onoff_row(Display &it, int y, const char *label, bool on) {
    it.print(6, y, &id(mnuFont), label);
    it.printf(122, y, &id(mnuFont), ": %s", on ? "On" : "Off");
}

// Menu item: 40px-high rectangle with centered label
void draw_menu_item(Display &it, int x, int y, int w, const char *label) {
    it.rectangle(x, y, w, 40, id(my_blue));
    it.print(x + w / 2, y + 20, &id(mnuFont), TextAlign::CENTER, label);
}

// Calibration instruction screen: fixed text lines + checkbox + live value at (150,162)
void draw_calibration_page(Display &it, const char *first_line, float value) {
    it.print(10, 40, &id(txtFont), first_line);
    it.print(10, 60, &id(txtFont), "solution and press OK button");
    it.print(10, 80, &id(txtFont), "when value stabilizes.");
    it.image(20, 140, &id(checkbox));
    if (isnan(value))
        it.print(150, 162, &id(calFont), "n/a");
    else
        it.printf(150, 162, &id(calFont), "%.2f", value);
}

// Calibration completion screen: 2 fixed lines + checkbox
void draw_completion_page(Display &it) {
    it.print(10, 40, &id(txtFont), "Calibration completed!");
    it.print(10, 60, &id(txtFont), "Press OK to exit");
    it.image(20, 140, &id(checkbox));
}

bool is_nan(float value) {
    return isnan(value);
}
