#include <charconv>
#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace string_utils{

std::vector<std::string_view> SplitCommandString(std::string_view sv){
    std::vector<std::string_view> res{};
    for(auto pos = sv.find(' ', 0); pos != std::string::npos; pos = sv.find(' ', 0))
    {
        res.push_back(sv.substr(0, pos));
        sv.remove_prefix(pos + 1);
    }
    res.push_back(sv);
    return res;
}

int GetIntValue(std::string_view str_value){
    int val = 0;
    std::from_chars(str_value.data(), str_value.data() + str_value.size(), val);
    return val;
}

} // namespace string_utils

enum class CommandType{READ, CHEER, ERROR};

struct Command{
    CommandType type;
    int user;
    int pages;
};


Command MakeCommand(std::string_view sv){

    std::vector<std::string_view> command_data = string_utils::SplitCommandString(sv);
    if(command_data[0] == "READ"){
         return Command{CommandType::READ, string_utils::GetIntValue(command_data[1]), string_utils::GetIntValue(command_data[2])};
    }
    if(command_data[0] == "CHEER"){
        return Command{CommandType::CHEER, string_utils::GetIntValue(command_data[1]), 0};
    }

    return Command{CommandType::ERROR, 0, 0};
}

class Ebook{
public:
    Ebook()
    :page_readers_(std::vector<int>(1001, 0))
    {}

    void Read(int user, int page){
        int prev_page = 0;
        if(users_pages_.count(user)){
            prev_page = users_pages_.at(user);
        }
        AddReadPagesCounter(prev_page, page);
        users_pages_[user] = page;
    }

    double Cheer(int user) const{
      int pages_count = 0;
        if(users_pages_.count(user)){
            pages_count = users_pages_.at(user);
        }
        if(pages_count == 0){
            return 0.0;
        }
        int users = users_pages_.size();
        if(users == 1){
            return 1.0;
        } 
        if(pages_count == 1){
            return 0;
        }
        int less = users -  page_readers_[pages_count];
        if(less == 0){
            return 0;
        }
        return static_cast<double>(less) / (users - 1);
    }
private:
    std::unordered_map<int, int> users_pages_;
    std::vector<int> page_readers_;

    void AddReadPagesCounter(int from,int to){
        for(int i = from + 1; i <= to; ++i){
            ++page_readers_[static_cast<size_t>(i)];
        }
    }
};


int main(){

    int q;
    std::string str;
    Ebook ebook{};

    std::cin >> q;
    std::getline(std::cin, str);

    std::cout << std::setprecision(6);

    for(int i = 0; i < q; ++i){
        std::getline(std::cin, str);
        Command cmd = MakeCommand(str);
        if(cmd.type == CommandType::READ){
            ebook.Read(cmd.user, cmd.pages);
        }
        if(cmd.type == CommandType::CHEER){
            std::cout << ebook.Cheer(cmd.user) << std::endl;
        }
    }

}