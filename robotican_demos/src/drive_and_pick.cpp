


#include <ros/ros.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>



int main(int argc, char **argv) {

    ros::init(argc, argv, "demo_pick_node");
    ros::NodeHandle n;

    ros::ServiceClient drive_client = n.serviceClient<std_srvs::Trigger>("drive2object_go");
    ros::ServiceClient pick_client = n.serviceClient<std_srvs::Trigger>("pick_go");

     ROS_INFO("Waiting for services...");
    drive_client.waitForExistence();
    pick_client.waitForExistence();
    ros::Duration(5).sleep();
     ROS_INFO("Ready!");
    std_srvs::Trigger drive_srv;
    if (drive_client.call(drive_srv))
    {
        ROS_INFO("drive2object response: %s", drive_srv.response.message.c_str());
        if (drive_srv.response.success) {
            ros::ServiceClient uc_client = n.serviceClient<std_srvs::SetBool>("update_collision_objects");
            std_srvs::SetBool srv;
            srv.request.data=true;
            uc_client.call(srv);

            ros::Duration(5).sleep();
            std_srvs::Trigger pick_srv;
            if (pick_client.call(pick_srv)) {
                ROS_INFO("pick response: %s", pick_srv.response.message.c_str());
                if (pick_srv.response.success)  ROS_INFO("Done!");
            }
            else ROS_ERROR("Failed to call pick service");

        }
    }
    else ROS_ERROR("Failed to call drive2object service");

ros::shutdown();
    return 0;
}

