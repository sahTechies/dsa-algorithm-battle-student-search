#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Student.h"
#include <vector>
#include <iostream>
#include <iomanip>

// Slot entry in the Hash Table
struct HashSlot {
    Student student;
    bool isOccupied;

    HashSlot() : isOccupied(false) {}
};

// Hash Table with Open Addressing and Linear Probing
class HashTable {
public:
    static const int TABLE_SIZE = 53; // Fixed prime size for 40-record dataset

private:
    std::vector<HashSlot> table;
    int occupiedCount;
    int totalCollisions;        // Incremented whenever initial regNo % 53 slot is occupied
    int totalProbeStepsInsert;  // Total probe steps taken across all insertions

    // Hash function: regNo % TABLE_SIZE
    int hashFunction(int regNo) const {
        return (regNo % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
    }

public:
    HashTable()
        : table(TABLE_SIZE), occupiedCount(0), totalCollisions(0), totalProbeStepsInsert(0) {}

    // Disallow copy
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // Insert student record using Linear Probing
    // Tracks collision if initial slot is occupied, then steps linearly
    bool insert(const Student& s) {
        if (occupiedCount >= TABLE_SIZE) {
            std::cerr << "Hash Table Error: Table is completely full!\n";
            return false;
        }

        int initialIndex = hashFunction(s.regNo);
        int currentIndex = initialIndex;
        int probes = 0;

        // Check if initial slot suffers a collision
        if (table[initialIndex].isOccupied && table[initialIndex].student.regNo != s.regNo) {
            totalCollisions++;
        }

        // Linear probing: (index + 1) % TABLE_SIZE
        while (table[currentIndex].isOccupied) {
            // Update existing record if same regNo
            if (table[currentIndex].student.regNo == s.regNo) {
                table[currentIndex].student = s;
                return true;
            }

            probes++;
            totalProbeStepsInsert++;
            currentIndex = (currentIndex + 1) % TABLE_SIZE;

            if (probes == TABLE_SIZE) {
                std::cerr << "Hash Table Error: Probed entire table without finding empty slot.\n";
                return false;
            }
        }

        // Place student in the first available slot
        table[currentIndex].student = s;
        table[currentIndex].isOccupied = true;
        occupiedCount++;
        return true;
    }

    // Search student by regNo using Linear Probing
    // Increments comparisons for every slot inspected (including initial hash and probes)
    const Student* search(int regNo, int& comparisons) const {
        comparisons = 0;
        int initialIndex = hashFunction(regNo);
        int currentIndex = initialIndex;
        int checkedSlots = 0;

        while (checkedSlots < TABLE_SIZE) {
            comparisons++;

            // If an empty slot is encountered, key does not exist
            if (!table[currentIndex].isOccupied) {
                return nullptr;
            }

            // Key found
            if (table[currentIndex].student.regNo == regNo) {
                return &(table[currentIndex].student);
            }

            // Linear probe to next slot
            currentIndex = (currentIndex + 1) % TABLE_SIZE;
            checkedSlots++;
        }

        return nullptr;
    }

    // Returns total collisions during insertion
    int getTotalCollisions() const {
        return totalCollisions;
    }

    // Returns total probe steps taken during insertion
    int getTotalProbeStepsInsert() const {
        return totalProbeStepsInsert;
    }

    // Returns total occupied slots
    int size() const {
        return occupiedCount;
    }

    // Returns load factor (alpha = N / M)
    float getLoadFactor() const {
        return static_cast<float>(occupiedCount) / TABLE_SIZE;
    }

    // Display occupied slots, original hash, and probe offset for viva verification
    void displayTable() const {
        std::cout << "\n=========================================================================================\n";
        std::cout << "                         HASH TABLE LAYOUT (Table Size = " << TABLE_SIZE << ")\n";
        std::cout << "=========================================================================================\n";
        std::cout << std::setw(6) << "Index" << " | "
                  << std::setw(6) << "Status" << " | "
                  << std::setw(6) << "RegNo" << " | "
                  << std::setw(20) << std::left << "Name" << " | "
                  << std::setw(6) << "Dept" << " | "
                  << std::setw(5) << "CGPA" << " | "
                  << std::setw(8) << "OrigHash" << " | "
                  << "Probe Offset" << "\n";
        std::cout << "-----------------------------------------------------------------------------------------\n";

        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (table[i].isOccupied) {
                int orig = hashFunction(table[i].student.regNo);
                int offset = (i - orig + TABLE_SIZE) % TABLE_SIZE;
                std::cout << std::setw(6) << std::right << i << " | "
                          << std::setw(6) << "OCC" << " | "
                          << std::setw(6) << table[i].student.regNo << " | "
                          << std::setw(20) << std::left << table[i].student.name << " | "
                          << std::setw(6) << table[i].student.dept << " | "
                          << std::setw(5) << std::fixed << std::setprecision(2) << table[i].student.cgpa << " | "
                          << std::setw(8) << std::right << orig << " | "
                          << (offset == 0 ? "Direct (0)" : ("+" + std::to_string(offset) + " (Probed)")) << "\n";
            } else {
                std::cout << std::setw(6) << std::right << i << " | "
                          << std::setw(6) << "EMPTY" << " | "
                          << std::setw(6) << "-" << " | "
                          << std::setw(20) << std::left << "-" << " | "
                          << std::setw(6) << "-" << " | "
                          << std::setw(5) << "-" << " | "
                          << std::setw(8) << std::right << "-" << " | "
                          << "-" << "\n";
            }
        }
        std::cout << "=========================================================================================\n";
        std::cout << "Occupied Slots: " << occupiedCount << " / " << TABLE_SIZE
                  << " | Load Factor (alpha): " << std::fixed << std::setprecision(3) << getLoadFactor()
                  << " | Total Initial Collisions: " << totalCollisions
                  << " | Total Probes: " << totalProbeStepsInsert << "\n";
        std::cout << "=========================================================================================\n\n";
    }
};

#endif // HASHTABLE_H

