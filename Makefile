CC=g++
OBJDIR=objs
SRCDIR=src
INCDIR=$(SRCDIR)/inc
CFLAGS+=-I$(INCDIR)

SRCS=$(wildcard $(SRCDIR)/*.cpp)
PLATSPECIFIC=$(wildcard $(SRCDIR)/core_*.cpp) $(wildcard $(SRCDIR)/connect_*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(filter-out $(PLATSPECIFIC), $(SRCS)))
PLATOBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(PLATSPECIFIC))

ifeq ($(OS),Windows_NT)
  OBJS+=$(filter %_windows.o, %_windows.o, $(PLATOBJS))
else
  OBJS+=$(filter %_posix.o, %_posix.o, $(PLATOBJS))
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

