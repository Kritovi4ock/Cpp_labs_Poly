#pragma once
#include "quadratic_equation.h"
#include "student.h"
#include <map>
#include <string>
#include <vector>

struct StudentSolution {
    QuadraticEquation equation;
    Solution solution;
    std::string student_name;
};

class Teacher {
private:
    std::vector<StudentSolution> solutions_queue;
    std::map<std::string, int> results_table;

public:
    void receiveSolution(const QuadraticEquation& eq, const Solution& sol,
        const std::string& student_name);
    void checkAllSolutions();
    void publishResults() const;
};