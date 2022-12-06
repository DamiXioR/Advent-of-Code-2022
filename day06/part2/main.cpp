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
    
    const auto& buffer = fileContent[0];
    const uint sizeOfMarker{14};

    auto endOfPotentialMarker = buffer.begin();
    std::advance(endOfPotentialMarker, sizeOfMarker);
    const auto endOfBuffer = buffer.end();

    while(endOfPotentialMarker != endOfBuffer){
        std::string partOfBuffer{std::prev(endOfPotentialMarker, sizeOfMarker), endOfPotentialMarker};
        std::ranges::sort(partOfBuffer);
        const auto newEnd = std::unique(partOfBuffer.begin(), partOfBuffer.end());
        std::string partOfBufferWithNewEnd{partOfBuffer.begin(), newEnd};
        if(partOfBufferWithNewEnd.size() == sizeOfMarker){
            break;
        }
        ++endOfPotentialMarker;
    }

    std::cout << std::distance(buffer.begin(), endOfPotentialMarker);
}

int main(){
    FileReader fr{fileName};
    fr.proceedFile();

    run(fr.getFileContent());
    
    return 0;
}