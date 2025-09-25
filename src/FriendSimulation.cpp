#include "FriendSimulation.hpp"

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/GJFriendRequest.hpp>
#include <Geode/binding/GJProfilePage.hpp>
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/modify/GJAccountManager.hpp>
#include <Geode/modify/GJMessagePopup.hpp>
#include <Geode/modify/GJProfilePage.hpp>

using namespace geode::prelude;

namespace friendgd {
    bool isSimulationEnabled() {
        return Mod::get()->getSettingValue<bool>(kEnableSetting);
    }

    bool isPartialModeEnabled() {
        return Mod::get()->getSettingValue<bool>(kPartialSetting);
    }

    bool shouldLogErrors() {
        return Mod::get()->getSettingValue<bool>(kLogSetting);
    }

    bool shouldSpoofFriend(bool originalValue) {
        if (!isSimulationEnabled()) {
            return originalValue;
        }
        if (isPartialModeEnabled() && originalValue) {
            return originalValue;
        }
        return true;
    }

    bool shouldSpoofMessageAccess(bool originalValue) {
        if (!isSimulationEnabled()) {
            return originalValue;
        }
        if (isPartialModeEnabled() && originalValue) {
            return originalValue;
        }
        return true;
    }
}

namespace {
    constexpr FriendRequestStatus kFriendsStatus = FriendRequestStatus::Accepted;

    void activateFriendUI(GJProfilePage* page) {
        if (!page) {
            return;
        }

        auto manager = GJAccountManager::sharedState();
        if (!manager) {
            if (friendgd::shouldLogErrors()) {
                log::error("[FriendGD] Missing account manager when updating profile UI.");
            }
            return;
        }

        const int accountID = page->m_score ? page->m_score->m_accountID : 0;
        const bool realFriend = manager->GJAccountManager::isFriend(accountID);

        if (!friendgd::isSimulationEnabled()) {
            return;
        }

        if (friendgd::isPartialModeEnabled() && realFriend) {
            return;
        }

        page->m_isFriend = true;
        page->m_friendStatus = kFriendsStatus;

        if (page->m_messageBtn) {
            page->m_messageBtn->setVisible(true);
            page->m_messageBtn->setEnabled(true);
        }

        if (page->m_sendRequestBtn) {
            page->m_sendRequestBtn->setVisible(false);
            page->m_sendRequestBtn->setEnabled(false);
        }

        if (page->m_toggleBlockBtn) {
            page->m_toggleBlockBtn->setEnabled(true);
        }
    }
}

class $modify(FriendAccountManager, GJAccountManager) {
public:
    bool isFriend(int accountID) {
        bool original = GJAccountManager::isFriend(accountID);
        return friendgd::shouldSpoofFriend(original);
    }

    bool isFriendRequestSent(int accountID) {
        bool original = GJAccountManager::isFriendRequestSent(accountID);
        if (!friendgd::isSimulationEnabled()) {
            return original;
        }
        if (friendgd::isPartialModeEnabled() && original) {
            return original;
        }
        return false;
    }

    FriendRequestStatus friendRequestStatusForUser(int accountID) {
        auto status = GJAccountManager::friendRequestStatusForUser(accountID);
        if (!friendgd::isSimulationEnabled()) {
            return status;
        }
        if (friendgd::isPartialModeEnabled() && status == kFriendsStatus) {
            return status;
        }
        return kFriendsStatus;
    }

    bool canSendMessageToUser(int accountID) {
        bool original = GJAccountManager::canSendMessageToUser(accountID);
        return friendgd::shouldSpoofMessageAccess(original);
    }
};

class $modify(FriendProfilePage, GJProfilePage) {
public:
    void updateUserScore() {
        GJProfilePage::updateUserScore();
        friendgd::withErrorLogging("updateUserScore", [this] {
            activateFriendUI(this);
            return 0;
        });
    }

    void setupButtons() {
        GJProfilePage::setupButtons();
        friendgd::withErrorLogging("setupButtons", [this] {
            activateFriendUI(this);
            return 0;
        });
    }
};

class $modify(FriendMessagePopup, GJMessagePopup) {
public:
    bool init(GJUserMessage* message, GJMessagePopupDelegate* delegate) {
        if (!GJMessagePopup::init(message, delegate)) {
            return false;
        }

        if (friendgd::isSimulationEnabled()) {
            if (m_buttonMenu) {
                if (auto sendBtn = static_cast<CCMenuItemSpriteExtra*>(m_buttonMenu->getChildByTag(1))) {
                    sendBtn->setVisible(true);
                    sendBtn->setEnabled(true);
                }
            }
        }
        return true;
    }
};
