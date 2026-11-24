// main.cpp
#include "calc.cpp"
#include "data.hpp"
#include <filesystem>
#include <format>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// --- prompt helpers ---
std::string prompt(const std::string &q) {
    std::cout << q;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

bool prompt_yes(const std::string &q) {
    std::string s = prompt(q);
    return s == "Y" || s == "y";
}

int prompt_int(const std::string &q) {
    while (true) {
        std::string s = prompt(q);
        try {
            return std::stoi(s);
        } catch (...) {
            std::cout << "enter a number\n";
        }
    }
}

// --- init flow (SPEC: steps 3-10) ---
void add_course(SemesterFile &f) {
    Course c;
    c.code = prompt("course code (e.g. CS2310): ");
    c.name = prompt("course name: ");
    double left = 100.0;
    int i = 1;
    while (true) {
        Component comp;
        comp.name = prompt("component " + std::to_string(i) + " name: ");
        std::string w = prompt("weight (0-100 or 'remainder'): ");
        double wgt;
        if (w == "remainder") {
            wgt = left;
        } else {
            wgt = std::stod(w);
        }
        if (wgt <= 0.0 || wgt > left) {
            std::cout << "invalid weight, try again\n";
            continue;
        }
        comp.weight = wgt;
        left -= wgt;
        std::string t = prompt("single or total? (s/t): ");
        if (t == "t" || t == "T") {
            comp.type = "total";
            comp.partitions = prompt_int("partitions: ");
            comp.marks.assign(comp.partitions, -1.0);
        } else {
            comp.type = "single";
            comp.marks = {-1.0};
        }
        c.components.push_back(comp);
        i++;
        if (left <= 0.0) {
            break;
        }
        if (!prompt_yes("add another component? ")) {
            break;
        }
    }
    f.courses.push_back(c);
}

void edit_user_data() {
    std::string sel = pick_semester();
    if (sel.empty()) {
        return;
    }
    std::string path = "data/" + sel + ".json";
    SemesterFile f = load_semester(path);

    std::cout << "current:\n"
              << "  1. display name: " << f.display_name << "\n"
              << "  2. degree:       " << f.degree << "\n"
              << "  3. semester:     " << f.semester << "\n";
    int field = prompt_int("edit which field? (1-3, 0=cancel): ");
    if (field < 1 || field > 3) {
        return;
    }

    if (field == 1) {
        f.display_name = prompt("new display name: ");
    } else if (field == 2) {
        f.degree = prompt("new degree: ");
    } else {
        std::string old_sem = f.semester;
        f.semester = prompt("new semester (e.g. 2A): ");
        if (f.semester != old_sem) {
            // rename the file so path stays consistent with stem
            std::string new_path = "data/" + f.semester + ".json";
            save_semester(new_path, f);
            fs::remove(path);
            std::cout << "updated (renamed " << path << " -> " << new_path << ")\n";
            return;
        }
    }
    save_semester(path, f);
    std::cout << "updated\n";
}

void init_new_semester() {
    SemesterFile f;
    f.display_name = prompt("display name: ");
    f.degree = prompt("degree: ");
    f.semester = prompt("semester (e.g. 2A): ");
    if (prompt_yes("add a course now? ")) {
        add_course(f);
    }
    std::string path = "data/" + f.semester + ".json";
    save_semester(path, f);
    std::cout << "saved " << path << "\n";
}

// Add courses to an existing semester
void add_new_courses() {
    std::string sel = pick_semester();
    if (sel.empty()) {
        return;
    }
    std::string path = "data/" + sel + ".json";
    SemesterFile f = load_semester(path);

    do {
        add_course(f);
    } while (prompt_yes("add another course? "));

    save_semester(path, f);
    std::cout << "saved " << path << "\n";
}

// Add components to an existing course in a semester
void add_new_components() {
    std::string sel = pick_semester();
    if (sel.empty()) {
        return;
    }
    std::string path = "data/" + sel + ".json";
    SemesterFile f = load_semester(path);

    if (f.courses.empty()) {
        std::cout << "no courses in this semester\n";
        return;
    }

    std::cout << "courses:\n";
    for (size_t i = 0; i < f.courses.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << f.courses[i].code << " - " << f.courses[i].name
                  << "\n";
    }
    std::string key = prompt("course code or #: ");
    Course *target = nullptr;
    try {
        int idx = std::stoi(key);
        if (idx >= 1 && idx <= (int)f.courses.size()) {
            target = &f.courses[idx - 1];
        }
    } catch (...) {
    }
    if (!target) {
        for (auto &c : f.courses) {
            if (c.code == key || c.name == key) {
                target = &c;
                break;
            }
        }
    }
    if (!target) {
        std::cout << "course not found\n";
        return;
    }

    add_components_to_course(*target);
    save_semester(path, f);
    std::cout << "updated\n";
}

// --- record a mark (SPEC: new item 6) ---
void record_mark() {
    std::vector<std::string> sems;
    for (auto &e : fs::directory_iterator("data")) {
        if (e.path().extension() == ".json") {
            sems.push_back(e.path().stem().string());
        }
    }
    if (sems.empty()) {
        std::cout << "no semester data\n";
        return;
    }
    std::string sel = (sems.size() == 1) ? sems[0] : prompt("which semester? (" + sems[0] + "): ");
    SemesterFile f = load_semester("data/" + sel + ".json");

    std::string code = prompt("course code: ");
    for (auto &c : f.courses) {
        if (c.code != code) {
            continue;
        }
        std::cout << "components:\n";
        for (size_t i = 0; i < c.components.size(); i++) {
            std::cout << (i + 1) << ". " << c.components[i].name << "\n";
        }
        int pick = prompt_int("component #: ");
        if (pick < 1 || pick > (int)c.components.size()) {
            return;
        }
        Component &comp = c.components[pick - 1];
        for (size_t k = 0; k < comp.marks.size(); k++) {
            std::string q = comp.marks.size() > 1
                                ? std::format("mark {} (empty = keep {}): ", k + 1, comp.marks[k])
                                : "mark (empty = keep): ";
            std::string s = prompt(q);
            if (!s.empty()) {
                comp.marks[k] = std::stod(s);
            }
        }
        save_semester("data/" + sel + ".json", f);
        std::cout << "updated\n";
        return;
    }
    std::cout << "course not found\n";
}

// --- receipt (your mockup) ---
std::string pad(const std::string &s, int w) {
    if ((int)s.size() >= w) {
        return s;
    }
    return s + std::string(w - s.size(), '.');
}

void show_report(const SemesterFile &f) {
    std::cout << std::format("NAME: {}\n", f.display_name);
    std::cout << std::format("DEGREE: {}\n", f.degree);
    std::cout << std::format("SEMESTER: Year {} ({})\n\n", f.semester, f.semester);
    for (const Course &c : f.courses) {
        std::cout << std::format("COURSE: {} ({})\n", c.name, c.code);
        int i = 1;
        for (const Component &comp : c.components) {
            double st = component_subtotal(comp);
            double sub = (st >= 0.0) ? st * comp.weight / 100.0 : 0.0;
            std::cout << std::format("  ({}) {} | {:>4.0f}% | {:>7.2f} / {:>3d} | {:>7.2f}%\n", i,
                                     pad(comp.name, 20), comp.weight, (st >= 0.0) ? st : 0.0, 100,
                                     sub);
            if (comp.type == "total") {
                char letter = 'a';
                for (double m : comp.marks) {
                    std::cout << std::format("     ({}{}) {:<20} {:>7.2f} / {:>3d}\n", i, letter++,
                                             "", (m >= 0.0) ? m : 0.0, 100);
                    letter++;
                }
            }
            i++;
        }
        std::cout << std::format("  course total: {:.2f}%\n\n", course_total(c));
    }
    std::cout << std::format("SEMESTER TOTAL: {:.2f}%\n", semester_total(f));
}

int main() {
    if (!fs::exists("data")) {
        fs::create_directories("data");
    }
    bool has_data = false;
    for (auto &e : fs::directory_iterator("data")) {
        if (e.path().extension() == ".json") {
            has_data = true;
            break;
        }
    }
    if (!has_data) {
        std::cout << "no data found, running init...\n";
        init_new_semester();
    }
    while (true) {
        std::cout << "\n1. Edit user data\n2. Add new semester record\n3. Add new courses\n"
                  << "4. Add new components\n5. Show report\n6. Record a mark\n0. Quit\n";
        int c = prompt_int("> ");
        switch (c) {
        case 1:
            edit_user_data();
            break;
        case 2:
            init_new_semester();
            break;
        case 3:
            add_new_courses();
            break;
        case 4:
            add_new_components();
            break;
        case 5: {
            for (auto &e : fs::directory_iterator("data")) {
                if (e.path().extension() != ".json") {
                    continue;
                }
                show_report(load_semester(e.path().string()));
            }
            break;
        }
        case 6:
            record_mark();
            break;
        case 0:
            return 0;
        default:
            std::cout << "?\n";
        }
    }
}
