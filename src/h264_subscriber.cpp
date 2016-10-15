#include "h264_image_transport/h264_subscriber.h"
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

namespace h264_image_transport {

void H264Subscriber::internalCallback(const h264_image_transport::H264Packet::ConstPtr& message, const Callback& user_cb)
{
}

};

