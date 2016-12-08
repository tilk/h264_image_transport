#include "h264_image_transport/h264_subscriber.h"
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

namespace h264_image_transport {

H264Subscriber::H264Subscriber(): convert_ctx(NULL)
{
    av_init_packet(&avpkt);
    avcodec_register_all();
    av_log_set_level(AV_LOG_FATAL);
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        ROS_ERROR("cannot find H.264 codec in ffmpeg");
        throw ros::Exception("H264 codec not found, check that your ffmpeg is built with H264 support"); 
    }
    c = avcodec_alloc_context3(codec);
    if(avcodec_open2(c, codec, 0) < 0) {
        ROS_ERROR("Cannot open codec");
        throw ros::Exception("Cannot open codec");
    }
    picture = av_frame_alloc();
}

H264Subscriber::~H264Subscriber()
{
    avcodec_close(c);
    av_free(c);
    av_frame_free(&picture);
}

void H264Subscriber::subscribeImpl (ros::NodeHandle &nh, const std::string &base_topic, uint32_t queue_size, const Callback &callback, const ros::VoidPtr &tracked_object, const image_transport::TransportHints &transport_hints)
{
    queue_size = 10;
    SimpleSubscriberPlugin::subscribeImpl(nh, base_topic, queue_size, callback, tracked_object, transport_hints);
}

void H264Subscriber::internalCallback(const h264_image_transport::H264Packet::ConstPtr& message, const Callback& user_cb)
{
    avpkt.size = message->data.size();
    avpkt.data = (uint8_t*)&message->data[0];
    int got_picture;
    int len = avcodec_decode_video2(c, picture, &got_picture, &avpkt);
    if (len < 0 || !got_picture) {
        ROS_ERROR("Could not decode H264 frame");
        return;
    }
    convert_ctx = sws_getCachedContext(convert_ctx, picture->width, picture->height, AV_PIX_FMT_YUV420P, picture->width, picture->height, AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
    sensor_msgs::ImagePtr out = boost::make_shared<sensor_msgs::Image>();
    out->data.resize(picture->width * picture->height * 3);
    int stride = 3*picture->width;
    uint8_t *dst = &out->data[0];
    sws_scale(convert_ctx, (const uint8_t* const*)picture->data, picture->linesize, 0, picture->height, &dst, &stride);

    out->width = picture->width;
    out->height = picture->height;
    out->step = 3*picture->width;
    out->encoding = sensor_msgs::image_encodings::BGR8;
    out->header = message->header;
    user_cb(out);
}

};

