#include "gamecontrollerdb.h"

#include <fstream>
#include <cstdlib>

namespace gamecontrollerdb {

bool DB::addFromFile(const std::string &filename) {
    std::ifstream fs(filename);
    if (!fs.is_open()) {
        return false;
    }
    fs.seekg(0, std::ios::end);
    auto size = fs.tellg();
    if (size <= 0) {
        fs.close();
        return false;
    }
    fs.seekg(0, std::ios::beg);
    std::string n(size, 0);
    fs.read(&n[0], size);
    fs.close();
    return addFromString(n) > 0;
}

int DB::addFromString(const std::string &content) {
    int result = 0;
    typename std::string::size_type pos = 0;
    while (pos != std::string::npos) {
        auto end_pos = content.find_first_of("\r\n", pos);
        auto line = content.substr(pos, end_pos == std::string::npos ? std::string::npos : (end_pos - pos));
        pos = content.find_first_not_of("\r\n", end_pos);
        addFromLine(line);
    }
    return result;
}

inline bool convertGUID(GUID &guid, const std::string &guidStr) {
    size_t len = guidStr.length() & ~1;
    if (len > guid.size()) {
        len = guid.size();
    }
    for (size_t i = 0; i < len; i += 2) {
        char hex[3] = {guidStr[i], guidStr[i + 1], 0};
        char *end_ptr;
        guid[i >> 1] = static_cast<uint8_t>(std::strtoul(hex, &end_ptr, 16));
        if (end_ptr && end_ptr - hex < 2) {
            /* not a valid hex string */
            return false;
        }
    }
    return true;
}

bool DB::addFromLine(const std::string &line) {
    typename std::string::size_type pos = 0;
    Controller c;
    int index = 0;
    while (pos != std::string::npos) {
        auto start_pos = line.find_first_not_of(" \t\v\r\n", pos);
        auto end_pos = line.find(',', pos);
        std::string token;
        if (end_pos == std::string::npos) {
            token = line.substr(start_pos, end_pos - start_pos);
            pos = std::string::npos;
        } else {
            token = line.substr(start_pos);
            pos = end_pos + 1;
        }
        switch (index) {
        case 0:
            if (!convertGUID(c.guid, token)) {
                return false;
            }
            break;
        case 1:
            c.name = std::move(token);
            break;
        default:
            processToken(c, token);
            break;
        }
        ++index;
    }
    return true;
}

bool DB::processToken(Controller &c, const std::string &token) {
    return true;
}

}
