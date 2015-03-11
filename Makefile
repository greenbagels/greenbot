CC=g++
OBJDIR=objs
SRCDIR=src
INCDIR=$(SRCDIR)/inc
CFLAGS+=-I$(INCDIR)

SRCS=$(wildcard $(SRCDIR)/*.cpp)
PLATSPECIFIC=$(wildcard $(SRCDIR)/core_*.cpp $(SRCDIR)/connect_*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(filter-out $(PLATSPECIFIC), $(SRCS)))

ifeq ($(OS),Windows_NT)
  OBJS+=$(wildcard $(OBJDIR)/*_windows.o)
else
  OBJS+=$(wildcard $(OBJDIR)/*_linux.o)
endif

CFLAGS+=-O2 -Wall
CFLAGS_DEBUG+=-O0 -g3 -Werror -DDEBUG -pedantic
LDFLAGS+=

all: greenbot

greenbot: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(wildcard $(INCDIR)/*.h) Makefile
	echo $(OBJS)
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -rf $(OBJDIR) greenbot

