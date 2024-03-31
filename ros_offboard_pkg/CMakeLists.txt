cmake_minimum_required(VERSION 3.10.2)
project(ros_offboard_pkg)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions("-Wall -Wextra -Werror")

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  mavros_msgs
  roscpp
  std_msgs
  tf
  message_generation
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  LIBRARIES api_mavlink
 CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
#  DEPENDS system_lib
)

# include_directories(
# # include
#   ${catkin_INCLUDE_DIRS} /home/wilson/catkin_ws/devel/include/  
# )

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(offboard_node src/offboard_node.cpp)
target_link_libraries(offboard_node ${catkin_LIBRARIES})
