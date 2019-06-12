#include <cstdlib>
#include "drone_offboard.h"

bool offboardVelocityCtrlNED(Drone& aero);
bool offboardVelocityCtrlBody(Drone& aero);
bool offboardPositionCtrlBody(Drone& aero);
void stepUp(Drone& aero);
void stepDown(Drone& aero);

int main(int argc, char** argv)
{
  ros::init(argc, argv, "offb_velocity_node");
  Drone aero;
  int ret = EXIT_SUCCESS;
  bool is_success;

  if (!aero.arm())
  {
    ROS_ERROR("Fatal: Arming failed!");
    ret = EXIT_FAILURE;
    goto end;
  }

  // Takeoff
  if (!aero.takeoff())
  {
    ROS_ERROR("Fatal: Takeoff failed!");
    ret = EXIT_FAILURE;
    goto end;
  }
  else
  {
    ROS_INFO("Takeoff sent");
  }
  sleep(5);  // Let Aero reach takeoff altitude


  // is_success = offboardPositionCtrlBody(aero);


 // Using local NED co-ordinates
  is_success = offboardVelocityCtrlNED(aero);
  if (is_success)
  {
    ROS_INFO("Offboard velocity control using NED: Done.\n");
  }
  else
  {
    ROS_ERROR("Offboard velocity control using NED: Failed.\n");
  }

    // Using Body co-ordinates
  is_success = offboardVelocityCtrlBody(aero);
  if (is_success)
  {
    ROS_INFO("Offboard velocity control using Body: Done.\n");
  }
  else
  {
    ROS_ERROR("Offboard velocity control using Body: Failed.\n ");
  }

 


  // Land
  if (!aero.land())
  {
    ROS_ERROR("Fatal: Land failed!");
    ret = EXIT_FAILURE;
    goto end;
  }
  sleep(5);  // Let it land

end:
  ros::shutdown();
  return ret;
}

// position body doesnt work???
// bool offboardPositionCtrlBody(Drone& aero)
// {

//   ROS_INFO("Body: Move Forward 3m");
// aero.setOffboardPositionBody(3.0f, 0.0f, 0.0f, 0.0f, 140);

//   ROS_INFO("Body: Turn Left");
// aero.setOffboardPositionBody(0.0f, 0.0f, 0.0f, 90.0f, 100);


//   ROS_INFO("Body: Move Left 3m");
// aero.setOffboardPositionBody(0.0f, 3.0f, 0.0f, 0.0f, 140);



// }

bool offboardVelocityCtrlNED(Drone& aero)
{
  // Send once before starting offboard, otherwise it will be rejected.
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f);

  if (aero.setOffboardMode())
    ROS_INFO("Offboard enabled");
  else
  {
    ROS_ERROR("Unable to switch to Offboard");
    return false;
  }

  // Moves up in step pattern
  stepUp(aero);

  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 20);

  // Moves down in step pattern
  stepDown(aero);

  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 20);

  return true;
}

bool offboardVelocityCtrlBody(Drone& aero)
{
  ROS_INFO("Started Body velocity");

  // Send it once before starting offboard, otherwise it will be rejected.
  aero.setOffboardVelocityBody(0.0f, 0.0f, 0.0f, 0.0f);

  if (aero.setOffboardMode())
    ROS_INFO("Offboard enabled");
  else
  {
    ROS_ERROR("Unable to switch to Offboard");
    return false;
  }

  ROS_INFO("Body: Move Forward");
  aero.setOffboardVelocityBody(2.0f, 0.0f, 0.0f, 0.0f, 100);

  ROS_INFO("Body: Move Right");
  aero.setOffboardVelocityBody(0.0f, 2.0f, 0.0f, 0.0f, 100);

  ROS_INFO("Body: Turn AntiClockwise");
  aero.setOffboardVelocityBody(0.0f, 0.0f, 0.0f, 30.0f, 100);

  ROS_INFO("Body: Move Forwards");
  aero.setOffboardVelocityBody(0.0f, 2.0f, 0.0f, 0.0f, 100);

  ROS_INFO("body: Wait for a bit");
  aero.setOffboardVelocityBody(0.0f, 0.0f, 0.0f, 0.0f, 4);

  ROS_INFO("Body: Fly a circle sideways");
  aero.setOffboardVelocityBody(0.0f, 5.0f, 0.0f, -30.0f, 200);

  ROS_INFO("body: Wait for a bit");
  aero.setOffboardVelocityBody(0.0f, 0.0f, 0.0f, 0.0f, 4);

  return true;
}

void stepUp(Drone& aero)
{

   ROS_INFO("NED: move");
  aero.setOffboardVelocityNED(-1.0f, -1.0f, 0.0f, 0.0f, 80);


 ROS_INFO("NED: Hold position");
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 60);

    // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 40);

  ROS_INFO("NED: Yaw 90 deg");
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 90.0f, 100);


  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 90.0f, 40);


  ROS_INFO("NED: yaw 90");
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 180.0f, 100);

    // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 180.0f, 40);

  ROS_INFO("NED: Yaw 90");
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 270.0f, 100);

    // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 270.0f, 40);

  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 270.0f, 40);

   ROS_INFO("NED: move down");
  aero.setOffboardVelocityNED(0.0f, 0.0f, -1.0f, 0.0f, 10);

    // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 40);

  ROS_INFO("NED: Yaw 90 deg");
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 90.0f, 200);


  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 90.0f, 40);


  ROS_INFO("NED: yaw 90");
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 180.0f, 200);

    // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 180.0f, 40);

  ROS_INFO("NED: Yaw 90");
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 270.0f, 200);

    // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 270.0f, 40);

  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 270.0f, 40);





  // // original code

  // ROS_INFO("NED: Move North");
  // aero.setOffboardVelocityNED(2.0f, 0.0f, 0.0f, 0.0f, 100);

  //   // Wait for a bit
  // aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 40);

  // ROS_INFO("NED: Move West");
  // aero.setOffboardVelocityNED(0.0f, 2.0f, 0.0f, 90.0f, 100);


  // // Wait for a bit
  // aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 90.0f, 40);


  // ROS_INFO("NED: Move South");
  // aero.setOffboardVelocityNED(-2.0f, 0.0f, 0.0f, 180.0f, 100);

  //   // Wait for a bit
  // aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 180.0f, 40);

  // ROS_INFO("NED: Move East");
  // aero.setOffboardVelocityNED(0.0f, -2.0f, 0.0f, 270.0f, 100);

  //   // Wait for a bit
  // aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 270.0f, 40);

  // // Wait for a bit
  // aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 270.0f, 40);

}

void stepDown(Drone& aero)
{
  

  ROS_INFO("NED: Move towards west");
  aero.setOffboardVelocityNED(0.0f, 2.0f, 0.0f, 360.0f, 60);

  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 4);

 

  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 4);

  ROS_INFO("NED: Move towards west");
  aero.setOffboardVelocityNED(0.0f, 2.0f, 0.0f, 360.0f, 60);

  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 4);

 

  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 4);

  // Wait for a bit
  aero.setOffboardVelocityNED(0.0f, 0.0f, 0.0f, 0.0f, 4);


}