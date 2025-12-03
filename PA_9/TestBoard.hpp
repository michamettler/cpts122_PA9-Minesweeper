#pragma once
#include "Board.hpp"
#include <iostream>
#include <string>

// AI DISCLAIMER: Test cases generated with AI
class TestBoard {
public:
    TestBoard();
    void runAllTests();

private:
    // Test helper functions
    bool assertEqual(int expected, int actual, const std::string& testName);
    bool assertTrue(bool condition, const std::string& testName);
    bool assertFalse(bool condition, const std::string& testName);
    void printTestResult(bool passed, const std::string& testName);

    // Individual test cases
    void testBoardInitialization();
    void testCursorMovement();
    void testFlagPlanting();
    void testRevealSafeTile();
    void testRevealMine();

    // Test statistics
    int totalTests;
    int passedTests;
    int failedTests;
};