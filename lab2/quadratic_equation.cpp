#include "quadratic_equation.h"
#include <cmath>

Solution solveEquation(const QuadraticEquation& eq) {
    Solution sol{};
    double discriminant = eq.b * eq.b - 4 * eq.a * eq.c;

    if (discriminant > 0) {
        sol.num_roots = 2;
        sol.x1 = (-eq.b + sqrt(discriminant)) / (2 * eq.a);
        sol.x2 = (-eq.b - sqrt(discriminant)) / (2 * eq.a);
    }
    else if (discriminant == 0) {
        sol.num_roots = 1;
        sol.x1 = -eq.b / (2 * eq.a);
        sol.x2 = sol.x1;
    }
    else {
        sol.num_roots = 0;
    }

    return sol;
}