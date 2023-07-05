# tiny-streamer
<img width=262 src="https://github.com/FelipeIzolan/tiny-streamer/assets/80170121/cdf6500c-c8dc-48f4-ac59-117535af028e"></img>\
create your digital avatar for streams/videos!

## How to use 🔑

- Download tiny-streamer
- Determine assets
  - idle: on /assets/idle
  - speak: on /assets/speak
- Execute tiny-streamer.exe
- Configure OBS
  - Create 'Window Capture'
  - Select 'tiny-streamer.exe'
  - Select the best 'Capture Method'
  - Add chroma-key filter to green color.
  - Edit the scene!

## Assets 👾
The software get assets dynamically on idle & speak folders.\
The frames is limited to 16, but you can change it on main.c:12.\
The names of images is important! name them keeping your desirable frames order.

# Frequency 🎤
Tiny-streamer use frequency to detect when you are speaking and you may need adjust it:\

CTRL + '+' = to more sensitivity.\
CTRL + '-' = to minus sensitivity.


## Development ⌨️
I need test the voice-detector in another microphones (with my microphone it work correctly).\
I will add key-bindings to change sprite-sheet speed and voice-detector frequency.\
Emojis!

## License 📜

- [tiny-streamer](https://github.com/FelipeIzolan/tiny-streamer) - unlicense
- [kit](https://github.com/rxi/kit) - MIT
- [miniaudio](https://github.com/mackron/miniaudio) - Public Domain or MIT
