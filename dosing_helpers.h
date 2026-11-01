// dosing_helpers.h
#pragma once

#include <cmath>
#include <cstdint>

// Installation-selected minimum circulation time after a chemical pump stops.
inline constexpr uint32_t CHEMICAL_FLUSH_MS = 60U * 1000U;

// Use publication age, not polling requests, to validate circulation evidence.
inline bool dosing_flow_is_safe(float flow, bool received, uint32_t now_ms, uint32_t last_update_ms) {
    return received && std::isfinite(flow) && flow > 0.0f &&
           static_cast<uint32_t>(now_ms - last_update_ms) <= 75000U;
}
