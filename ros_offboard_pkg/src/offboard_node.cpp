#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/CommandLong.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <tf/transform_broadcaster.h> 
#include <tf/message_filter.h>
#include <tf/tf.h>

const uint8_t  MODEL_MAN = 1;        // 手动模式
const uint8_t  MODEL_BUILD_MAP = 2;  // 建图模式
const uint8_t  MODEL_NAV = 3;		 // 导航模式
const uint8_t  MODEL_CHARGE = 4;	 // 回充模式
const uint8_t  MODEL_NONE = 0;		 // 默认模式

ros::Time lastTwistReceived;

mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
	current_state = *msg;
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "mavros_offboard_node");
	ros::NodeHandle nh;

	ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);
	// ros::Publisher vision_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/vision_pose/pose", 1);
	ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
	ros::ServiceClient command_client = nh.serviceClient<mavros_msgs::CommandLong>("mavros/cmd/command");
	ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");

	//the setpoint publishing rate MUST be faster than 2Hz
	ros::Rate rate(5.0);

	// wait for FCU connection
	while (ros::ok() && !current_state.connected){
		ros::spinOnce();
		rate.sleep();
	}

	mavros_msgs::SetMode offb_set_mode;
	offb_set_mode.request.custom_mode = "GUIDED";

	mavros_msgs::CommandBool arm_cmd;
	arm_cmd.request.value = true;

	// ros::Time last_request = ros::Time::now();
	// lastTwistReceived = ros::Time::now();

	ROS_ERROR("Setting GUIDED mode... (5 seconds)");
	ros::spinOnce();

    // sleep(1); // 延时 1s，等待 RTAB 启动完成

	// while(ros::ok()) {
	// 	if (current_state.mode != "GUIDED" && (ros::Time::now() - last_request > ros::Duration(5.0))) {
	// 		if (set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent) {
	// 			ROS_ERROR("GUIDED enabled");
	// 			ROS_ERROR("Vehicle arming... (5 seconds)");
	// 		}
	// 		last_request = ros::Time::now();
	// 	} else {
	// 		if (!current_state.armed && (ros::Time::now() - last_request > ros::Duration(5.0))) {
	// 			if (arming_client.call(arm_cmd) &&arm_cmd.response.success){
	// 				ROS_ERROR("**********Vehicle armed");
	// 			}
	// 			last_request = ros::Time::now();
	// 		}
	// 	}
	// 	ros::spinOnce();
	// 	rate.sleep();
	// }
	return EXIT_SUCCESS;
}
