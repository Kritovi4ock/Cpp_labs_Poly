#include "quadratic.h"
int main() {
    double a, b, c;
    std::cout << "Enter the coefficients a, b, c of a quadratic equation of the "
        "form: ax^2+bx+c=0"
        << std::endl;
    std::cin >> a >> b >> c;
    solve_equation(a, b, c);
    return 0;
}