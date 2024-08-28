CFLAGS = -DUI_WINDOWS -Wall -Wextra -Wno-unused -Wshadow -Wconversion -fsanitize=address,undefined,integer
main: main.o luigi.o
	clang $(CFLAGS) -luser32 -lgdi32 -lshell32 main.o luigi.o -o main.exe

main.o: main.c str.h
	clang -c $(CFLAGS) main.c -o main.o

luigi.o: luigi.c 
	clang -c $(CFLAGS) luigi.c -o luigi.o


test: str.h
	clang -Wall -Wextra -Wconversion -Wshadow -fsanitize=address,undefined -x c str.h -DTEST -o str_test.exe
	str_test.exe

clean:
	rm -rf *.o
