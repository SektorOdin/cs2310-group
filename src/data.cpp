#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct Component {
    std::string name;
    std::string type;            // "single" | "total"
    double      weight;          // already-resolved number
    int         partitions = 1;  // ignored if single
    bool        isEqual  = true; // partitions weighted equally (true = the only mode for now)
    std::vector<double> marks;   // 1 element if single; -1 = not entered
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Component, name, type, weight, partitions, marks)

struct Course {
    std::string code;
    std::string name;
    std::vector<Component> components;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Course, code, name, components)

struct SemesterFile {
    std::string display_name;
    std::string degree;
    std::string semester;
    std::vector<Course> courses;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SemesterFile, display_name, degree, semester, courses)