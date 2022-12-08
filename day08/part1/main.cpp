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

using TwoDimensionalArray = std::vector<std::vector<std::size_t>>;

TwoDimensionalArray packContentIntoArrays(const std::vector<std::string>& fileContent){
    const std::size_t vertically{fileContent.size()};
    const std::size_t horizontally{fileContent.front().size()};
    
    TwoDimensionalArray result{};
    result.resize(vertically);
    for(auto& eachRow : result){
        eachRow.resize(horizontally);
    }

    std::transform(fileContent.begin(), fileContent.end(), result.begin(),result.begin(), [](const auto& textRow, auto& numRow){
        std::transform(textRow.begin(), textRow.end(), numRow.begin(),numRow.begin(), [](const auto& letter, auto& number){
            // 48 - cast char to num adjustment
            return static_cast<std::size_t>(letter) - 48;
        });
        return numRow;
    });

    return result;
    
}

void run(const std::vector<std::string>&& fileContent){
    const auto tdArray = packContentIntoArrays(fileContent);
    const auto verticalSize = tdArray.size();
    const auto horizontalSize = tdArray.front().size();
    const auto alwaysVisible = (verticalSize + horizontalSize) * 2 - 4;
    std::size_t visibleHouses{0};

    std::size_t newVerticalBegin{1};
    const auto newVerticalEnd{tdArray.size() - 1};
    std::size_t newHorizontalBegin{1};
    const auto newHorizontalEnd{tdArray.front().size() - 1};

    for(std::size_t vert{newVerticalBegin}; vert < newVerticalEnd; ++vert){
        for(std::size_t hr{newHorizontalBegin}; hr < newHorizontalEnd; ++hr){
            const auto currentElement = tdArray[vert][hr];
            std::size_t isNotVisible{0};

            for(int goUp(vert - 1); goUp >= 0; --goUp){
                if(currentElement <= tdArray[goUp][hr]){
                    ++isNotVisible;
                    break;
                }
            }
            for(std::size_t goDown{vert + 1}; goDown < verticalSize; ++goDown){
                if(currentElement <= tdArray[goDown][hr]){
                    ++isNotVisible;
                    break;
                }
            }
            for(int goLeft(hr - 1); goLeft >= 0; --goLeft){
                if(currentElement <= tdArray[vert][goLeft]){
                    ++isNotVisible;
                    break;
                }
            }
            for(std::size_t goRight{hr + 1}; goRight < horizontalSize; ++goRight){
                if(currentElement <= tdArray[vert][goRight]){
                    ++isNotVisible;
                    break;
                }
            }
            
            if(isNotVisible < 4){
                ++visibleHouses;
            }
        }
    }

    std::cout << alwaysVisible + visibleHouses;
}

int main(){
    FileReader fr{fileName};
    fr.proceedFile();

    run(fr.getFileContent());
    
    return 0;
}