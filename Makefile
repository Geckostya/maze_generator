#You can change this values
CC      := g++
EXT     := cpp
FG		:= -std=c++14 -g -Wall -Wshadow -Wextra
PROGNAME:= maze_generator

INCDIR  := include
OBJDIR  := bin
SRCDIR  := src
DEPDIR  := dep


#Don't change this, if you don't know what is it
SOURCES := $(shell find $(SRCDIR) -name '*.$(EXT)')
OBJECTS := $(subst $(SRCDIR), $(OBJDIR), $(SOURCES:.$(EXT)=.o))
DEPS    := $(subst $(SRCDIR), $(DEPDIR), $(SOURCES:.$(EXT)=.d))

.PHONY: dirs all clean val run std

all: dirs $(PROGNAME)

dirs:
	mkdir -p $(OBJDIR) $(DEPDIR)

$(PROGNAME): $(OBJECTS)
	$(CC) $(FG) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.$(EXT)
	$(CC) $(FG) -MMD -I$(INCDIR) $< -c -o $@ 
	mv $(OBJDIR)/*.d $(DEPDIR)/
-include $(DEPS)

std:
	clear; clear; clear; make -B;

run:
	./$(PROGNAME)

val:
	valgrind ./$(PROGNAME)

clean:
	rm -rf $(PROGNAME) $(OBJDIR) $(DEPDIR)

#Makefile by Kostya and Misha ;)