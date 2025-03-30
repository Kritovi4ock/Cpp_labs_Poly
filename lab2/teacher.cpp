#include "teacher.h"
#include <iostream>

void Teacher::receiveSolution(const QuadraticEquation& eq, const Solution& sol,
    const std::string& student_name) {
    solutions_queue.push_back({ eq, sol, student_name });
}

void Teacher::checkAllSolutions() {
    for (const auto& student_sol : solutions_queue) {
        Solution correct_sol = solveEquation(student_sol.equation);
        bool is_correct = false;

        if (correct_sol.num_roots == student_sol.solution.num_roots) {
            if (correct_sol.num_roots == 0) {
                is_correct = true;
            }
            else if (correct_sol.num_roots == 1) {
                is_correct = (abs(correct_sol.x1 - student_sol.solution.x1) < 1e-6);
            }
            else {
                is_correct = (abs(correct_sol.x1 - student_sol.solution.x1) < 1e-6 &&
                    abs(correct_sol.x2 - student_sol.solution.x2) < 1e-6) ||
                    (abs(correct_sol.x1 - student_sol.solution.x2) < 1e-6 &&
                        abs(correct_sol.x2 - student_sol.solution.x1) < 1e-6);
            }
        }

        if (is_correct) {
            results_table[student_sol.student_name]++;
        }
        else {
            results_table[student_sol.student_name] += 0;
        }
    }

    solutions_queue.clear();
}

void Teacher::publishResults() const {
    std::cout << "Results table:\n";
    for (const auto& entry : results_table) {
        std::cout << entry.first << ": " << entry.second << " correct solutions\n";
    }
}