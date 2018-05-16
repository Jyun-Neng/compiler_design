SRC_DIR = ./src
INC_DIR = ./inc
TEST_DIR = ./tb

INC = -I$(INC_DIR)
CC = gcc

HDR = scanner.h
SRC = ${HDR:.h=.c}
OBJ = ${HDR:.h=.o} Test_scanner.o
TEST = Test_scanner.c
TARGET = Test_scanner_x

all: 
scanner.o:
	$(CC) $(INC) -c $(SRC_DIR)/$(@:.o=.c)	
Test_scanner.o:
	$(CC) $(INC) -c $(TEST_DIR)/$(@:.o=.c)
scanner: $(OBJ) 
	$(CC) $(OBJ) -o $(TARGET); \
			rm *.o

clean:
	rm $(TARGET)
