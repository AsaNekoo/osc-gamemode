#include <Geode/binding/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/loader/SettingV3.hpp>

#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include <string>

using namespace geode::prelude;

auto oscPath = Mod::get()->getSettingValue<std::string>("path");
auto port = Mod::get()->getSettingValue<int>("port");

$on_mod(Loaded) {
    listenForSettingChanges<std::string>(
        "path",
        [](std::string const& value) {
            geode::log::info("OSC path changed to: {}", value);
            oscPath = value;
        }
    );

    listenForSettingChanges<int>(
        "port",
        [](int const& value) {
            port = value;
        }
    );
}

static UdpTransmitSocket& oscOut() {
    static UdpTransmitSocket sock(IpEndpointName("127.0.0.1", port));
    return sock;
}

enum GameMode {
    Cube = 0,
    Ship = 1,
    Ufo = 2,
    Wave = 3,
    Ball = 4,
    Robot = 5,
    Spider = 6,
    Swing = 7
};

static int getGameMode(PlayerObject* player) {
    if (!player) return -1;

    if (player->m_isShip) return GameMode::Ship;
    if (player->m_isBird) return GameMode::Ufo;
    if (player->m_isDart) return GameMode::Wave;
    if (player->m_isBall) return GameMode::Ball;
    if (player->m_isRobot) return GameMode::Robot;
    if (player->m_isSpider) return GameMode::Spider;
    if (player->m_isSwing) return GameMode::Swing;

    return GameMode::Cube;
}

class $modify(Player, PlayerObject) {
    struct Fields {
        int m_lastMode = -1;
    };

    void update(float dt) {
        PlayerObject::update(dt);

        auto pl = PlayLayer::get();
        if (!pl || this != pl->m_player1 && this != pl->m_player2) return;

        int mode = getGameMode(this);
        if (mode == m_fields->m_lastMode) return;

        m_fields->m_lastMode = mode;
        geode::log::info("The player has entered mode {}", m_fields->m_lastMode);

        // OSC
        char buffer[256];
        osc::OutboundPacketStream p(buffer, sizeof(buffer));
        p << osc::BeginMessage(std::string(oscPath).c_str())
          << m_fields->m_lastMode
          << osc::EndMessage;

        oscOut().Send(p.Data(), p.Size());

        return;
    }
};