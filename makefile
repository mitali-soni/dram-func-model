# AUTHORS:
# Mitali Soni

CC = g++
WARNING_FLAGS = -Wall -Wextra -g
EXE = dramfunc
#SCAN_BUILD_DIR = scan-build-out

all: main.o bank.o buspacket.o
	$(CC) -o $(EXE) main.o bank.o buspacket.o -g

main.o: main.cpp bank.h buspacket.h
	$(CC) $(WARNING_FLAGS) -c main.cpp

bank.o: bank.cpp bank.h
	$(CC) $(WARNING_FLAGS) -c bank.cpp

buspacket.o: buspacket.cpp buspacket.h
	$(CC) $(WARNING_FLAGS) -c buspacket.cpp

clean:
	rm $(EXE) *.o
