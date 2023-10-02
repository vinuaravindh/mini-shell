CC = g++
CFLAGS = -Wall
SRC = shell379.cpp
TARGET = shell379

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)