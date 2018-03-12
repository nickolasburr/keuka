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

CFLAGS  = -I/usr/local/opt/openssl/include -I$(INCLUDE)
LDFLAGS = -L/usr/local/opt/openssl/lib -pthread -lssl -lcrypto -lz

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
