// calc.cpp
// short for calculator btw
#pragma once
#include "data.hpp"

inline double component_subtotal(const Component& c) {
    double sum = 0.0;
    int n = 0;
    for (double m : c.marks) {
        if (m >= 0.0) { sum += m; n++; }
    }
    return n > 0 ? sum / n : -1.0;  // -1 = nothing entered yet
}

inline double course_total(const Course& c) {
    double total = 0.0;
    for (const Component& comp : c.components) {
        double st = component_subtotal(comp);
        if (st >= 0.0) { total += st * comp.weight / 100.0; }
    }
    return total;
}

inline double semester_total(const SemesterFile& f) {
    if (f.courses.empty()) { return -1.0; }
    double sum = 0.0;
    int n = 0;
    for (const Course& c : f.courses) {
        double t = course_total(c);
        if (t >= 0.0) { sum += t; n++; }
    }
    return n > 0 ? sum / n : -1.0;
}