nightmare: main.cpp res.o
	x86_64-w64-mingw32-g++ main.cpp res.o -lwinmm -lgdi32 -static -o nightmare

res.o: resources.rc
	x86_64-w64-mingw32-windres resources.rc -o res.o
