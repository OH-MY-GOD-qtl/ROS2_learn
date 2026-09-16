#include <iostream>
#include <functional>

void save_with_free_fun(const std::string &file_name){
    std::cout << "调用了自由函数,保存:" << file_name << std::endl;
}
class FileSave{
    public:
    void save_with_member_fun(const std::string&file_name){
        std::cout << "调用了成员函数,保存:" << file_name << std::endl;
    };
};

int main(){
    FileSave file_save;
    auto save_with_lambda_fun = [](const std::string &file_name) ->void{
        std::cout << "调用了lambda函数,保存:" << file_name << std::endl;
    };
    std::function<void(const std::string &)> save1 = save_with_free_fun;
    std::function<void(const std::string &)> save2 = save_with_lambda_fun;
    std::function<void(const std::string &)> save3 = std::bind(&FileSave::save_with_member_fun, &file_save, std::placeholders::_1);
    save1("example1.txt");
    save2("example2.txt");
    save3("example3.txt"); 
    return 0;
}