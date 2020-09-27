#pragma once

#include <unordered_map>
#include <array>
#include <string>
#include <cstdint>

namespace gamecontrollerdb {

enum {
    PlatformInvalid = -1,
    PlatformWindows = 0,
    PlatformMacOS,
    PlatformLinux,
    PlatformiOS,
    PlatformAndroid,
    PlatformMax,
};

enum {
    ButtonInvalid = -1,
    ButtonA,
    ButtonB,
    ButtonX,
    ButtonY,
    ButtonBack,
    ButtonGuide,
    ButtonStart,
    ButtonLeftstick,
    ButtonRightstick,
    ButtonLeftshoulder,
    ButtonRightshoulder,
    ButtonDpadUp,
    ButtonDpadDown,
    ButtonDpadLeft,
    ButtonDpadRight,
    ButtonMax,
    AxisLeftX = ButtonMax,
    AxisLeftY,
    AxisRightX,
    AxisRightY,
    AxisMax,
};

using GUID = std::array<uint8_t, 16>;

struct HashGUID {
    size_t operator()(const gamecontrollerdb::GUID&) const noexcept;
};

class Controller {
    friend class DB;

private:
    bool processToken(int index, const std::string &token);

private:
    GUID guid;
    std::string name;
    int platform = 0;
};

class DB {
    using ControllersByPlatform = std::array<Controller, PlatformMax>;
public:

    bool addFromFile(const std::string &filename);
    int addFromString(const std::string &content);

private:
    bool addFromLine(const std::string &line);

private:
    std::unordered_map<GUID, ControllersByPlatform, HashGUID> controllers;
};

}
