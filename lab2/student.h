#pragma once
#include "quadratic_equation.h"
#include <string>

enum class StudentType { GOOD, AVERAGE, BAD };

class Student {
private:
    std::string name;
    StudentType type;
    double correct_probability;

public:
    Student(const std::string& name, StudentType type,
        double correct_probability = 1.0);

    Solution solveEquation(const QuadraticEquation& eq) const;
    std::string getName() const;
};