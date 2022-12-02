#include <iostream>
#include <algorithm>
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
    char opponentChoice{'-'};
    char myGuide{'-'};
    std::string status{""};
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

const WinRule rockDefeatsScissors{'C', 'X', "win", winPoint + rockPoint};
const WinRule PaperDefeatsRock{'A', 'Y', "win", winPoint + paperPoint};
const WinRule scissorsDefeatsPaper{'B', 'Z', "win", winPoint + scissorsPoint};

const DrawRule rockDraw{'A', 'X', "draw", rockPoint + drawPoint};
const DrawRule PaperDraw{'B', 'Y', "draw", paperPoint + drawPoint};
const DrawRule scissorsDraw{'C', 'Z', "draw", scissorsPoint + drawPoint};

const LoseRule rockLoseWithPaper{'B', 'X', "lose", rockPoint};
const LoseRule paperLoseWithScissors{'C', 'Y', "lose", paperPoint};
const LoseRule scissorsLoseWithRock{'A', 'Z', "lose", scissorsPoint};

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

constexpr char shouldWin{'Z'};
constexpr char shouldDraw{'Y'};
constexpr char shouldLose{'X'};

void run(const std::vector<std::string>&& fileContent){
    uint score{0};
    char myGuide{'-'};
    char opponentChoice{'-'};

    for(const auto& element : fileContent){
        myGuide = element.at(2);
        opponentChoice = element.at(0);
        if(myGuide == shouldLose){
            const auto it = std::ranges::find_if(allRules, [&opponentChoice](const auto& rule){
                const auto& extractedRule = rule.get();
                if(extractedRule.status == "lose" && extractedRule.opponentChoice == opponentChoice){
                    return true;
                }
                return false;
            });
            if(it != allRules.end()){
                score += it->get().admittedPoints;
            }
            continue;
        }
        if(myGuide == shouldDraw){
            const auto it = std::ranges::find_if(allRules, [&opponentChoice](const auto& rule){
                const auto& extractedRule = rule.get();
                if(extractedRule.status == "draw" && extractedRule.opponentChoice == opponentChoice){
                    return true;
                }
                return false;
            });
            if(it != allRules.end()){
                score += it->get().admittedPoints;
            }
            continue;
        }
        if(myGuide == shouldWin){
            const auto it = std::ranges::find_if(allRules, [&opponentChoice](const auto& rule){
                const auto& extractedRule = rule.get();
                if(extractedRule.status == "win" && extractedRule.opponentChoice == opponentChoice){
                    return true;
                }
                return false;
            });
            if(it != allRules.end()){
                score += it->get().admittedPoints;
            }
            continue;
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