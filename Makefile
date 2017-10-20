###
### Makefile
###

CC      = gcc
TARGET  = keuka
INSTALL = /usr/bin/install -c

prefix = /usr/local
bindir = $(prefix)/bin
binprefix =

ARFLAGS = rcs
RM      = rm
RMFLAGS = -rf

INCLUDE = include
SOURCES = src
TOOLS   = tools

CSFILES = $(wildcard $(SOURCES)/*.c)
OBFILES = $(patsubst %.c,%.o,$(CSFILES))

KERNEL := $(shell sh -c 'uname -s 2>/dev/null || echo unknown')

CFLAGS  = -I$(INCLUDE) -I/usr/local/opt/openssl/include
LDFLAGS = -pthread -lssl -lcrypto -lz -L/usr/local/opt/openssl/lib

ifeq "$(KERNEL)" "Darwin"
LDFLAGS += -framework CoreFoundation -framework Security
endif

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(CSFILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	@cd $(TOOLS) && ./clean.sh

install:
	@cd $(TOOLS) && ./install.sh

uninstall:
	@cd $(TOOLS) && ./uninstall.sh
