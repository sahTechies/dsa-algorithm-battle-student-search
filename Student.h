#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <iostream>
#include <iomanip>

// Structure representing a Student Record in the University System
struct Student {
    int regNo;          // Unique Student Registration Number (Key)
    std::string name;   // Full Name
    std::string dept;   // Academic Department
    float cgpa;         // Cumulative Grade Point Average

    // Default constructor
    Student() : regNo(0), name(""), dept(""), cgpa(0.0f) {}

    // Parameterized constructor
    Student(int r, const std::string& n, const std::string& d, float c)
        : regNo(r), name(n), dept(d), cgpa(c) {}

    // Formatted display helper for technical defense and console verification
    void display() const {
        std::cout << "RegNo: " << std::setw(6) << std::right << regNo
                  << " | Name: " << std::setw(22) << std::left << name
                  << " | Dept: " << std::setw(8) << dept
                  << " | CGPA: " << std::fixed << std::setprecision(2) << cgpa << "\n";
    }
};

#endif // STUDENT_H
