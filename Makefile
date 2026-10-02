CC = g++
CFLAGS = -Wall -Wextra
# i hate makefiles 

binaries=matrix

temp: main.cpp
	$(CC) $(CFLAGS) -o matrix main.cpp

.PHONY: clean
clean:
	rm -f matrix
