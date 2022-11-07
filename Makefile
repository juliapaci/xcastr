# Compiler flags
CFLAGS = -lX11 -g -Wall

# Build Target
TARGET = XCastr

$(TARGET): $(TARGET).c
	gcc $(CFLAGS) $(TARGET).c -o $(TARGET)
