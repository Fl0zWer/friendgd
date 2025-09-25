#pragma once

#include <Geode/Geode.hpp>

namespace friendgd {
    inline constexpr const char* kEnableSetting = "enable-simulation";
    inline constexpr const char* kPartialSetting = "partial-mode";
    inline constexpr const char* kLogSetting = "log-errors";

    bool isSimulationEnabled();
    bool isPartialModeEnabled();
    bool shouldLogErrors();

    bool shouldSpoofFriend(bool originalValue);
    bool shouldSpoofMessageAccess(bool originalValue);

    template <class Fn>
    auto withErrorLogging(const char* context, Fn&& fn) -> decltype(fn()) {
        try {
            return fn();
        }
        catch (const std::exception& err) {
            if (shouldLogErrors()) {
                geode::log::error("[FriendGD] {}: {}", context, err.what());
            }
            throw;
        }
    }
}
