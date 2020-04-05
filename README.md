
# Multi Screen Warping app


openFrameworks App

#### Required addons:

- ofxWarp
- 

## Instructions use


#### Json files

* `config.json` Config parameters file

- Window information:
    - `width, height` size of the a single video output, if the video is 4k it will be scaled down to the proposed dimensions.
    - `decorated`: frameless or frame window
    - `numScreen`: Number of displays that we want to display on
    `Display output = (1920 x 1080) * numScreen`
- Network
  - `ip`: ip send information to
  - `port`: port to receive the information from.
- Position
  - `location`: where the display will start
- Video
  - `type`: 0 -> HAP video, 1 -> HPV
  - `render`: force 0 -> HD video output, 1 force 4K output

```json
"window":{
    "width" : 1920,
    "height" : 1080,
    "decorated": 1,
    "numScreen": 4
},

"network":{
  "ip": "127.0.0.1",
  "port": 32000
},

"position":{
  "x" : 0,
  "y" : 0
},

"video":{
  "type": 0
}
```

* `video.json`

- Videos
  - `width, height`: dimensions of the input video
  - `name`: file name relative to the data folder
  - `alias`: alias video name
  - `drawx, drawy`: position to draw the video on the app

```json
{
	"videos": [

		{
			"width": 4096,
			"height": 2160,
			"name": "Videos/P1344322_hap.mov",
			"alias": "left",
			"drawx": 0,
			"drawy": 0
		},
  ]
}
```

* `warp.json`

Warp information

### Update Video

To change videos modify the entry `name` under the json fie `video.json`
To add more videos output add more entries under `video.json` and the number of `displyScreens` in the `config.json` file.

### Controls

You can use `Controller` to adjust your warps:

* `g` to hide GUI interface
* `s` to save current Warp settings
* `w` to toggle editing on all warps
* Use mouse or cursor keys to move the currently selected control point
* `TAB` to select the next control point
* `-` or `+` to change brightness
* `r` to reset the warp to its default settings
* `F11` to flip content horizontally
* `F12` to flip content vertically

For Perspective warps only:
* `F9` to rotate content counter-clockwise
* `F10` to rotate content clockwise

For Bilinear warps only:
* `m` to toggle between linear and curved mapping
* `F1` to reduce the number of horizontal control points
* `F2` to increase the number of horizontal control points
* `F3` to reduce the number of vertical control points
* `F4` to increase the number of vertical control points
* `F5` to decrease the mesh resolution
* `F6` to increase the mesh resolution
* `F7` to toggle adaptive mesh resolution

## HPV Codec


### HPV Codec

https://github.com/HasseltVR/Holo_Toolset/releases

Console HPV compression from console

https://github.com/HasseltVR/Holo_Toolset/tree/master/HPV_Creator_Console


## Hap codec

HAP is a family of video codecs which perform decompression using a computer's graphics hardware, substantially reducing the CPU usage necessary to play video — this is useful in situations where CPU power is a limiting factor, such as when working with multiple high resolution videos in real-time.

- `Hap` has the lowest data-rate and reasonable image quality.
- `Hap Alpha` has similar image quality to Hap, and supports an Alpha channel.
- `Hap Q` has improved image quality, at the expense of larger file sizes.
- `Hap Q Alpha` has improved image quality and includes an Alpha channel.

Github:
https://github.com/Vidvox/hap

### Hap Converter

https://vdmx.vidvox.net/tutorials/using-the-hap-video-codec

Example settings:


##### Code

Download ffmpeg


````
ffmpeg -i yourSourceFile.mov -c:v hap outputName.mov
````

#### Notes

##### OSX

-  .dylib from the ofxHap addon copy to ffmepg/lib/osx to /usr/local/ib
