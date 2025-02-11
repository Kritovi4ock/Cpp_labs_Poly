#include "quadratic.h"

void solve_equation(double a, double b, double c)
{
	if (a == 0)
	{
		solve_line(b, c);
	}
	else
	{
		solve_quadratic(a, b, c);
	}
}
void solve_line(double b, double c)
{
	if (b == 0)
	{
		if (c == 0)
		{
			std::cout << "Any x is a solution" << std::endl;
		}
		else
		{
			std::cout << "No x is a solution" << std::endl;
		}
	}
	else
	{
		double x = (-c) / b;
		std::cout << "There is one root: " << x << std::endl;
	}
}
double discriminant(double a, double b, double c)
{
	return pow(b, 2) - 4 * a * c;
}
void solve_quadratic(double a, double b, double c)
{
	double d = discriminant(a, b, c);
	if (d < 0)
	{
		std::cout << "There are no valid roots" << std::endl;
	}
	else if (d == 0)
	{
		double x = (-b) / (2 * a);
		std::cout << "There is one root: " << x << std::endl;
	}
	else
	{
		double x1 = (-b - sqrt(d)) / (2 * a);
		double x2 = (-b + sqrt(d)) / (2 * a);
		std::cout << "There are two roots:" << std::endl << "x1 = " << x1 << std::endl << "x2 = " << x2 << std::endl;

	}
}