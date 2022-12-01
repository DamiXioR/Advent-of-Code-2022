#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "../../helpful/fileReader.hpp"

#ifdef TEST
    const std::string fileName{"../test_input.txt"};
#else
    const std::string fileName{"../input.txt"};
#endif

namespace ranges = std::ranges;

/************************* PROGRAM ****************************/

void run(std::vector<std::string>&& fileContent){
    std::vector<uint> sums;

    
    uint currentSum{0};
    for(const auto& element : fileContent){
        if(element.empty()){
            sums.push_back(currentSum);
            currentSum = 0;
        } else {
            currentSum += std::stoul(element);
        }
    }

    uint maxSum{0};
    uint numTopElements{3};
    for(uint i{0}; i < numTopElements; ++i){
        auto it = ranges::max_element(sums);
        maxSum += *it;
        sums.erase(it);

    }
    std::cout << maxSum << "\n";
}

/*************************** MAIN ******************************/

int main(){
    FileReader fr{fileName};
    fr.proceedFile();

    run(fr.getFileContent());

    return 0;
}