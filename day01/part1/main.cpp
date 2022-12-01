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

void run(std::vector<std::string>&& fileContent){
    uint maxSum{0};
    uint currentSum{0};
    for(const auto& element : fileContent){
        if(element.empty()){
            if(currentSum > maxSum){
                maxSum = currentSum;
                currentSum = 0;
            }
            currentSum = 0;
        } else {
            currentSum += std::stoul(element);
        }
    }
    std::cout << maxSum << "\n";
}

int main(){
    FileReader fr{fileName};
    fr.proceedFile();

    run(fr.getFileContent());

    return 0;
}