#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <iterator>

#include "../../helpful/fileReader.hpp"

#ifdef TEST
    const std::string fileName{"../test_input.txt"};
#else
    const std::string fileName{"../input.txt"};
#endif

const uint parsePoints(const char letter){
    if(letter >= 'a' && letter <= 'z'){
        return static_cast<uint>(letter) - 96;
    }
    return static_cast<uint>(letter) - 38;
}

void run(const std::vector<std::string>&& fileContent){
    uint score{0};
    uint currentElf{0};
    constexpr uint maxNumElves{3};
    constexpr uint expectedOccurrence = maxNumElves;
    std::map<const char, uint> itemsOccurrence;
    for(const auto& line : fileContent){
       if(currentElf < maxNumElves){
            std::string mutableLine = line;
            std::ranges::sort(mutableLine);
            const auto newEnd = std::unique(mutableLine.begin(), mutableLine.end());
            mutableLine.erase(newEnd,mutableLine.end());
            for(const auto item : mutableLine){
                const auto isInserted = itemsOccurrence.emplace(item, 1); 
                if(!isInserted.second){
                    itemsOccurrence[item] += 1;
                }
            }
            ++currentElf;
       } 
       if (currentElf == maxNumElves) {
            const auto it = std::ranges::find_if(itemsOccurrence, [expectedOccurrence](const auto item){  
                if(item.second == expectedOccurrence){
                    return true;
                }
                return false;
            });
            if(it != itemsOccurrence.end()){
                score += parsePoints(it->first);
            }
            currentElf = 0;
            itemsOccurrence.clear();
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
