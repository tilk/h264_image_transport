#include <pluginlib/class_list_macros.h>
#include <h264_image_transport/h264_subscriber.h>

PLUGINLIB_DECLARE_CLASS(h264_image_transport, H264Subscriber, h264_image_transport::H264Subscriber, image_transport::SubscriberPlugin)

