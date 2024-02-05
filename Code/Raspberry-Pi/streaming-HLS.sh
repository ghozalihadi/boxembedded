aws --profile iot-profile kinesisvideo get-data-endpoint --stream-name kvs-camera-01 --api-name GET_HLS_STREAMING_SESSION_URL > ./aws-iot/hls-session-url.json

aws --profile iot-profile kinesis-video-archived-media get-hls-streaming-session-url --endpoint-url $(jq --raw-output '.DataEndpoint' ./aws-iot/hls-session-url.json) \
--stream-name kvs-camera-01 \
--playback-mode LIVE
