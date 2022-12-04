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
    uint score{0};
    for(const auto& line : fileContent){
        uint firstNum{0};
        uint secondNum{0};
        uint thirdNum{0};
        uint fourthNum{0};
        auto founded = line.find('-');
        std::size_t lastFounded{0};
        if(founded != std::string::npos){
            firstNum = std::stoul(line.substr(0, founded));
            lastFounded = founded;
        }
        founded = line.find(',', ++lastFounded);
        if(founded != std::string::npos){
            secondNum = std::stoul(line.substr(lastFounded, founded));
            lastFounded = founded;
        }
        founded = line.find('-', ++lastFounded);
        if(founded != std::string::npos){
            thirdNum = std::stoul(line.substr(lastFounded, founded));
            lastFounded = founded;
        }
        fourthNum = std::stoul(line.substr(++lastFounded, line.size()));

        if(firstNum >= thirdNum && secondNum <= fourthNum){
            ++score;
            continue;
        }
        if(thirdNum >= firstNum && fourthNum <= secondNum){
            ++score;
        }
    }
    std::cout << score << "\n";
}

int main(){
    FileReader fr{fileName};
    fr.proceedFile();

    run(fr.getFileContent());
    
    return 0;
}