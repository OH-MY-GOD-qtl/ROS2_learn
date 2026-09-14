#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include <string>

class PersonNode : public rclcpp::Node{
private:
    std::string name_;
    int age_;

public:
    PersonNode(const std::string &node_name, const std::string &name, const int age) : Node(node_name){
        this -> name_ = name;
        this -> age_ = age;
    };
    void eat(const std::string &food_name){
        RCLCPP_INFO(this -> get_logger(), "My name is %s, i'm %d years old, and i like to eat %s", name_.c_str(), age_, food_name.c_str());
    };
};
int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PersonNode>("cpp_node", "法外狂徒张三", 18);
    node -> eat("鱼香肉丝");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}