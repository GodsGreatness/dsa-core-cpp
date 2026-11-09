#include "dsa/linear/Vector.hpp"
#include <iostream>
#include <string>

struct Student {
    std::string name;
    double gpa;

    Student(std::string n, double g) : name(std::move(n)), gpa(g) {}
};

int main() {
    std::cout << "=== dsa::Vector Example ===\n\n";

    dsa::Vector<Student> students;
    students.emplace_back("Alice", 3.9);
    students.emplace_back("Bob", 3.7);
    students.emplace_back("Charlie", 4.0);

    std::cout << "Initial students (size = " << students.size()
              << ", capacity = " << students.capacity() << "):\n";
    for (std::size_t i = 0; i < students.size(); ++i) {
        std::cout << "  - " << students[i].name << " (GPA: " << students[i].gpa << ")\n";
    }

    students.pop_back();
    std::cout << "\nAfter pop_back, back is: " << students.back().name << "\n";

    return 0;
}
