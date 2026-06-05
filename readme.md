# Command Line based Video Player

## What it does

* Runs any video and plays it in ASCII style

## Requirements

* OS - Windows
* ffmpeg
* C++23

## How to build

`g++ -std=c++23 -Wall -Wextra -I C:/msys64/ucrt64/include main.cpp -o videoplayer -LC:/msys64/ucrt64/lib -lavcodec -lavformat -lavutil -lswscale -lwinmm`

## How to use

1. Place your video file in the project folder
2. Run the executable:
   ./videoplayer
3. Type the filename when prompted:
   Enter filename: myvideo.mp4

## Known limitations

- Best viewed with font size 6-8pt in terminal
- Recommended terminal size: minimum 120x30
- No audio playback for MP4 files
- Windows only (Linux/Mac planned)

## Future features

1. Audio 
2. Automatic frame resize
3. Interface
4. Keyboard hotkeys
5. Colors