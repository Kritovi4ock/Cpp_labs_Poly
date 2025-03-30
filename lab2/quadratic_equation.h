#pragma once
struct QuadraticEquation {
	double a, b, c;
};

struct Solution {
	double x1, x2;
	int num_roots;
};

Solution solveEquation(const QuadraticEquation& eq);