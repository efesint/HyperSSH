CPP = c++
CPP_STANDARD = -std=c++17
CPP_FLAGS ?= -Wall -Wextra
LDLIBS = -lyaml-cpp

TARGET = hyperssh
SOURCE = src/HyperSSH.cpp
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
DESTDIR ?=

.PHONY: all default install uninstall clean

all: $(TARGET)

default: all

$(TARGET): $(SOURCE) Makefile
	$(CPP) $(CPP_STANDARD) $(CPP_FLAGS) $(SOURCE) $(LDLIBS) -o $@

install: $(TARGET)
	install -D -m 755 $(TARGET) "$(DESTDIR)$(BINDIR)/$(TARGET)"

uninstall:
	rm -f "$(DESTDIR)$(BINDIR)/$(TARGET)"

clean:
	rm -f $(TARGET)
