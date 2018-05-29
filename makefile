SRC_DIR = ./src
INC_DIR = ./inc
TEST_DIR = ./tb

INC = -I$(INC_DIR)
CC = gcc

GLB = global.h
HDR = scanner.h parser.h cgen.h
SRC = ${HDR:.h=.c}
OBJ = ${HDR:.h=.o} main.o
TARGET = main_x

all: main
cgen.o:
	$(CC) $(INC) -c $(SRC_DIR)/$(@:.o=.c)	
parser.o:
	$(CC) $(INC) -c $(SRC_DIR)/$(@:.o=.c)	
scanner.o:
	$(CC) $(INC) -c $(SRC_DIR)/$(@:.o=.c)	
main.o:
	$(CC) $(INC) -c $(SRC_DIR)/$(@:.o=.c)
main: $(OBJ) 
	$(CC) $(OBJ) -o $(TARGET); \
			rm *.o

clean:
	rm $(TARGET)
