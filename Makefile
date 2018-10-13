# ChooseCompiler g++ for C++
CC = gcc

#Flags
CFLAGS = -g -Wall -O0 -Wextra

#OBJ folder
OBJDIR = obj

#target
TARGET = merger

#all: $(TARGET)

$(TARGET): merger.o functions.o
	$(CC) $(CFLAGS) -o $(TARGET) merger.o functions.o -lm

merger.o: merger.c
	$(CC) $(CFLAGS) -c -o merger.o merger.c
functions.o: functions.c
	$(CC) $(CFLAGS) -c -o functions.o functions.c

clean:
	rm *.o $(TARGET)

remake: clean $(TARGET)