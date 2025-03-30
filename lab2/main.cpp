#include "quadratic_equation.h"
#include "student.h"
#include "teacher.h"
#include <fstream>
#include <iostream>
#include <vector>

std::vector<QuadraticEquation>
readEquationsFromFile(const std::string& filename) {
    std::vector<QuadraticEquation> equations;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename << std::endl;
        return equations;
    }

    QuadraticEquation eq;
    while (file >> eq.a >> eq.b >> eq.c) {
        equations.push_back(eq);
    }

    file.close();
    return equations;
}

int main() {
    auto equations = readEquationsFromFile("equations.txt");
    if (equations.empty()) {
        std::cerr << "No equations found in file." << std::endl;
        return 1;
    }

    Student good_student("Andrey (good)", StudentType::GOOD);
    Student average_student1("Dmitriy (average)", StudentType::AVERAGE, 0.85);
    Student average_student2("Egor (average)", StudentType::AVERAGE, 0.55);
    Student bad_student("Pididi (bad)", StudentType::BAD);

    std::vector<Student> students = { good_student, average_student1,
                                     average_student2, bad_student };

    Teacher teacher;

    for (const auto& eq : equations) {
        for (const auto& student : students) {
            Solution sol = student.solveEquation(eq);
            teacher.receiveSolution(eq, sol, student.getName());
        }
    }

    teacher.checkAllSolutions();
    teacher.publishResults();

    return 0;
}