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

struct Compartment{
    std::string firstCompartment{""};
    std::string secondCompartment{""};
};

Compartment parseItems(const std::string& items){
    std::size_t half{items.length()/2u};
    std::string firstCompartment{items.substr(0,half)};
    std::string secondCompartment{items.substr(half)};
    return Compartment{firstCompartment, secondCompartment};
}

const uint parsePoints(const char letter){
    if(letter >= 'a' && letter <= 'z'){
        return static_cast<uint>(letter) - 96;
    }
    return static_cast<uint>(letter) - 38;
}

void run(const std::vector<std::string>&& fileContent){
    uint score{0};
    for(const auto& element : fileContent){
        const Compartment comp = parseItems(element);
        const std::string firstCompartment = comp.firstCompartment;
        const std::string secondCompartment = comp.secondCompartment;

        const auto it = std::ranges::find_if(firstCompartment, [&secondCompartment](const auto& fItem){
            for(const auto& sItem : secondCompartment){
                if(fItem == sItem){
                    return true;
                }
            }
            return false;
        });
        if(it != firstCompartment.end()){
            score += parsePoints(*it);
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