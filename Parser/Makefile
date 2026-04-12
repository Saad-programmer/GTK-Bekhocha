CC = gcc
CFLAGS = -Wall -Wextra -g
PARSER = gtk_parser
GTK_CFLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`

all: $(PARSER)

$(PARSER): parser.c
	$(CC) $(CFLAGS) -o $(PARSER) parser.c

clean:
	rm -f $(PARSER) *.c *.o gtk_app

run: $(PARSER)
	./$(PARSER) interface.html myapp

install:
	cp $(PARSER) /usr/local/bin/

.PHONY: all clean run install