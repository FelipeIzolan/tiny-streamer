debug: 
	gcc main.c src/utils.c -Isrc -o tiny-streamer.exe -lgdi32 -luser32 -lwinmm && "./tiny-streamer.exe"
