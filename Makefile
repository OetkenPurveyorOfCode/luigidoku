ifeq ($(OS), Windows_NT) 
DEFINES = -DUI_WINDOWS
LINKIES = -luser32 -lgdi32 -lshell32
TARGETNAME = sudoku.exe
else
DEFINES = -DUI_LINUX 
LINKIES = -lX11 -lm
TARGETNAME = sudoku
endif


ifeq ($(RELEASE), 1)
CFLAGS = $(DEFINES) -Wall -Wextra -Wno-unused -Wshadow -Wconversion -O2
else
CFLAGS = $(DEFINES) -Wall -Wextra -Wno-unused -Wshadow -Wconversion -fsanitize=address,undefined,integer
endif

$(TARGETNAME): main.o luigi.o
	clang $(CFLAGS) $(LINKIES) main.o luigi.o -o $(TARGETNAME)

main.o: main.c str.h
	clang -c $(CFLAGS) main.c -o main.o

luigi.o: luigi.c 
	clang -c $(DEFINES) luigi.c -o luigi.o


test: str.h
	clang -Wall -Wextra -Wconversion -Wshadow -fsanitize=address,undefined -x c str.h -DTEST -o str_test.exe
	str_test.exe

clean:
	rm -rf *.o
