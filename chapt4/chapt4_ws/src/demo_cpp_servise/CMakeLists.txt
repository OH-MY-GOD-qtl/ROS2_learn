cmake_minimum_required(VERSION 3.20)
project(demo_cpp_servise)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(chapt4_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(turtlesim_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(turtle_control src/turtle_control.cpp)

target_include_directories(turtle_control PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(turtle_control rclcpp::rclcpp
  geometry_msgs::geometry_msgs__rosidl_generator_cpp
  geometry_msgs::geometry_msgs__rosidl_typesupport_cpp
  turtlesim_msgs::turtlesim_msgs__rosidl_generator_cpp
  turtlesim_msgs::turtlesim_msgs__rosidl_typesupport_cpp
  chapt4_interfaces::chapt4_interfaces__rosidl_generator_cpp
  chapt4_interfaces::chapt4_interfaces__rosidl_typesupport_cpp)

install(TARGETS turtle_control
  DESTINATION lib/${PROJECT_NAME})

ament_package()
