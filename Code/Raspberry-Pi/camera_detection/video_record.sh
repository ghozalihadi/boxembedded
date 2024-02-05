raspivid -awb fluorescent -br 55 -t 10000 -w 1920 -h 1080 -ex verylow -fps 25 -b 1000000 -o - | ffmpeg -y -r 25.14 -i - -f alsa -ar 44100 -ac 1 -itsoffset 6 -i hw:1,0 -c:v copy -c:a aac -strict experimental -b:a 128k output1.mp4
#ffmpeg -y -i output.avi -c copy output1.mp4
killall -q raspivid ffmpeg
