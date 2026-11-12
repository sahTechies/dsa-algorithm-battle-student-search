#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <numeric>
#include "Student.h"
#include "BST.h"
#include "AVL.h"
#include "HashTable.h"

// Helper function to trim whitespace and Windows carriage returns (\r)
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Loads student records from dataset.csv
std::vector<Student> loadDataset(const std::string& filename) {
    std::vector<Student> dataset;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open dataset file: " << filename << "\n";
        return dataset;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        if (isHeader) {
            isHeader = false;
            // Check if line looks like header
            if (line.find("regNo") != std::string::npos || line.find("name") != std::string::npos) {
                continue;
            }
        }

        std::stringstream ss(line);
        std::string regNoStr, name, dept, cgpaStr;

        if (std::getline(ss, regNoStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, dept, ',') &&
            std::getline(ss, cgpaStr, ',')) {

            try {
                int regNo = std::stoi(trim(regNoStr));
                name = trim(name);
                dept = trim(dept);
                float cgpa = std::stof(trim(cgpaStr));
                dataset.emplace_back(regNo, name, dept, cgpa);
            } catch (const std::exception& e) {
                std::cerr << "[WARNING] Skipping malformed record: " << line << " (" << e.what() << ")\n";
            }
        }
    }

    file.close();
    return dataset;
}

// Executes automated benchmark tests for 5 successful and 5 unsuccessful searches
void runBenchmark(const BST& bst, const AVL& avl, const HashTable& ht) {
    std::cout << "\n=========================================================================================\n";
    std::cout << "               ALGORITHM BATTLE BENCHMARK REPORT: STUDENT RECORD SEARCH                  \n";
    std::cout << "=========================================================================================\n";
    std::cout << "Dataset Size              : 40 Student Records\n";
    std::cout << "Hash Table Size           : " << HashTable::TABLE_SIZE << " Slots (Prime Number)\n";
    std::cout << "Hash Table Load Factor    : " << std::fixed << std::setprecision(2) << (ht.getLoadFactor() * 100.0f) << "%\n";
    std::cout << "-----------------------------------------------------------------------------------------\n";
    std::cout << "STRUCTURAL METRICS (After 40 Insertions):\n";
    std::cout << "  - Unbalanced BST Height : " << bst.getHeight() << " (Subject to insertion-order skew)\n";
    std::cout << "  - Balanced AVL Height   : " << avl.getHeight() << " (Strictly height-balanced, O(log N))\n";
    std::cout << "  - HashTable Collisions  : " << ht.getTotalCollisions() << " Initial Collisions (Probed: " 
              << ht.getTotalProbeStepsInsert() << " total probe steps)\n";
    std::cout << "=========================================================================================\n\n";

    // 5 Successful Search Keys
    const std::vector<int> successfulKeys = { 1001, 1025, 1042, 1068, 1080 };
    std::vector<int> bstSuccessComps;
    std::vector<int> avlSuccessComps;
    std::vector<int> htSuccessComps;

    std::cout << "TEST 1: 5 SUCCESSFUL SEARCHES (Records Present in Dataset)\n";
    std::cout << "+-------+----------------------+-------+------+---------+---------+-----------+\n";
    std::cout << "| RegNo | Name                 | Dept  | CGPA | BST Cmp | AVL Cmp | Hash Cmp  |\n";
    std::cout << "+-------+----------------------+-------+------+---------+---------+-----------+\n";

    for (int key : successfulKeys) {
        int cBST = 0, cAVL = 0, cHT = 0;
        const Student* sBST = bst.search(key, cBST);
        const Student* sAVL = avl.search(key, cAVL);
        const Student* sHT  = ht.search(key, cHT);

        bstSuccessComps.push_back(cBST);
        avlSuccessComps.push_back(cAVL);
        htSuccessComps.push_back(cHT);

        std::string name = sBST ? sBST->name : "NOT FOUND";
        std::string dept = sBST ? sBST->dept : "-";
        float cgpa = sBST ? sBST->cgpa : 0.0f;

        std::cout << "| " << std::setw(5) << key
                  << " | " << std::setw(20) << std::left << name
                  << " | " << std::setw(5) << dept
                  << " | " << std::setw(4) << std::fixed << std::setprecision(2) << cgpa
                  << " | " << std::setw(7) << std::right << cBST
                  << " | " << std::setw(7) << cAVL
                  << " | " << std::setw(9) << cHT << " |\n";

        // Verification assertion to avoid any silent mismatch
        if (!sBST || !sAVL || !sHT) {
            std::cerr << "[CRITICAL] Key " << key << " was not found in all structures!\n";
        }
    }
    std::cout << "+-------+----------------------+-------+------+---------+---------+-----------+\n";

    double avgBstSuccess = std::accumulate(bstSuccessComps.begin(), bstSuccessComps.end(), 0.0) / bstSuccessComps.size();
    double avgAvlSuccess = std::accumulate(avlSuccessComps.begin(), avlSuccessComps.end(), 0.0) / avlSuccessComps.size();
    double avgHtSuccess  = std::accumulate(htSuccessComps.begin(), htSuccessComps.end(), 0.0) / htSuccessComps.size();

    std::cout << "| AVERAGE (Successful Searches)               | "
              << std::setw(7) << std::fixed << std::setprecision(2) << avgBstSuccess
              << " | " << std::setw(7) << avgAvlSuccess
              << " | " << std::setw(9) << avgHtSuccess << " |\n";
    std::cout << "+---------------------------------------------+---------+---------+-----------+\n\n";

    // 5 Unsuccessful Search Keys
    const std::vector<int> unsuccessfulKeys = { 999, 1004, 1020, 1054, 2000 };
    std::vector<int> bstFailComps;
    std::vector<int> avlFailComps;
    std::vector<int> htFailComps;

    std::cout << "TEST 2: 5 UNSUCCESSFUL SEARCHES (Records Absent from Dataset)\n";
    std::cout << "+-------+-------------------------------------+---------+---------+-----------+\n";
    std::cout << "| RegNo | Test Reason                         | BST Cmp | AVL Cmp | Hash Cmp  |\n";
    std::cout << "+-------+-------------------------------------+---------+---------+-----------+\n";

    const std::vector<std::string> reasons = {
        "Below minimum key range (Key < 1001)",
        "Missing intermediate key (Between 1003 & 1005)",
        "Missing intermediate key (Between 1019 & 1022)",
        "Missing key with initial hash collision (47)",
        "Above maximum key range (Key > 1080)"
    };

    for (size_t i = 0; i < unsuccessfulKeys.size(); ++i) {
        int key = unsuccessfulKeys[i];
        int cBST = 0, cAVL = 0, cHT = 0;
        const Student* sBST = bst.search(key, cBST);
        const Student* sAVL = avl.search(key, cAVL);
        const Student* sHT  = ht.search(key, cHT);

        bstFailComps.push_back(cBST);
        avlFailComps.push_back(cAVL);
        htFailComps.push_back(cHT);

        std::cout << "| " << std::setw(5) << key
                  << " | " << std::setw(35) << std::left << reasons[i]
                  << " | " << std::setw(7) << std::right << cBST
                  << " | " << std::setw(7) << cAVL
                  << " | " << std::setw(9) << cHT << " |\n";

        if (sBST || sAVL || sHT) {
            std::cerr << "[CRITICAL] Absent key " << key << " was found unexpectedly!\n";
        }
    }
    std::cout << "+-------+-------------------------------------+---------+---------+-----------+\n";

    double avgBstFail = std::accumulate(bstFailComps.begin(), bstFailComps.end(), 0.0) / bstFailComps.size();
    double avgAvlFail = std::accumulate(avlFailComps.begin(), avlFailComps.end(), 0.0) / avlFailComps.size();
    double avgHtFail  = std::accumulate(htFailComps.begin(), htFailComps.end(), 0.0) / htFailComps.size();

    std::cout << "| AVERAGE (Unsuccessful Searches)             | "
              << std::setw(7) << std::fixed << std::setprecision(2) << avgBstFail
              << " | " << std::setw(7) << avgAvlFail
              << " | " << std::setw(9) << avgHtFail << " |\n";
    std::cout << "+---------------------------------------------+---------+---------+-----------+\n\n";

    // Overall Averages
    double overallBST = (avgBstSuccess + avgBstFail) / 2.0;
    double overallAVL = (avgAvlSuccess + avgAvlFail) / 2.0;
    double overallHT  = (avgHtSuccess + avgHtFail) / 2.0;

    std::cout << "SUMMARY COMPARISON OVERALL:\n";
    std::cout << "=========================================================================================\n";
    std::cout << "Metric                        | BST           | AVL           | Hash Table (Linear Probe)\n";
    std::cout << "------------------------------+---------------+---------------+--------------------------\n";
    std::cout << "Avg Successful Comparisons    | " << std::setw(13) << avgBstSuccess << " | "
              << std::setw(13) << avgAvlSuccess << " | "
              << std::setw(24) << avgHtSuccess << "\n";
    std::cout << "Avg Unsuccessful Comparisons  | " << std::setw(13) << avgBstFail << " | "
              << std::setw(13) << avgAvlFail << " | "
              << std::setw(24) << avgHtFail << "\n";
    std::cout << "Overall Avg Comparisons       | " << std::setw(13) << overallBST << " | "
              << std::setw(13) << overallAVL << " | "
              << std::setw(24) << overallHT << "\n";
    std::cout << "Structure Height              | " << std::setw(13) << bst.getHeight() << " | "
              << std::setw(13) << avl.getHeight() << " | "
              << std::setw(24) << "N/A (Array-based)" << "\n";
    std::cout << "Total Collisions              | " << std::setw(13) << "N/A" << " | "
              << std::setw(13) << "N/A" << " | "
              << std::setw(24) << ht.getTotalCollisions() << "\n";
    std::cout << "Insertion Complexity          | O(N) Worst    | O(log N)      | O(1) Avg, O(N) Worst\n";
    std::cout << "Search Complexity             | O(N) Worst    | O(log N)      | O(1) Avg, O(N) Worst\n";
    std::cout << "Ordering Support              | In-order O(N) | In-order O(N) | None (Unordered)\n";
    std::cout << "=========================================================================================\n";
}

// Interactive Console Menu for Live Faculty Viva Demonstration
void runInteractiveMenu(BST& bst, AVL& avl, HashTable& ht) {
    int choice = 0;
    while (true) {
        std::cout << "\n=========================================================\n";
        std::cout << "      ALGORITHM BATTLE: STUDENT RECORD SEARCH ENGINE      \n";
        std::cout << "=========================================================\n";
        std::cout << " 1. Search Student Record across All 3 Data Structures   \n";
        std::cout << " 2. Insert New Student Record (Live Viva Demonstration)   \n";
        std::cout << " 3. Display In-Order Traversal (Sorted BST & AVL Keys)   \n";
        std::cout << " 4. Display Hash Table Layout (Indices & Probe Offsets)  \n";
        std::cout << " 5. Run Automated Benchmark Comparison Report            \n";
        std::cout << " 6. Exit                                                 \n";
        std::cout << "=========================================================\n";
        std::cout << "Enter your choice (1-6): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string discard;
            std::getline(std::cin, discard);
            std::cout << "[ERROR] Invalid input. Please enter a number between 1 and 6.\n";
            continue;
        }

        if (choice == 1) {
            int regNo;
            std::cout << "\nEnter Registration Number to Search: ";
            if (!(std::cin >> regNo)) {
                std::cin.clear();
                std::string discard;
                std::getline(std::cin, discard);
                std::cout << "[ERROR] Invalid Registration Number.\n";
                continue;
            }

            int cBST = 0, cAVL = 0, cHT = 0;
            const Student* sBST = bst.search(regNo, cBST);
            const Student* sAVL = avl.search(regNo, cAVL);
            const Student* sHT  = ht.search(regNo, cHT);

            // Consistency verification across all 3 data structures
            bool consistent = ((sBST != nullptr) == (sAVL != nullptr)) && ((sAVL != nullptr) == (sHT != nullptr));
            if (!consistent) {
                std::cerr << "[WARNING] Inconsistency detected between data structures!\n";
            }

            std::cout << "\n------------------ SEARCH RESULTS ------------------\n";
            if (sBST) {
                std::cout << "STATUS: RECORD FOUND!\n";
                sBST->display();
            } else {
                std::cout << "STATUS: RECORD NOT FOUND IN SYSTEM.\n";
            }
            std::cout << "---------------- COMPARISON METRICS ----------------\n";
            std::cout << " 1. Unbalanced BST Comparisons : " << cBST << " steps\n";
            std::cout << " 2. Balanced AVL Comparisons   : " << cAVL << " steps\n";
            std::cout << " 3. Hash Table Probes/Checks   : " << cHT  << " steps\n";
            std::cout << "----------------------------------------------------\n";

        } else if (choice == 2) {
            int regNo;
            std::string name, dept;
            float cgpa;

            std::cout << "\nEnter Student Details for Live Insertion:\n";
            std::cout << "Registration Number (e.g., 1085): ";
            std::cin >> regNo;
            std::cin.ignore();

            std::cout << "Full Name: ";
            std::getline(std::cin, name);

            std::cout << "Department (e.g., CSE, ECE, IT): ";
            std::getline(std::cin, dept);

            std::cout << "CGPA (0.0 - 10.0): ";
            std::cin >> cgpa;

            Student newStudent(regNo, name, dept, cgpa);

            int bstHeightBefore = bst.getHeight();
            int avlHeightBefore = avl.getHeight();
            int htCollisionsBefore = ht.getTotalCollisions();

            bst.insert(newStudent);
            avl.insert(newStudent);
            bool htOk = ht.insert(newStudent);

            std::cout << "\n[SUCCESS] Inserted Student " << regNo << " successfully into all structures!\n";
            std::cout << " - BST Height     : " << bstHeightBefore << " -> " << bst.getHeight() << "\n";
            std::cout << " - AVL Height     : " << avlHeightBefore << " -> " << avl.getHeight() << "\n";
            std::cout << " - Hash Collision : " << (ht.getTotalCollisions() > htCollisionsBefore ? "YES (Collision at hash index)" : "NO (Direct placement)") << "\n";
            if (!htOk) {
                std::cout << " [WARNING] Hash Table was unable to insert (full table).\n";
            }

        } else if (choice == 3) {
            std::cout << "\n================ IN-ORDER TRAVERSAL (BST) ================\n";
            bst.inorderTraversal();
            std::cout << "==========================================================\n";
            std::cout << "\n================ IN-ORDER TRAVERSAL (AVL) ================\n";
            avl.inorderTraversal();
            std::cout << "==========================================================\n";

        } else if (choice == 4) {
            ht.displayTable();

        } else if (choice == 5) {
            runBenchmark(bst, avl, ht);

        } else if (choice == 6) {
            std::cout << "\nExiting Algorithm Battle. All resources cleanly deallocated.\n";
            break;
        } else {
            std::cout << "[ERROR] Invalid choice. Please choose 1 - 6.\n";
        }
    }
}

int main(int argc, char* argv[]) {
    std::cout << "=========================================================================\n";
    std::cout << "  SRM INSTITUTE OF SCIENCE AND TECHNOLOGY - CLA 2 ASSIGNMENT             \n";
    std::cout << "  ALGORITHM BATTLE: 'WHICH DATA STRUCTURE WINS?'                         \n";
    std::cout << "  Domain: Student Record Search Engine                                   \n";
    std::cout << "=========================================================================\n";

    std::string datasetPath = "dataset.csv";
    std::vector<Student> dataset = loadDataset(datasetPath);

    if (dataset.empty()) {
        std::cerr << "[FATAL] Dataset is empty or could not be loaded from " << datasetPath << "\n";
        return 1;
    }

    std::cout << "[INFO] Loaded " << dataset.size() << " student records from " << datasetPath << "\n";

    // Instantiate all three data structures
    BST bst;
    AVL avl;
    HashTable ht;

    // Populate all 3 structures with the exact same records in identical insertion order
    for (const auto& student : dataset) {
        bst.insert(student);
        avl.insert(student);
        ht.insert(student);
    }

    std::cout << "[INFO] Successfully populated BST, AVL, and HashTable.\n";

    // Check if CLI flag --benchmark is passed
    if (argc > 1 && std::string(argv[1]) == "--benchmark") {
        runBenchmark(bst, avl, ht);
        return 0;
    }

    // Otherwise launch interactive CLI menu for live viva defense
    runInteractiveMenu(bst, avl, ht);

    return 0;
}
