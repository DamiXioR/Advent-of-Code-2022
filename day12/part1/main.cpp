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
#include <numeric>
#include <climits>

#include "../../helpful/fileReader.hpp"

#ifdef TEST
    const std::string fileName{"../test_input.txt"};
#else
    const std::string fileName{"../input.txt"};
#endif

struct Point{
    int vertical{0};
    int horizontal{0};

    bool operator==(const Point& otherPoint) const{
        return this->vertical == otherPoint.vertical && this->horizontal == otherPoint.horizontal;
    }
    bool operator!=(const Point& otherPoint) const{
        return this->vertical != otherPoint.vertical || this->horizontal != otherPoint.horizontal;
    }
};

Point getPosOfStartEndPoint(const std::vector<std::string>&& fileContent, bool isStart = true){
    const char requestedPoint = isStart ? 'S' : 'E';
    for(auto vertically{0}; vertically < fileContent.size(); ++vertically){
        const auto horizontally = fileContent[vertically].find(requestedPoint);
        if(horizontally != std::string::npos){
            return Point{vertically, static_cast<int>(horizontally)};
        }
    }
    return Point{0,0};
}

using TwoDimensionalIntArray = std::vector<std::vector<int>>;

TwoDimensionalIntArray createMatrixOfNumbersFromCharacters(const std::vector<std::string>&& fileContent, 
                                                        const std::size_t maxVertical, 
                                                        const std::size_t maxHorizontal){ 
    TwoDimensionalIntArray result{};
    result.resize(maxVertical);
    for(auto& eachRow : result){
        eachRow.resize(maxHorizontal);
    }

    std::transform(fileContent.begin(), fileContent.end(), result.begin(),result.begin(), [](const auto& textRow, auto& numRow){
        std::transform(textRow.begin(), textRow.end(), numRow.begin(),numRow.begin(), [](const auto& letter, auto& number){
            // 96 - cast char to num adjustment
            return letter == 'S' ? 0 : (letter == 'E' ? 27 : static_cast<int>(letter) - 96);
        });
        return numRow;
    });

    return result;
}

void printMatrix(const TwoDimensionalIntArray& matrix){
    for(const auto& row : matrix){
        for(const auto& horizontal : row){
            std::cout << horizontal << "\t";
        }
        std::cout << "\n";
    }
}

// Matrix traversing

using TwoDimensionalPointArray = std::vector<std::vector<Point>>;

void movePoint(const TwoDimensionalIntArray& matrix,
                Point currentPosition, 
                const std::size_t maxVertical, 
                const std::size_t maxHorizontal, 
                const Point& endPos, 
                std::vector<Point> visitedPoints,
                TwoDimensionalPointArray& pathsThatAchievesEndPos,
                TwoDimensionalPointArray& rejectedPathsThatNotAchievesEndPos);

void matrixTraversing(const TwoDimensionalIntArray& matrix, const Point& startPos, const Point& endPos){
    const auto maxVertical{matrix.size()};
    const auto maxHorizontal{matrix.front().size()};
    Point currentPosition = startPos;
    
    std::vector<Point> visitedPoints;
    visitedPoints.reserve(maxVertical * maxHorizontal);
    visitedPoints.push_back(currentPosition); // current Position is visited

    TwoDimensionalPointArray pathsThatAchievesEndPos;
    pathsThatAchievesEndPos.reserve(maxVertical * maxHorizontal);

    TwoDimensionalPointArray rejectedPathsThatNotAchievesEndPos;
    rejectedPathsThatNotAchievesEndPos.reserve(maxVertical * maxHorizontal);
 
    movePoint(matrix, currentPosition, maxVertical, maxHorizontal, endPos, visitedPoints, pathsThatAchievesEndPos, rejectedPathsThatNotAchievesEndPos);

    using PairsOfStepsAndPathPoints = std::vector<std::pair<int, int>>; 
    PairsOfStepsAndPathPoints pathsInfo{};
    pathsInfo.reserve(pathsThatAchievesEndPos.size());
    for(const auto& row : pathsThatAchievesEndPos){
        int points{0};
        for(const auto& point : row){
            points += matrix[point.vertical][point.horizontal];
        }
        pathsInfo.push_back(std::make_pair(points, row.size() - 1));
        points = 0;
    }

    int minPoints{INT_MAX};
    int minSteps{minPoints};

    for(const auto& item : pathsInfo){
        minPoints = std::min(minPoints,item.first);
    }

    auto iter = pathsInfo.begin();
    while((iter = std::find_if(iter, pathsInfo.end(),[minPoints](const auto& item){
        return item.first == minPoints;
    })) != pathsInfo.end()){
        minSteps = std::min(iter->second, minSteps);
        ++iter;
    }

    std::cout << "Min points: " << minPoints << " and min steps: " << minSteps << "\n";
}

/// Possible Moves
//// Up
void goUp(const TwoDimensionalIntArray& matrix,
            Point currentPosition,  // should be copy
            const std::size_t maxVertical, 
            const std::size_t maxHorizontal, 
            const Point& endPos,
            std::vector<Point> visitedPoints,   // should be copy
            TwoDimensionalPointArray& pathsThatAchievesEndPos,
            TwoDimensionalPointArray& rejectedPathsThatNotAchievesEndPos){

    if(currentPosition.vertical == 0){
        rejectedPathsThatNotAchievesEndPos.push_back(visitedPoints);
        return;
    }
    Point upperPosition{currentPosition.vertical - 1, currentPosition.horizontal};
    if(upperPosition == endPos && (matrix[currentPosition.vertical][currentPosition.horizontal] == 26)){
        currentPosition = upperPosition;
        visitedPoints.push_back(currentPosition);
        pathsThatAchievesEndPos.push_back(visitedPoints);
        return;
    } 

    const auto hasUpperPositionTheSameValue{matrix[upperPosition.vertical][upperPosition.horizontal] == matrix[currentPosition.vertical][currentPosition.horizontal]};
    const auto hasUpperPositionBiggerByOneValue{matrix[upperPosition.vertical][upperPosition.horizontal] == matrix[currentPosition.vertical][currentPosition.horizontal] + 1};
    const auto isPointExistsInVisitedPoints = std::find(visitedPoints.begin(), visitedPoints.end(), upperPosition);
    const auto pointWasNotVisitedPreviously = isPointExistsInVisitedPoints == visitedPoints.end();
    if((hasUpperPositionTheSameValue || hasUpperPositionBiggerByOneValue) && pointWasNotVisitedPreviously){
        currentPosition = upperPosition;
        visitedPoints.push_back(currentPosition);
        movePoint(matrix, upperPosition, maxVertical, maxHorizontal, endPos, visitedPoints, pathsThatAchievesEndPos, rejectedPathsThatNotAchievesEndPos);
        return;
    }
    rejectedPathsThatNotAchievesEndPos.push_back(visitedPoints);
    return;
}

//// Down
void goDown(const TwoDimensionalIntArray& matrix,
            Point currentPosition,  // should be copy
            const std::size_t maxVertical, 
            const std::size_t maxHorizontal, 
            const Point& endPos,
            std::vector<Point> visitedPoints,   // should be copy
            TwoDimensionalPointArray& pathsThatAchievesEndPos,
            TwoDimensionalPointArray& rejectedPathsThatNotAchievesEndPos){
 
    if(currentPosition.vertical == maxVertical - 1){
        rejectedPathsThatNotAchievesEndPos.push_back(visitedPoints);
        return;
    }
    Point bottomPosition{currentPosition.vertical + 1, currentPosition.horizontal};
    if(bottomPosition == endPos && (matrix[currentPosition.vertical][currentPosition.horizontal] == 26)){
        currentPosition = bottomPosition;
        visitedPoints.push_back(currentPosition);
        pathsThatAchievesEndPos.push_back(visitedPoints);
        return;
    } 

    const auto hasBottomPositionTheSameValue{matrix[bottomPosition.vertical][bottomPosition.horizontal] == matrix[currentPosition.vertical][currentPosition.horizontal]};
    const auto hasBottomPositionBiggerByOneValue{matrix[bottomPosition.vertical][bottomPosition.horizontal] == matrix[currentPosition.vertical][currentPosition.horizontal] + 1};
    const auto isPointExistsInVisitedPoints = std::find(visitedPoints.begin(), visitedPoints.end(), bottomPosition);
    const auto pointWasNotVisitedPreviously = isPointExistsInVisitedPoints == visitedPoints.end();
    if((hasBottomPositionTheSameValue || hasBottomPositionBiggerByOneValue) && pointWasNotVisitedPreviously){
        currentPosition = bottomPosition;
        visitedPoints.push_back(currentPosition);
        movePoint(matrix, bottomPosition, maxVertical, maxHorizontal, endPos, visitedPoints, pathsThatAchievesEndPos, rejectedPathsThatNotAchievesEndPos);
        return;
    }
    rejectedPathsThatNotAchievesEndPos.push_back(visitedPoints);
    return;
}

//// Right
void goRight(const TwoDimensionalIntArray& matrix,
            Point currentPosition,  // should be copy
            const std::size_t maxVertical, 
            const std::size_t maxHorizontal, 
            const Point& endPos,
            std::vector<Point> visitedPoints,   // should be copy
            TwoDimensionalPointArray& pathsThatAchievesEndPos,
            TwoDimensionalPointArray& rejectedPathsThatNotAchievesEndPos){

    if(currentPosition.horizontal == maxHorizontal - 1){
        rejectedPathsThatNotAchievesEndPos.push_back(visitedPoints);
        return;
    }

    Point rightPosition{currentPosition.vertical, currentPosition.horizontal + 1};
    if(rightPosition == endPos && (matrix[currentPosition.vertical][currentPosition.horizontal] == 26)){
        currentPosition = rightPosition;
        visitedPoints.push_back(currentPosition);
        pathsThatAchievesEndPos.push_back(visitedPoints);
        return;
    } 

    const auto hasRightPositionTheSameValue{matrix[rightPosition.vertical][rightPosition.horizontal] == matrix[currentPosition.vertical][currentPosition.horizontal]};
    const auto hasRightPositionBiggerByOneValue{matrix[rightPosition.vertical][rightPosition.horizontal] == matrix[currentPosition.vertical][currentPosition.horizontal] + 1};
    const auto isPointExistsInVisitedPoints = std::find(visitedPoints.begin(), visitedPoints.end(), rightPosition);
    const auto pointWasNotVisitedPreviously = isPointExistsInVisitedPoints == visitedPoints.end();
    if((hasRightPositionTheSameValue || hasRightPositionBiggerByOneValue) && pointWasNotVisitedPreviously){
        currentPosition = rightPosition;
        visitedPoints.push_back(currentPosition);
        movePoint(matrix, rightPosition, maxVertical, maxHorizontal, endPos, visitedPoints, pathsThatAchievesEndPos, rejectedPathsThatNotAchievesEndPos);
        return;
    }
    rejectedPathsThatNotAchievesEndPos.push_back(visitedPoints);
    return;
}

//// Left
void goLeft(const TwoDimensionalIntArray& matrix,
            Point currentPosition,  // should be copy
            const std::size_t maxVertical, 
            const std::size_t maxHorizontal, 
            const Point& endPos,
            std::vector<Point> visitedPoints,   // should be copy
            TwoDimensionalPointArray& pathsThatAchievesEndPos,
            TwoDimensionalPointArray& rejectedPathsThatNotAchievesEndPos){
   
    if(currentPosition.horizontal == 0){
        rejectedPathsThatNotAchievesEndPos.push_back(visitedPoints);
        return;
    }

    Point leftPosition{currentPosition.vertical, currentPosition.horizontal - 1};
    if(leftPosition == endPos && (matrix[currentPosition.vertical][currentPosition.horizontal] == 26)){
        currentPosition = leftPosition;
        visitedPoints.push_back(currentPosition);
        pathsThatAchievesEndPos.push_back(visitedPoints);
        return;
    } 

    const auto hasRightPositionTheSameValue{matrix[leftPosition.vertical][leftPosition.horizontal] == matrix[currentPosition.vertical][currentPosition.horizontal]};
    const auto hasRightPositionBiggerByOneValue{matrix[leftPosition.vertical][leftPosition.horizontal] == matrix[currentPosition.vertical][currentPosition.horizontal] + 1};
    const auto isPointExistsInVisitedPoints = std::find(visitedPoints.begin(), visitedPoints.end(), leftPosition);
    const auto pointWasNotVisitedPreviously = isPointExistsInVisitedPoints == visitedPoints.end();
    if((hasRightPositionTheSameValue || hasRightPositionBiggerByOneValue) && pointWasNotVisitedPreviously){
        currentPosition = leftPosition;
        visitedPoints.push_back(currentPosition);
        movePoint(matrix, leftPosition, maxVertical, maxHorizontal, endPos, visitedPoints, pathsThatAchievesEndPos, rejectedPathsThatNotAchievesEndPos);
        return;
    }
    rejectedPathsThatNotAchievesEndPos.push_back(visitedPoints);
    return;
}

// Move Point

void movePoint(const TwoDimensionalIntArray& matrix,
                Point currentPosition, 
                const std::size_t maxVertical, 
                const std::size_t maxHorizontal, 
                const Point& endPos, 
                std::vector<Point> visitedPoints,
                TwoDimensionalPointArray& pathsThatAchievesEndPos,
                TwoDimensionalPointArray& rejectedPathsThatNotAchievesEndPos){
    
    goUp(matrix, currentPosition, maxVertical, maxHorizontal, endPos, visitedPoints, pathsThatAchievesEndPos, rejectedPathsThatNotAchievesEndPos);
    goDown(matrix, currentPosition, maxVertical, maxHorizontal, endPos, visitedPoints, pathsThatAchievesEndPos, rejectedPathsThatNotAchievesEndPos);
    goRight(matrix, currentPosition, maxVertical, maxHorizontal, endPos, visitedPoints, pathsThatAchievesEndPos, rejectedPathsThatNotAchievesEndPos);
    goLeft(matrix, currentPosition, maxVertical, maxHorizontal, endPos, visitedPoints, pathsThatAchievesEndPos, rejectedPathsThatNotAchievesEndPos);
}

// Main program

void run(const std::vector<std::string>&& fileContent){
    const auto maxVertical{fileContent.size()};
    const auto maxHorizontal{fileContent.front().size()};

    TwoDimensionalIntArray matrix = createMatrixOfNumbersFromCharacters(std::forward<const std::vector<std::string>>(fileContent), 
                                                                    maxVertical, 
                                                                    maxHorizontal);
    const auto startPos = getPosOfStartEndPoint(std::forward<const std::vector<std::string>>(fileContent));
    const auto endPos = getPosOfStartEndPoint(std::forward<const std::vector<std::string>>(fileContent), false);

    matrixTraversing(matrix, startPos, endPos);

    //printMatrix(matrix);
}

int main(){
    FileReader fr{fileName};
    fr.proceedFile();

    run(fr.getFileContent());
    
    return 0;
}