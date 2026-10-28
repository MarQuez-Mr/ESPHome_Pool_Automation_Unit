// plp_helper.h
#pragma once

#include "esphome.h"
using namespace esphome;

// Single source of truth for pool light (PLP) scene names.
// Order must match the `options:` list of select `plp_scene_select` in pau.yml.
constexpr int PLP_SCENE_COUNT = 14;
const char *const plp_scenes[PLP_SCENE_COUNT] = {
    "Deep Blue Sea",      // PS01
    "Daylight Blue",      // PS02
    "Sunset",             // PS03
    "Love Is In The Air", // PS04
    "Forest Spring",      // PS05
    "Think Pink",         // PS06
    "Sunshine",           // PS07
    "Jungle Beach",       // PS08
    "Voodoo",             // PS09
    "Disco",              // PS10
    "Party",              // PS11
    "Warm White",         // PS12
    "Neutral White",      // PS13
    "Sun White"           // PS14
};

// scene index 0..13 -> "PS01".."PS14". Static buffer: single-threaded call sites.
const char *plp_scene_cmd(int idx) {
    static char cmd[5];
    if (idx < 0)
        idx = 0;
    if (idx >= PLP_SCENE_COUNT)
        idx = PLP_SCENE_COUNT - 1;
    snprintf(cmd, sizeof(cmd), "PS%02d", idx + 1);
    return cmd;
}

// (idx + delta) with wraparound over PLP_SCENE_COUNT
int plp_scene_wrap(int idx, int delta) {
    int next = idx + delta;
    if (next < 0)
        next = PLP_SCENE_COUNT - 1;
    if (next >= PLP_SCENE_COUNT)
        next = 0;
    return next;
}
