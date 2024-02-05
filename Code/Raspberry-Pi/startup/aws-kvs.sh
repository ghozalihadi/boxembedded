#!/bin/bash
echo "----------------------------------------"
echo "System date and time: $(date '+%d/%m/%Y %H:%M:%S')"
echo "Kernel info: $(uname -rmv)"

export GST_PLUGIN_PATH=/home/pi/amazon-kinesis-video-streams-producer-sdk-cpp/build
export LD_LIBRARY_PATH=/home/pi/amazon-kinesis-video-streams-producer-sdk-cpp/open-source/local/lib
echo $GST_PLUGIN_PATH
echo $LD_LIBRARY_PATH

gst-inspect-1.0 kvssink

export STREAM_NAME="kvs-camera-01"
export AWS_REGION="ap-southeast-1"
echo $STREAM_NAME
echo $AWS_REGION

gst-launch-1.0 -q v4l2src device=/dev/video0 \
! videoconvert \
! video/x-raw,format=I420,width=640,height=480 \
! omxh264enc control-rate=2 target-bitrate=512000 inline-header=FALSE periodicty-idr=45 \
! h264parse \
! video/x-h264,stream-format=avc,alignment=au,profile=baseline \
! kvssink \
    stream-name="$STREAM_NAME" \
    aws-region="$AWS_REGION" \
    storage-size=256 \
    iot-certificate="iot-certificate,endpoint=$(jq --raw-output '.endpointAddress' /home/pi/aws-iot/iot-credential-provider.json),cert-path=/home/pi/certs/certificate.pem,key-path=/home/pi/certs/private.pem.key,ca-path=/home/pi/certs/cacert.pem,role-aliases=kvs-role-alias" &
