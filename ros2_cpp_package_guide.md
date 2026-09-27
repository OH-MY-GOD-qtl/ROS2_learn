# ROS 2 Lyrical C++ 包编写指南（CMakeLists 与 package.xml 修改方法）

> 适用环境：Ubuntu 26.04 + ROS 2 Lyrical（`/opt/ros/lyrical`）。
> 核心原则：**不要用 `ament_target_dependencies()`**，一律使用 `target_link_libraries`。

---

## 一、package.xml 的修改规则

**核心规则**：代码里用到了哪个 ROS 包，就加一行 `<depend>`。

| 代码里出现 | package.xml 加 |
|---|---|
| `find_package(rclcpp)` / `#include "rclcpp/rclcpp.hpp"` | `<depend>rclcpp</depend>` |
| `#include "geometry_msgs/msg/twist.hpp"` | `<depend>geometry_msgs</depend>` |
| `#include "turtlesim_msgs/msg/pose.hpp"` | `<depend>turtlesim_msgs</depend>` |
| `#include "chapt4_interfaces/srv/patrol.hpp"` | `<depend>chapt4_interfaces</depend>` |

```xml
<package format="3">
  <name>包名</name>
  <version>0.0.0</version>
  <description>...</description>
  <maintainer email="...">...</maintainer>
  <license>Apache-2.0</license>

  <buildtool_depend>ament_cmake</buildtool_depend>   <!-- 固定不变 -->

  <depend>rclcpp</depend>               <!-- 按需添加 -->
  <depend>geometry_msgs</depend>
  <depend>turtlesim_msgs</depend>
  <depend>chapt4_interfaces</depend>

  <test_depend>ament_lint_auto</test_depend>      <!-- 固定不变 -->
  <test_depend>ament_lint_common</test_depend>

  <export>
    <build_type>ament_cmake</build_type>
  </export>
</package>
```

---

## 二、CMakeLists.txt 的修改规则

只需四步，对应四个位置：

```cmake
cmake_minimum_required(VERSION 3.20)
project(包名)

# ① find_package：声明用到的依赖（和 package.xml 的 <depend> 一一对应）
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim_msgs REQUIRED)
find_package(chapt4_interfaces REQUIRED)

# ② add_executable：一个源文件一个节点
add_executable(节点名 src/xxx.cpp)

# ③ target_link_libraries：链接依赖 target（关键！）
target_link_libraries(节点名 rclcpp::rclcpp
  geometry_msgs::geometry_msgs__rosidl_generator_cpp
  geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
  turtlesim_msgs::turtlesim_msgs__rosidl_generator_cpp
  turtlesim_msgs::turtlesim_msgs__rosidl_typesupport_cpp
  chapt4_interfaces::chapt4_interfaces__rosidl_generator_cpp
  chapt4_interfaces::chapt4_interfaces__rosidl_typesupport_cpp)

# ④ install：安装可执行文件
install(TARGETS 节点名
  DESTINATION lib/${PROJECT_NAME})

ament_package()
```

---

## 三、依赖包 → CMake target 对应关系（最重要）

| 包类型 | find_package / depend 名 | target_link_libraries 里写 |
|---|---|---|
| 客户端库 | `rclcpp` | `rclcpp::rclcpp` |
| 消息/服务接口包 | `geometry_msgs` | `geometry_msgs::geometry_msgs__rosidl_generator_cpp` + `geometry_msgs::geometry_msgs__rosidl_typesupport_cpp` |
| 消息/服务接口包 | `turtlesim_msgs` | `turtlesim_msgs::turtlesim_msgs__rosidl_generator_cpp` + `turtlesim_msgs::turtlesim_msgs__rosidl_typesupport_cpp` |
| 自定义接口包 | `chapt4_interfaces` | `chapt4_interfaces::chapt4_interfaces__rosidl_generator_cpp` + `chapt4_interfaces::chapt4_interfaces__rosidl_typesupport_cpp` |

**规律**：

- `rclcpp` 直接是 `rclcpp::rclcpp`
- 所有**接口包**（msg/srv）都要写**两个 target**，命名规律：
  - `<包名>::<包名>__rosidl_generator_cpp`
  - `<包名>::<包名>__rosidl_typesupport_cpp`

---

## 四、Lyrical 特有注意事项

1. **别用 `ament_target_dependencies()`**
   - 该指令在 Lyrical 中已被移除，会报 `Unknown CMake command "ament_target_dependencies"`。
   - 一律用 `target_link_libraries`。
2. **turtlesim 已拆分**
   - `turtlesim` 包只负责 `turtlesim_node`。
   - 小海龟的 `Pose` 消息头在 `turtlesim_msgs` 包（`#include "turtlesim_msgs/msg/pose.hpp"`），两个包都要声明。
3. **多个节点**：重复 ②③④ 三块，每个节点一套。
4. 构建前先 `source /opt/ros/lyrical/setup.bash`。

---

## 五、完整流程（新节点五步走）

1. 写 `src/xxx.cpp`（含 `main()`）
2. `package.xml` 加 `<depend>`
3. `CMakeLists.txt` 加 `find_package`
4. `CMakeLists.txt` 加 `add_executable` + `target_link_libraries` + `install`
5. `colcon build`（记得先 source 环境）
