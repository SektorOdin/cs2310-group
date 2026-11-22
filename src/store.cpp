#include "data.hpp"
#include <fstream>
#include <stdexcept>

SemesterFile load_semester(const std::string& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("cannot open " + path);
    nlohmann::json j;
    f >> j;                        // throws parse_error if file isn't valid JSON
    return j.get<SemesterFile>();  // throws if keys don't match the struct
}

void save_semester(const std::string& path, const SemesterFile& data) {
    std::ofstream f(path);
    if (!f) throw std::runtime_error("cannot write " + path);
    f << nlohmann::json(data).dump(2);
}