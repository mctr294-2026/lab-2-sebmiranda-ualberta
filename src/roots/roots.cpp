#include <functional>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "roots.hpp"

using namespace std;

double tolerance = 1e-6;
double max_iterations = 1e6;

// Bisection Method Execution
//Step 1: Start with two points, a and b, such that f(a) and f(b) have opposite signs. This guarantees that there is at least one root between a and b.
//Step 2: Calculate the midpoint, c, of the interval [a,b] using  c = (a + b)/2.
//Step 3: Determine the sign of f(c). If f(c) is close enough to zero (within a predefined tolerance), c is the root. Otherwise, replace a or b with c depending on the sign of f(c), ensuring that the new interval still brackets the root.
//Step 4: Repeat the process until the interval is sufficiently small or f(c) is close enough to zero.

bool bisection(function<double(double)> f,
                double a, double b,
                double *root) {
    // Check that f(a) and f(b) have opposite signs
    if (f(a) * f(b) >= 0){
        return false;
    }
    // Define a variable to track iterations
    int iterations = 0;
    // Calculatre the midpoint c
    double c = (a + b) / 2.0;
    // Iteratively refine the bracket [a, b]
    while(abs(f(c)) > tolerance && iterations < max_iterations){
        // Refines the bracket [a, b] based on the sign of f(c)
        if (f(a) * f(c) < 0){
            b = c;
        } else {
            a = c;
        }
        // Recalculates the midpoint c
        c = (a + b) / 2.0;
        iterations++;
    }
    // If the root is found within tolerance, assigns it to the output parameter and returns true
    if (abs(f(c)) < tolerance){
        *root = c;
        return true;
    } else {
        return false;
    }
}
// Regula Falsi Method Execution
//Step 1: Start with two points, a and b, such that f(a) and f(b) have opposite signs. This guarantees that there is at least one root between a and b.
//Step 2: Calculate the midpoint, c, of the interval [a,b] using  c = a - [f(a).(b - a)]/[f(b) - f(a)].
//Step 3: Evaluate f(c). If f(c) is close enough to zero (within a predefined tolerance), then c is the root.
//Step 4: Depending on the sign of f(c), update the interval:
	//If f(a) and f(c) have opposite signs, set b = c.
	//If f(b) and f(c) have opposite signs, set a = c.
//Step 5: Repeat the process until the interval is sufficiently small or f(c) is close enough to zero.
bool regula_falsi(function<double(double)> f,
                  double a, double b,
                  double *root) {
     // Check that f(a) and f(b) have opposite signs
    if (f(a) * f(b) >= 0)
    {
        return false;
    }
    // Define a variable to track iterations
    int iterations = 0;
    // Calculates the midpoint c
    double c = a - (f(a) * (b - a)) / (f(b) - f(a));
    // Iteratively refine the bracket [a, b]
    while(abs(f(c)) > tolerance && iterations < max_iterations){
        if (f(a) * f(c) < 0) {
            b = c;
        } else {
            a = c;
        }
        c = a - (f(a) * (b - a)) / (f(b) - f(a));
        iterations++;
    } 
    // If the root is found within tolerance, assigns it to the output parameter and returns true
    if (abs(f(c)) < tolerance) {
        *root = c;
        return true;
    } else {
        return false;
    }
}
//Step 1: Start with an initial guess x0.
//Step 2: Use the formula (x_n - x_n-1)/(f(x_n)-f(x_n-1))
//Step 3: Repeat the iteration until the change between xn and x(n+1) is smaller than a predefined tolerance.

// Newton-Raphson Method Execution
bool newton_raphson(function<double(double)> f,
                    function<double(double)> g,
                    double a, double b, double c,
                    double *root) {
    // Check a is less than b
    if (a > b) {
        throw invalid_argument("a must be less than b");
    }
    // Check c is within [a, b]
    if (c < a || c > b) {
        throw invalid_argument("c must be within [a, b]");
    }
    // Define a variable to track iterations
    int iterations = 0;

    // Checks if the guess is the root
    if (abs(f(c)) < tolerance){
        *root = c;
        return true;
    }
    // Iteratively refines the guess c until the root is found within tolerance or max iterations reached
    while(iterations < max_iterations){
        // Creates a variable to store the derivative at c
        double g_c = g(c);

        // Prevents division by zero
        if (g_c == 0) {
            return false;
        }
        // Gets the new c value using the Newton-Raphson formula
        double c_new = c - f(c) / g_c;

        // Checks if the change between c and c_new is within tolerance
        if (abs(c - c_new) < tolerance){
            *root = c_new;
            return true;
        } else{
            c = c_new;
        }

        iterations++;
    }

    // If the root is not found within max iterations, returns false
    return false;
}
// Secant Method Execution
//Step 1: Start with two initial guesses x0 and x1.
//Step 2: Use the formula x_n+1 = (x_n - x_n-1)/f(x_n)-f(x_n-1) to find the next approximation.
//Step 3: Repeat the iteration until the change between xn and xn+1 is smaller than a predefined tolerance.

bool secant(function<double(double)> f,
            double a, double b, double c,
            double *root){
    // Defines a variable to track iterations
    int iterations = 0;
    // Defined c_next to store the next approximation
    double c_next;
    // Ensures a is less than b
    if (a > b){
        throw invalid_argument("a must be less than b");
    }
    // Ensures c is within [a, b]
    if (c < a || c > b){
        throw invalid_argument("c must be within [a, b]");
    }
    // Chooses the 2nd point
    if (c == a) {
        c_next = a + (b - a) * 0.1; // Moves into the interval
    } else if (c == b) {
        c_next = b - (b - a) * 0.1; // Moves into the interval
    } else {
        // Tries to get it on opposite sides if possible
        double f_a = f(a);
        double f_b = f(b);
        if (f(a) * f(c) < 0){
            c_next = a; // Uses a if it's on the opposite side
        } else if (f(b) * f(c) < 0) {
            c_next = b; // Uses b if it's on the opposite side
        } else {
            // No sign change, chooses a point near c
            double dx = (b - a) * 0.1;
            c_next = c + dx;
            if (c_next > b) c_next = c - dx;
        }
    }
    // Ensures within bounds and different from c
    if (c_next < a) c_next = a;
    if (c_next > b) c_next = b;
    // Checks faliure case that the points are essentially the same
    if (abs(c_next - c) < tolerance){
        return false;
    }
    // Checks if either guess is a root
    if (abs(f(c)) < tolerance){
        *root = c;
        return true;
    } else if (abs(f(c_next)) < tolerance){
        *root = c_next;
        return true;
    }
    // Iteratively refines the guesses c and c_next until the root is found within tolerance or max iterations reached
    while(iterations < max_iterations){
        double f_c = f(c);
        double f_c_next = f(c_next);
        // Prevents division by zero
        if (f_c - f_c_next == 0){
            return false;
        }
        // Gets the new c value using the Secant formula
        double c_new = c - f_c * (c - c_next) / (f_c - f_c_next);

        // Checks if the change between c_next and c_new is within tolerance
        if (abs(c_next - c_new) < tolerance){
            *root = c_new;
            return true;
        } else{
            c = c_next;
            c_next = c_new;
        }

        iterations++;
    }
    // If the root is not found within max iterations, return false
    return false;
}