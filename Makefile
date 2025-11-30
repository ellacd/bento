# My somewhat-universal makefile for libraries.
# LDFLAGS must of course be tailored to the individual project.
NAME := bento

LIBNAME := lib$(NAME).so

CC := clang
CFLAGS := -fPIC -std=c99 -W -Wall -Wpedantic -Wno-comment -Wcomma -Wdeclaration-after-statement -Wdocumentation -Wdocumentation-pedantic -Wdouble-promotion -Wempty-translation-unit -Wflexible-array-extensions -Wfloat-conversion -Wfloat-equal -Wfor-loop-analysis -Wformat-non-iso -Wformat-pedantic -Wgcc-compat -Wimplicit -Winfinite-recursion -Wnewline-eof -Wpadded -Wpoison-system-directories -Wstring-concatenation -Wswitch-enum -Wunaligned-access -Wuninitialized -Wunreachable-code-aggressive -Wunused-label -Wvariadic-macros -Wvla
CPPFLAGS = -MMD -MP -I $(INCLUDE_DIR)
LDFLAGS = -shared -lm -lnori -L $(LIBRARY_DIR)

SANFLAGS := -fsanitize=address,undefined
MAKEFLAGS := --no-print-directory

SRC_DIR := src
BUILD_DIR := .build
INCLUDE_DIR := include
LIBRARY_DIR := lib

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJ:%.o=%.d)

all: debug

$(LIBNAME): $(OBJ)
	$(CC) $(LDFLAGS) -o $(LIBNAME) $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP)

fclean: clean
	rm -f $(LIBNAME) $(NAME)

debug: CFLAGS += -g3 -Og $(SANFLAGS)
debug: LDFLAGS += $(SANFLAGS)
debug: $(OBJ)
	$(CC) -o $(NAME) $^ $(LDFLAGS) 

release: CFLAGS += -O2 -DNDEBUG
release: fclean $(LIBNAME)

install: $(LIBNAME)
	cp $(LIBNAME) /usr/lib

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: clean fclean re
.SILENT:
