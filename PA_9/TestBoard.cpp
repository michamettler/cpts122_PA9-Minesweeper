#include "TestBoard.hpp"

TestBoard::TestBoard() : totalTests(0), passedTests(0), failedTests(0) {}

void TestBoard::runAllTests() {
    std::cout << "========================================" << std::endl;
    std::cout << "Running Board Tests..." << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    testBoardInitialization();
    testCursorMovement();
    testFlagPlanting();
    testRevealSafeTile();
    testRevealMine();

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Test Results Summary" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Total Tests: " << totalTests << std::endl;
    std::cout << "Passed: " << passedTests << " ("
        << (totalTests > 0 ? (passedTests * 100 / totalTests) : 0) << "%)" << std::endl;
    std::cout << "Failed: " << failedTests << std::endl;
    std::cout << "========================================" << std::endl;
}

// ---------------------- Helper functions --------------------------

bool TestBoard::assertEqual(int expected, int actual, const std::string& testName) {
    totalTests++;
    bool passed = (expected == actual);
    if (passed) {
        passedTests++;
    }
    else {
        failedTests++;
        std::cout << "  FAILED: Expected " << expected << " but got " << actual << std::endl;
    }
    printTestResult(passed, testName);
    return passed;
}

bool TestBoard::assertTrue(bool condition, const std::string& testName) {
    totalTests++;
    if (condition) {
        passedTests++;
    }
    else {
        failedTests++;
        std::cout << "  FAILED: Condition was false" << std::endl;
    }
    printTestResult(condition, testName);
    return condition;
}

bool TestBoard::assertFalse(bool condition, const std::string& testName) {
    totalTests++;
    bool passed = !condition;
    if (passed) {
        passedTests++;
    }
    else {
        failedTests++;
        std::cout << "  FAILED: Condition was true" << std::endl;
    }
    printTestResult(passed, testName);
    return passed;
}

void TestBoard::printTestResult(bool passed, const std::string& testName) {
    if (passed) {
        std::cout << "[PASS] " << testName << std::endl;
    }
    else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

// ---------------------- Test cases --------------------------

// Test Case 1: Board Initialization
void TestBoard::testBoardInitialization() {
    std::cout << "Test 1: Board Initialization" << std::endl;
    std::cout << "----------------------------" << std::endl;

    Board board(10, 15);

    // Check board size
    assertEqual(10, board.getSize(), "Board size is correct");

    // Check mine count
    assertEqual(15, board.getMines(), "Mine count is correct");

    // Check cursor starts at (0, 0)
    assertEqual(0, board.getCursorX(), "Cursor X starts at 0");
    assertEqual(0, board.getCursorY(), "Cursor Y starts at 0");

    // Check that all tiles are initially unrevealed
    bool allUnrevealed = true;
    for (int y = 0; y < board.getSize(); y++) {
        for (int x = 0; x < board.getSize(); x++) {
            Tile* tile = board.getTile(x, y);
            if (tile && tile->isRevealed()) {
                allUnrevealed = false;
                break;
            }
        }
        if (!allUnrevealed) break;
    }
    assertTrue(allUnrevealed, "All tiles start unrevealed");

    std::cout << std::endl;
}

// Test Case 2: Cursor Movement
void TestBoard::testCursorMovement() {
    std::cout << "Test 2: Cursor Movement" << std::endl;
    std::cout << "------------------------" << std::endl;

    Board board(10, 5);

    // Move cursor right
    board.moveCursor(1, 0);
    assertEqual(1, board.getCursorX(), "Cursor moves right");
    assertEqual(0, board.getCursorY(), "Cursor Y unchanged after right move");

    // Move cursor down
    board.moveCursor(0, 1);
    assertEqual(1, board.getCursorX(), "Cursor X unchanged after down move");
    assertEqual(1, board.getCursorY(), "Cursor moves down");

    // Move cursor left
    board.moveCursor(-1, 0);
    assertEqual(0, board.getCursorX(), "Cursor moves left");

    // Move cursor up
    board.moveCursor(0, -1);
    assertEqual(0, board.getCursorY(), "Cursor moves up");

    // Test boundary - try to move out of bounds (left)
    board.moveCursor(-1, 0);
    assertEqual(0, board.getCursorX(), "Cursor stops at left boundary");

    // Test boundary - move to far right and try to go further
    for (int i = 0; i < 15; i++) {
        board.moveCursor(1, 0);
    }
    assertEqual(9, board.getCursorX(), "Cursor stops at right boundary (size-1)");

    std::cout << std::endl;
}

// Test Case 3: Flag Planting
void TestBoard::testFlagPlanting() {
    std::cout << "Test 3: Flag Planting" << std::endl;
    std::cout << "----------------------" << std::endl;

    Board board(10, 5);

    // Get tile at cursor position
    Tile* tile = board.getTile(0, 0);

    // Initially not flagged
    assertFalse(tile->isFlagged(), "Tile starts unflagged");

    // Plant flag
    board.plantFlag();
    assertTrue(tile->isFlagged(), "Tile becomes flagged after planting");

    // Remove flag (toggle)
    board.plantFlag();
    assertFalse(tile->isFlagged(), "Tile becomes unflagged after toggling");

    // Flag at different position
    board.moveCursor(2, 2);
    Tile* tile2 = board.getTile(2, 2);
    board.plantFlag();
    assertTrue(tile2->isFlagged(), "Can flag tile at different position");

    std::cout << std::endl;
}

// Test Case 4: Reveal Safe Tile
void TestBoard::testRevealSafeTile() {
    std::cout << "Test 4: Reveal Safe Tile" << std::endl;
    std::cout << "-------------------------" << std::endl;

    // Create board with few mines to increase chance of finding safe tiles
    Board board(10, 3);

    // Try to find a safe tile and reveal it
    bool foundSafeTile = false;
    bool revealSuccess = false;

    for (int y = 0; y < board.getSize() && !foundSafeTile; y++) {
        for (int x = 0; x < board.getSize() && !foundSafeTile; x++) {
            Tile* tile = board.getTile(x, y);
            if (tile && !tile->hasMine()) {
                // Found a safe tile - move cursor there and reveal
                board.moveCursor(x - board.getCursorX(), y - board.getCursorY());
                revealSuccess = board.revealTile();
                foundSafeTile = true;

                assertTrue(revealSuccess, "Revealing safe tile returns true");
                assertTrue(tile->isRevealed(), "Safe tile becomes revealed");
            }
        }
    }

    assertTrue(foundSafeTile, "Found at least one safe tile on board");

    std::cout << std::endl;
}

// Test Case 5: Reveal Mine
void TestBoard::testRevealMine() {
    std::cout << "Test 5: Reveal Mine" << std::endl;
    std::cout << "--------------------" << std::endl;

    // Create board with many mines to ensure we can find one
    Board board(10, 20);

    // Find a mine and reveal it
    bool foundMine = false;
    bool revealResult = true;

    for (int y = 0; y < board.getSize() && !foundMine; y++) {
        for (int x = 0; x < board.getSize() && !foundMine; x++) {
            Tile* tile = board.getTile(x, y);
            if (tile && tile->hasMine()) {
                // Found a mine - move cursor there and reveal
                board.moveCursor(x - board.getCursorX(), y - board.getCursorY());
                revealResult = board.revealTile();
                foundMine = true;

                assertFalse(revealResult, "Revealing mine returns false (game over)");
                assertTrue(tile->isRevealed(), "Mine becomes revealed");
            }
        }
    }

    assertTrue(foundMine, "Found at least one mine on board");

    std::cout << std::endl;
}