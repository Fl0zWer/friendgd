#include <Geode/Geode.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

GEODE_MOD_REGISTER();

class FriendGDMod final : public Mod {
protected:
    void onLoad() override {
        log::info("FriendGD loaded: everyone is now a friend!");
    }
};
