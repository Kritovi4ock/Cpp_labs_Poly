#include "student.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Student::Student(const std::string& name, StudentType type,
    double correct_probability)
    : name(name), type(type), correct_probability(correct_probability) {
    static bool seeded = false;
    if (!seeded) {
        srand(time(nullptr));
        seeded = true;
    }
}

Solution Student::solveEquation(const QuadraticEquation& eq) const {
    Solution sol{};

    try {
        switch (type) {
        case StudentType::GOOD:
            sol = ::solveEquation(eq);
            break;

        case StudentType::AVERAGE:
            if ((rand() % 100) < (correct_probability * 100)) {
                sol = ::solveEquation(eq);
            }
            else {
                sol.num_roots = 1;
                sol.x1 = rand() % 20 - 10;
                sol.x2 = sol.x1;
            }
            break;

        case StudentType::BAD:
            sol.num_roots = 1;
            sol.x1 = 0;
            sol.x2 = 0;
            break;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error solving equation for student " << name << ": "
            << e.what() << std::endl;
        sol.num_roots = 0;
    }
    catch (...) {
        std::cerr << "Unknown error solving equation for student " << name
            << std::endl;
        sol.num_roots = 0;
    }

    return sol;
}

std::string Student::getName() const { return name; }