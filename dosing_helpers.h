// dosing_helpers.h
#pragma once

#include <cmath>
#include <cstdint>

// Installation-selected minimum circulation time after a chemical pump stops.
inline constexpr uint32_t CHEMICAL_FLUSH_MS = 60U * 1000U;

// Hard upper limit shared by dosing entry checks and the active-dose watchdog.
inline constexpr float DOSING_MAX_PH = 8.5f;

// Boot-relative millisecond timestamps are never persisted across restarts.
inline constexpr uint64_t DOSING_NO_TIMESTAMP = UINT64_MAX;
inline constexpr uint64_t DOSING_SHARED_GAP_MS = 60ULL * 1000ULL;
inline constexpr uint64_t DOSING_CALIBRATION_HOLDOFF_MS = 600ULL * 1000ULL;
inline constexpr uint64_t ACID_DOSING_COOLDOWN_MS = 3600ULL * 1000ULL;
inline constexpr uint64_t CHLORINE_DOSING_COOLDOWN_MS = 1800ULL * 1000ULL;
inline constexpr uint64_t ACID_STARTUP_LOCKOUT_MS = 3600ULL * 1000ULL;
inline constexpr uint64_t CHLORINE_STARTUP_LOCKOUT_MS = 1800ULL * 1000ULL;

inline bool dosing_wait_elapsed(uint64_t now_ms, uint64_t event_ms, uint64_t wait_ms) {
    return event_ms == DOSING_NO_TIMESTAMP ||
           (now_ms >= event_ms && now_ms - event_ms >= wait_ms);
}

inline uint64_t dosing_latest_event(uint64_t first_ms, uint64_t second_ms) {
    if (first_ms == DOSING_NO_TIMESTAMP)
        return second_ms;
    if (second_ms == DOSING_NO_TIMESTAMP)
        return first_ms;
    return first_ms > second_ms ? first_ms : second_ms;
}

// Use publication age, not polling requests, to validate circulation evidence.
inline bool dosing_flow_is_safe(float flow, bool received, uint32_t now_ms, uint32_t last_update_ms) {
    return received && std::isfinite(flow) && flow > 0.0f &&
           static_cast<uint32_t>(now_ms - last_update_ms) <= 75000U;
}

// Raw temperature freshness is independent of the pool-temperature display average.
inline bool pool_temperature_is_fresh(float temperature, bool received, uint32_t now_ms, uint32_t last_update_ms) {
    return received && std::isfinite(temperature) && temperature >= -25.0f && temperature <= 75.0f &&
           static_cast<uint32_t>(now_ms - last_update_ms) <= 75000U;
}
