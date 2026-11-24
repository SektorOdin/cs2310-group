#pragma once
#include "data.hpp"

// A negative subtotal means no marks have been entered.
double component_subtotal(const Component &component);
double course_total(const Course &course);
// A negative total means the semester has no courses.
double calculateSemesterTotal(const SemesterFile &semester);
