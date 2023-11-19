New-Item -ItemType Directory -Force -Path ./build
gcc main.c -Ilibs -o build/tiny-streamer.exe -lgdi32 -luser32 -lwinmm -Os -s -mwindows

Copy-Item ./icon.ico ./build/icon.ico
Copy-Item ./assets ./build -Force -Recurse
