#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "tracker-arb/TrackerARB.h"

namespace cv

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter {
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;


public:
  ImageConverter()
      : it_(nh_) {
    // Subscribe to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/iris/usb_cam/image_raw", 1,
                               &ImageConverter::imageCb, this);
    
    cv::namedWindow(OPENCV_WINDOW);
  }
  
  ~ImageConverter() {
    cv::destroyWindow(OPENCV_WINDOW);
  }
  
  void imageCb(const sensor_msgs::ImageConstPtr &msg) {
    cv_bridge::CvImagePtr cv_ptr;
    try {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
    }
    catch (cv_bridge::Exception &e) {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);
    
    // Get Pose Estimate
  const float markerLength = 2.59;
  const float markerSeparation = 1.90;
  const int markersX = 6;
  const int markersY = 8;
    Vec3d tVec, rVec, ctVec;
    CVCalibration cvl("CalibParams.txt");
   TrackerARB tracker(cvl, markerLength, markerSeparation, markersX, markersY, false);
    
    if (tracker.getPose(cv_ptr->image, tVec, rVec) {
      tracker.correctedPose(rVec, tVec, ctVec);
    };
  }
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}