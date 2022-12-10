#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <cmath>

#include "../../helpful/fileReader.hpp"

#ifdef TEST
    const std::string fileName{"../test_input.txt"};
#else
    const std::string fileName{"../input.txt"};
#endif

// Surface

struct Field{
    bool visited{false};
    std::size_t posVertical{0};
    std::size_t posHorizontal{0};
};

// MoveInstruction

struct MoveInstruction{
    char move{0};
    std::size_t howManyTimes{0};
};

// Pipes

struct Body{
    std::size_t posVertical{0};
    std::size_t posHorizontal{0};

    bool operator==(const Body& otherBody) const{
        return this->posVertical == otherBody.posVertical && this->posHorizontal == otherBody.posHorizontal; 
    }
};

// Checks

bool isHeadOnTailAfterMove(const Body& head, const Body& tail) {
    return head == tail;
}

// Head moves

void moveHeadUp(Body& head){
    --head.posVertical;
}

void moveHeadDown(Body& head){
    ++head.posVertical;
}

void moveHeadRight(Body& head){
    ++head.posHorizontal;
}

void moveHeadLeft(Body& head){
    --head.posHorizontal;
}

void moveHead(const MoveInstruction& moveInstruction, Body& head){
    if(moveInstruction.move == 'U'){
        moveHeadUp(head);
    } else if(moveInstruction.move == 'D'){
        moveHeadDown(head);
    } else if(moveInstruction.move == 'R'){
        moveHeadRight(head);
    } else if(moveInstruction.move == 'L'){
        moveHeadLeft(head);
    } else {
        std::cout << "Wrong!\n";
        throw;
    }
}

// Tail moves

void moveTailWhereHeadWasPreviously(const Body& head, Body& tail){
    tail.posHorizontal = head.posHorizontal;
    tail.posVertical = head.posVertical;
}

bool isHeadChangedOneLevelVertically(const Body& head, const Body& tail){
    return (head.posVertical - tail.posVertical) == 1 || (tail.posVertical - head.posVertical) == 1;
}

bool isHeadChangedTwoLevelsVertically(const Body& head, const Body& tail){
    return (head.posVertical - tail.posVertical) == 2 || (tail.posVertical - head.posVertical) == 2;
}

bool isHorizontalyOnePositionAhead(const Body& head, const Body& tail){
    return (head.posHorizontal - tail.posHorizontal) == 1 || (tail.posHorizontal - head.posHorizontal) == 1;
}

// Fields creator

using TwoDimensionalArray = std::vector<std::vector<Field>>;

void markTailPositionOnField(const Body& tail, TwoDimensionalArray& fields){
    fields[tail.posVertical][tail.posHorizontal].visited = true;
}

TwoDimensionalArray createEmptyFields(const std::size_t vertically, const std::size_t horizontally){
    TwoDimensionalArray result{};
    result.resize(vertically);

    std::size_t currentVertical{0};
    std::size_t currentHorizontal{0};

    for(auto& eachRow : result){
        eachRow.resize(horizontally);
        for(auto& field : eachRow){
            field.posVertical = currentVertical;
            field.posHorizontal = currentHorizontal;
            ++currentHorizontal;
        }
        currentHorizontal = 0;
        ++currentVertical;
    }
    return result;
}

void visualization(const TwoDimensionalArray& fields, const Body& head, const Body& tail){
    
    if(!(head == tail)){
    for(auto& vert : fields){
         for(auto& hr : vert){
                if(hr.posHorizontal == head.posHorizontal && hr.posVertical == head.posVertical){
                    std::cout << "H";
                }
                if(hr.posHorizontal == tail.posHorizontal && hr.posVertical == tail.posVertical){
                        std::cout << "T";
                } 
                if(!(hr.posHorizontal == head.posHorizontal && hr.posVertical == head.posVertical)
                    && !(hr.posHorizontal == tail.posHorizontal && hr.posVertical == tail.posVertical)){
                        std::cout << ".";
                }
         }
         std::cout << "\n";
        }
    } else {
        for(auto& vert : fields){
         for(auto& hr : vert){
                if(hr.posHorizontal == head.posHorizontal && hr.posVertical == head.posVertical){
                    std::cout << "H";
                }
                if(!(hr.posHorizontal == head.posHorizontal && hr.posVertical == head.posVertical)){
                        std::cout << ".";
                }
         }
         std::cout << "\n";
        }
    }
    using namespace std::chrono_literals;
    // std::this_thread::sleep_for(200ms);
    std::getchar();
    system("clear");
}

std::vector<MoveInstruction> parseInput(const std::vector<std::string>&& fileContent){
    std::vector<MoveInstruction> result{};
    result.reserve(fileContent.size());
    for(const auto& line : fileContent){
        result.push_back({line.at(0), std::stoul(line.substr(2))});
    }
    return result;
}

void run(const std::vector<std::string>&& fileContent){
    const std::size_t vertically{777}; // 14 for tests and visualization or 777 for main input
    const std::size_t horizontally{777}; // 14 for tests and visualization or 777 for main input
    auto fields = createEmptyFields(vertically, horizontally);
    const auto input = parseInput(std::forward<const std::vector<std::string>>(fileContent));

    Body head{vertically/2, horizontally/2};
    Body tail{vertically/2, horizontally/2};

    system("clear");
    for(const auto& moveInstruction : input){
        for(std::size_t move{0}; move < moveInstruction.howManyTimes; ++move){
            const Body previousHead{head};
            moveHead(moveInstruction, head);
            if(!isHeadChangedOneLevelVertically(head, tail) || !isHorizontalyOnePositionAhead(head, tail)){
                if((!isHorizontalyOnePositionAhead(head, tail) && !(head.posHorizontal == tail.posHorizontal)) || isHeadChangedTwoLevelsVertically(head, tail)){      
                    moveTailWhereHeadWasPreviously(previousHead, tail);
                }
            }
            markTailPositionOnField(tail, fields);
            // visualization(fields, head, tail);
        }
    }
    std::size_t score{0};
    for(const auto& field : fields){
        score += std::count_if(field.begin(), field.end(), [](const auto& position){
            return position.visited;
        });
    }

    std::cout << score << "\n";
}

int main(){
    FileReader fr{fileName};
    fr.proceedFile();

    run(fr.getFileContent());
    
    return 0;
}