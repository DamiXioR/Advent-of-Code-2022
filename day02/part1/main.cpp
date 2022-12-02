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

struct Rule{
    std::string game{""};
    uint admittedPoints{0};
};

struct WinRule : Rule{};
struct DrawRule : Rule{};
struct LoseRule : Rule{};

constexpr uint rockPoint{1};
constexpr uint paperPoint{2};
constexpr uint scissorsPoint{3};
constexpr uint winPoint{6};
constexpr uint drawPoint{3};

const WinRule rockDefeatsScissors{"C X", winPoint + rockPoint};
const WinRule PaperDefeatsRock{"A Y", winPoint + paperPoint};
const WinRule scissorsDefeatsPaper{"B Z", winPoint + scissorsPoint};

const DrawRule rockDraw{"A X", rockPoint + drawPoint};
const DrawRule PaperDraw{"B Y", paperPoint + drawPoint};
const DrawRule scissorsDraw{"C Z", scissorsPoint + drawPoint};

const LoseRule rockLoseWithPaper{"B X", rockPoint};
const LoseRule paperLoseWithScissors{"C Y", paperPoint};
const LoseRule scissorsLoseWithRock{"A Z", scissorsPoint};

const std::vector<std::reference_wrapper<const Rule>> allRules{
    rockDefeatsScissors,
    PaperDefeatsRock,
    scissorsDefeatsPaper,
    rockDraw,
    PaperDraw,
    scissorsDraw,
    rockLoseWithPaper,
    paperLoseWithScissors,
    scissorsLoseWithRock
};

void run(const std::vector<std::string>&& fileContent){
    uint score{0};
    for(const auto& element : fileContent){
        const auto it = std::find_if(allRules.begin(), allRules.end(),[&element](const auto& rule){
            if(rule.get().game == element){
                return true;
            }
            return false;
        });
        if (it != allRules.end()){
            score += it->get().admittedPoints;
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