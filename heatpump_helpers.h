// heatpump_helpers.h
#pragma once

#include <cmath>
#include <cstdint>

// Both timestamps use the same millis() clock; positive flow is evidence, not a physical interlock.
inline bool heatpump_flow_is_confirmed(float flow, bool received, uint32_t now_ms, uint32_t last_update_ms) {
    return received && std::isfinite(flow) && flow > 0.0f &&
           static_cast<uint32_t>(now_ms - last_update_ms) <= 75000U;
}
