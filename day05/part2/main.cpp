#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <fstream>

#include "../../helpful/fileReader.hpp"

#ifdef TEST
    const std::string fileName{"../test_input.txt"};
#else
    const std::string fileName{"../input.txt"};
#endif

void run(const std::vector<std::string>&& fileContent){
    std::vector<std::string> stacks{};
    std::string numsBelowStacks{""};
    std::size_t charToUintCastingAdjustment{48};

    // parse number of stacks
    for(const auto& line : fileContent){
        const auto firstNumInLastCharacter = *std::prev(line.end(),2);
        if(isdigit(firstNumInLastCharacter)){
            const auto howManyStacks = (std::size_t)firstNumInLastCharacter - charToUintCastingAdjustment;
            stacks.resize(howManyStacks);
            numsBelowStacks = line;
            break;
        }
    }

    // fill stacks
    for(const auto& line : fileContent){
        if(line == numsBelowStacks){
            break;
        }
        std::size_t startPos{0};
        std::size_t endPos{0};
        const std::string delimiter{'['};

        while ((endPos = line.find(delimiter, startPos)) != std::string::npos) {
            startPos = endPos + 1;
            std::size_t indexAdjustment{1};
            const auto where = (std::size_t)(numsBelowStacks.at(startPos) - charToUintCastingAdjustment) - indexAdjustment;
            stacks.at(where).push_back(line.at(startPos));
        }
    }  

    // main program
    const auto founded = std::find(fileContent.begin(), fileContent.end(), numsBelowStacks);
    const std::size_t diffBetweenEndOfStacksAndInstructions{2};
    std::size_t linesToSkip = std::distance(fileContent.begin(),founded) + diffBetweenEndOfStacksAndInstructions;
    
    for(const auto& line : fileContent){    
        if(linesToSkip){
            --linesToSkip;
            continue;
        }

        std::vector<std::size_t> digits;
        bool isDigit{false};
        std::string sDigit{""};
        
        for(const auto letter : line){
            if(isdigit(letter)){
                isDigit = true;
                sDigit.push_back(letter);
                continue;
            }
            if(isDigit){
                digits.push_back(std::stoul(sDigit));
                isDigit = false;
                sDigit = "";
            } 
        }
        digits.push_back(std::stoul(sDigit)); 

        auto& to {stacks.at(digits.at(2) - 1)};
        auto& from {stacks.at(digits.at(1) - 1)};
        const auto& move {digits.at(0)};

        std::string newPart{""};
        std::size_t topPos{0};
        newPart = from.substr(topPos, move);
        to.insert(topPos, newPart);
        from.erase(topPos, move);
    }

    // result
    for(const auto& element : stacks){
        std::cout << element.at(0);
    }
    std::cout << "\n";
}

int main(){
    FileReader fr{fileName};
    fr.proceedFile();

    run(fr.getFileContent());
    
    return 0;
}