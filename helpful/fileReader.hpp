#ifndef FILE_READER_HPP
#define FILE_READER_HPP

class FileReader{
public:
    explicit FileReader(std::string fileName) : fileName_(fileName){
        fileHandler_.open(fileName_);
    }
    void proceedFile(){
        std::string line{""};
        while (getline (fileHandler_, line)) {
            fileContent_.push_back(line);
        }
    }
    void checkFirstAndLastElement() const{
        std::cout   << "First Element: " << fileContent_.front() << "\n"
                    << "Last Element: " << fileContent_.back() << "\n";
    }

    void checkAllElements() const {
        for(const auto& element : fileContent_){
            std::cout << element << "\n";
        }
    }

    std::vector<std::string> getFileContent() const {
        return fileContent_;
    }

    ~FileReader(){
        fileHandler_.close();
    }

private:
    std::string fileName_{""};
    std::ifstream fileHandler_{};
    std::vector<std::string> fileContent_{};
};

#endif // FILE_READER_HPP
