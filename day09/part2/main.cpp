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
    bool reservedForPrint{false};
    int posVertical{0};
    int posHorizontal{0};
};

// MoveInstruction

struct MoveInstruction{
    char move{0};
    std::size_t howManyTimes{0};
};

// Pipes

struct Body{
    int posVertical{0};
    int posHorizontal{0};
    char printMark{'-'};

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

bool isHeadAndTailHorizontallyTheSame(const Body& head, const Body& tail){
    /*
        ......
        ...H..
        ..T...
    */
    return head.posHorizontal == tail.posHorizontal;
}

bool isHeadAndTailVerticallyTheSame(const Body& head, const Body& tail){
    /*
        ...H..
        ......
        ..T...
    */
    return head.posVertical == tail.posVertical;
}

bool isHeadAdjacentToTail(const Body& head, const Body& tail){
    /*
        head going up/down or walking around                  
        ......          ..H..       ...H.       .....       ..T..
        ..TH..    or    ..T..   or  ..T..   or  ..H..   or  .H...
    */
    const auto verticalDifference = head.posVertical - tail.posVertical;
    const auto horizontalDifference = head.posHorizontal - tail.posHorizontal;
    return (std::abs(verticalDifference) <= 1 && std::abs(horizontalDifference) <= 1);
}

void moveTail(const Body& head, Body& tail){
    if(isHeadAdjacentToTail(head, tail)){
        return;
    }
    if(isHeadAndTailVerticallyTheSame(head, tail)){
        if(head.posHorizontal > tail.posHorizontal){
            ++tail.posHorizontal;
        } else {
            --tail.posHorizontal;
        }
    } else if (isHeadAndTailHorizontallyTheSame(head, tail)){
        if(head.posVertical > tail.posVertical){
            ++tail.posVertical;
        } else {
            --tail.posVertical ;
        }
    } else {
        if(head.posVertical > tail.posVertical){
            ++tail.posVertical;
        } else {
            --tail.posVertical;
        }
        if(head.posHorizontal > tail.posHorizontal){
            ++tail.posHorizontal;
        } else {
            --tail.posHorizontal;
        }
    }
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

void visualization(TwoDimensionalArray& fields, const std::vector<Body>& pipes){
    
        for(auto& vert : fields){
            for(auto hr : vert){
                for(const auto& pipe : pipes){
                    if(hr.posHorizontal == pipe.posHorizontal && hr.posVertical == pipe.posVertical && !hr.reservedForPrint){
                        std::cout << pipe.printMark;
                        hr.reservedForPrint = true;

                    }
                }
                if(!hr.reservedForPrint){
                            std::cout << ".";
                }     
            }
            std::cout << "\n";
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

    std::vector<Body> pipes {
        {vertically/2, horizontally/2, 'H'},
        {vertically/2, horizontally/2, '1'},
        {vertically/2, horizontally/2, '2'},
        {vertically/2, horizontally/2, '3'},
        {vertically/2, horizontally/2, '4'},
        {vertically/2, horizontally/2, '5'},
        {vertically/2, horizontally/2, '6'},
        {vertically/2, horizontally/2, '7'},
        {vertically/2, horizontally/2, '8'},
        {vertically/2, horizontally/2, '9'},
    };
    Body& head = pipes.front();
    Body& tail = pipes.back();

    system("clear");
    for(const auto& moveInstruction : input){
        for(std::size_t move{0}; move < moveInstruction.howManyTimes; ++move){
            moveHead(moveInstruction, head);
            for(auto pipe{0}, nextPipe{1}; nextPipe < pipes.size(); ++pipe, ++nextPipe){
                const Body& currentHead{pipes[pipe]};
                Body& currentTail{pipes[nextPipe]};
                moveTail(currentHead, currentTail);
            }
            markTailPositionOnField(tail, fields);
            // visualization(fields, pipes);
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