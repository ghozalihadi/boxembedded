import subprocess
import time

filename=time.strftime("%Y%m%d%H%M%S")
p = subprocess.Popen("aws s3api put-object --bucket video-recording.45elgx4zs2 --key "+ filename +".mp4 --body output1.mp4", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
p.communicate() 
p.wait()
#p.kill()
