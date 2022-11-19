# Compiler flags
CFLAGS = -lX11 -lm -lXfixes -g -Wall

# Build Target
TARGET = XCastr

$(TARGET): $(TARGET).c
	gcc $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	rm $(TARGET) $(objects)
