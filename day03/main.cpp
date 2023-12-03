#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <cctype>

int main() {
    std::ifstream inputStream("input");

    std::vector<std::string> paddedInput;
    for (std::string line; std::getline(inputStream, line);) {
        paddedInput.push_back("." + line + ".");
    }

    int width = paddedInput[0].length() - 2;
    int height = paddedInput.size();

    std::string padLine = std::string(width + 2, '.');
    paddedInput.insert(paddedInput.begin(), padLine);
    paddedInput.push_back(padLine);

    int sum1 = 0;
    int sum2 = 0;

    for (int y = 1; y < height + 1; y++) {
        for (int x = 1; x < width + 1; x++) {
            char symbol = paddedInput[y][x];

            if (!isdigit(symbol) && symbol != '.') {
                std::vector<std::pair<int, int>> adjacentNumbersPos;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        char adjacentChar = paddedInput[y + i][x + j];

                        if (isdigit(adjacentChar)) {
                            adjacentNumbersPos.push_back({y + i, x + j});
                        }
                    }
                }

                // find start of numbers
                for (std::pair<int, int> &pos: adjacentNumbersPos) {
                    while (isdigit(paddedInput[pos.first][pos.second - 1])) {
                        pos.second--;
                    }
                }

                // remove duplicates
                adjacentNumbersPos.erase(std::unique(adjacentNumbersPos.begin(), adjacentNumbersPos.end()), adjacentNumbersPos.end());

                std::vector<int> adjacentNumbers;
                for (std::pair<int, int> pos: adjacentNumbersPos) {
                    // it seems that no number has more than two adjacent symbols, so erasing the numbers is not necessary

                    int end = paddedInput[pos.first].find_first_not_of("0123456789", pos.second);
                    adjacentNumbers.push_back(std::stoi(paddedInput[pos.first].substr(pos.second, end)));
                }

                sum1 += std::reduce(adjacentNumbers.begin(), adjacentNumbers.end());
                if (symbol == '*' && adjacentNumbers.size() == 2) {
                    sum2 += adjacentNumbers[0] * adjacentNumbers[1];
                }
            }
        }
    }

    std::cout << "Sum 1: " << sum1 << std::endl;
    std::cout << "Sum 2: " << sum2 << std::endl;

    return 0;
}
