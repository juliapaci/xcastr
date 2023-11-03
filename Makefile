CFLAGS = -lX11 -lXext -lXtst -g -Wall
TARGET = XCastr
BIN = /usr/bin

$(TARGET): $(TARGET).c WindowFunctions.c config.h Makefile
	gcc $(CFLAGS) $(TARGET).c WindowFunctions.c -o $(TARGET)

clean:
	rm -f $(TARGET) $(objects)

install: $(TARGET)
	mkdir -p $(BIN)
	cp -f $(TARGET) $(BIN)
	chmod 775 $(BIN)/$(TARGET)
	# TODO: man page?

uninstall:
	rm -f $(BIN)/$(TARGET)

.PHONY: uninstall install clean
