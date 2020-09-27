#pragma once

#include <unordered_map>
#include <array>
#include <string>
#include <cstdint>

namespace gamecontrollerdb {

enum : size_t {
    PlatformWindows = 0,
    PlatformMacOS,
    PlatformLinux,
    PlatformiOS,
    PlatformAndroid,
    PlatformMax
};

using GUID = std::array<uint8_t, 16>;

class Controller {
    friend class DB;

private:
    GUID guid;
    std::string name;
};

class DB {
    using ControllersByPlatform = std::array<Controller, PlatformMax>;
public:

    bool addFromFile(const std::string &filename);
    int addFromString(const std::string &content);

private:
    bool addFromLine(const std::string &line);
    bool processToken(Controller &c, const std::string &token);

private:
    std::unordered_map<GUID, ControllersByPlatform> controllers;
};

}
