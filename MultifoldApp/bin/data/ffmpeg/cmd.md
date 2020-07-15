ffmpeg -i toto.mov -q:v 1 -vcodec mpeg4 -y toto.avi

extract png files
ffmpeg -i toto.mov output_%05d.png
