#include <iostream>
#include <cmath>
#include <functional>
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtlesim_msgs/msg/pose.hpp"
#include "chapt4_interfaces/srv/patrol.hpp"

using Patrol = chapt4_interfaces::srv::Patrol;

class TurtleController : public rclcpp::Node{
private:
    rclcpp::Service<Patrol>::SharedPtr patrol_server_;
    rclcpp::Subscription<turtlesim_msgs::msg::Pose>::SharedPtr pose_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
    double target_x_{1.0};
    double target_y_{1.0};
    double k_{1.0};
    double max_speed_{3.0};
    void on_pose_received_(const turtlesim_msgs::msg::Pose::SharedPtr pose){
        auto message = geometry_msgs::msg::Twist();
        double current_x = pose -> x;
        double current_y = pose -> y;
        RCLCPP_INFO (this -> get_logger(), "当前位置:(x=%f,y=%f)", current_x, current_y);
        double distance = std::sqrt((target_x_ - current_x) * (target_x_ - current_x) + (target_y_ - current_y) * (target_y_ - current_y));
        double angle = std::atan2(target_y_ - current_y, target_x_ - current_x) - pose -> theta;

        if(distance > 0.1){
            if(std::fabs(angle) > 0.2) message.angular.z = std::fabs(angle);
            else message.linear.x = k_ * distance;
        }
        if(message.linear.x > max_speed_) message.linear.x = max_speed_;
        velocity_publisher_ ->publish(message);
    }

public:
    TurtleController() : Node("turtle_controller"){
        patrol_server_ = this -> create_service<Patrol>("patrol", [&](const std::shared_ptr<Patrol::Request> request, std::shared_ptr<Patrol::Response> response) -> void{
            if((0 < request -> target_x && request -> target_x < 12.0f) && (0 < request -> target_y && request -> target_y < 12.0f)){
                target_x_ = request -> target_x;
                target_y_ = request -> target_y;
                response -> result = Patrol::Response::SUCCESS;
            }else{
                response -> result = Patrol::Response::FALL;
            }
        });
        velocity_publisher_ = this ->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 1);
        pose_subscription_ = this -> create_subscription<turtlesim_msgs::msg::Pose>("/turtle1/pose", 1, std::bind(&TurtleController::on_pose_received_, this, std::placeholders::_1));
    }
};

int main(int argc, char **argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtleController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}