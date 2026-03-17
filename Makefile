###
### Makefile
###

CC        ?= gcc
TARGET    ?= keuka
INSTALL   ?= /usr/bin/install -c
prefix    ?= /usr/local
bindir    ?= $(prefix)/bin
binprefix ?=

ARFLAGS   ?= rcs
RM        ?= rm
RMFLAGS   ?= -rf

OPENSSL_INC ?= $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIB ?= $(shell pkg-config --libs openssl 2>/dev/null)

INCLUDE   = include
SOURCES   = src
TOOLS     = tools

CSFILES   = $(wildcard $(SOURCES)/*.c)
OBFILES   = $(patsubst %.c,%.o,$(CSFILES))

KERNEL := $(shell sh -c 'uname -s 2>/dev/null || echo unknown')

CFLAGS  += -I$(INCLUDE) $(OPENSSL_INC)
LDFLAGS += -pthread -lz $(OPENSSL_LIB) -lssl -lcrypto

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
