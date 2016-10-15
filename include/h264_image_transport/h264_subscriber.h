#ifndef H264_IMAGE_TRANSPORT_SUBSCRIBER_H
#define H264_IMAGE_TRANSPORT_SUBSCRIBER_H

#include <image_transport/simple_subscriber_plugin.h>
#include "h264_image_transport/H264Packet.h"

namespace h264_image_transport {

class H264Subscriber : public image_transport::SimpleSubscriberPlugin<h264_image_transport::H264Packet>
{
public:
    virtual std::string getTransportName() const
    {
        return "h264";
    }

protected:
    virtual void internalCallback(const h264_image_transport::H264Packet::ConstPtr& message, const Callback& user_cb);
};

};

#endif
