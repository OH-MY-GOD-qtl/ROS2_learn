# ROS 2 精简知识盲区总结（C++）

> 环境：Ubuntu 26.04 + ROS 2 Lyrical Luth
> 注意：Lyrical 中 `turtlesim` 已拆分，`Pose`、`Color` 消息均在 `turtlesim_msgs` 包中。

---

## 1. 核心概念

```cpp
rclcpp::Node                         // 节点：谁在做事
std::string topic_name               // 话题：在哪条通道传，如 "/turtle1/cmd_vel"
rclcpp::Publisher<MsgT>              // 发布者：节点 -> 话题
rclcpp::Subscription<MsgT>           // 订阅者：话题 -> 节点
MsgT                                 // 消息类型：传什么结构
```

示例：

```cpp
auto pub = create_publisher<geometry_msgs::msg::Twist>(
    "/turtle1/cmd_vel", 10);

auto sub = create_subscription<turtlesim_msgs::msg::Pose>(
    "/turtle1/pose", 10,
    [](const turtlesim_msgs::msg::Pose::SharedPtr msg) { /* 回调 */ });
```

---

## 2. 订阅的作用

订阅 = 注册回调，异步接收话题消息。

```cpp
sub_ = create_subscription<MsgT>(
    "topic", qos,
    [this](const MsgT::SharedPtr msg) { /* 处理消息 */ });
```

要点：

- 解耦发布者和订阅者。
- 支持多对多。
- 订阅者必须保存为成员变量，否则析构后失效。
- `rclcpp::spin(node)` 触发回调。

---

## 3. 消息类型 vs 消息名

```text
geometry_msgs/msg/Twist
       包名 / msg / 消息名
```

C++：

```cpp
geometry_msgs::msg::Twist
//          ^^^  ^^^^^
//          类别  消息名
```

- 消息类型：`geometry_msgs::msg::Twist`
- 消息名：`Twist`
- 话题名：`/turtle1/cmd_vel`

---

## 4. 常用命令

```bash
ros2 topic list                   # 看话题名
ros2 topic info /turtle1/pose     # 看类型和发布/订阅数量
ros2 interface show turtlesim_msgs/msg/Pose   # 看字段
ros2 topic echo /turtle1/pose     # 看数据
ros2 topic hz /turtle1/pose       # 看频率
ros2 topic pub ...                # 手动发布
```

---

## 5. 当前话题

| 话题 | C++ 消息类型 |
|---|---|
| `/turtle1/cmd_vel` | `geometry_msgs::msg::Twist` |
| `/turtle1/pose` | `turtlesim_msgs::msg::Pose` |
| `/turtle1/color_sensor` | `turtlesim_msgs::msg::Color` |
| `/rosout` | `rcl_interfaces::msg::Log` |
| `/parameter_events` | `rcl_interfaces::msg::ParameterEvent` |

---

## 6. 常见误区

- 节点 ≠ 话题。
- 发布者 ≠ 话题。
- 话题名相同，消息类型不同，不能通信。
- 订阅者写成局部变量，会失效。
- 消息名不是话题名，也不是变量名。

---

## 7. Ubuntu 26.04 注意

ROS 2 Lyrical Luth 核心可用。
Nav2、MoveIt、Gazebo、ros2_control 第三方生态还在早期适配，建议先用核心，第三方用 Docker 或 Ubuntu 24.04 + Jazzy。
