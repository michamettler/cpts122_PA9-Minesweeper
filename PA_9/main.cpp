#include "Game.hpp"
#include "TestBoard.hpp"
#include <iostream>

int main() {
    // Run tests first
    std::cout << "Starting Minesweeper Tests..." << std::endl;
    std::cout << std::endl;

    TestBoard test;
    test.runAllTests();

    std::cout << std::endl;
    std::cout << "Press Enter to start the game..." << std::endl;
    std::cin.get();

    // Start the game after tests
    Game game;
    game.run();

    return 0;
}