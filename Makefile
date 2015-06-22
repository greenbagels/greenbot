# Main Bot
OBJDIR=objs
SRCDIR=src
INCDIR=$(SRCDIR)/inc
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

# Modules
MODULEDIR=modules
MODULEOBJDIR=objs/$(MODULEDIR)
MODULEINCDIR=$(MODULEDIR)/inc
MODULES=$(wildcard $(MODULEDIR)/*.cpp)
MODULEOBJS=$(patsubst $(MODULEDIR)/%.cpp, $(MODULEOBJDIR)/%.o, $(MODULES))

# Protocols
PROTODIR=protocols
PROTOOBJDIR=objs/$(PROTODIR)
PROTOINCDIR=$(PROTODIR)/inc
PROTOS=$(wildcard $(PROTODIR)/*.cpp)
PROTOOBJS=$(patsubst $(PROTODIR)/%.cpp, $(PROTOOBJDIR)/%.o, $(PROTOS))

# Flags
CXXFLAGS+=-I$(INCDIR) -I$(MODULEINCDIR) -I$(PROTOINCDIR) -O3 -Wall -std=c++11 -pthread
CXXFLAGS_DEBUG+=-O0 -g3 -Werror -Wextra -DLOGGING -pedantic
include libs.mk # generated file

.PHONY: all clean debug

all: greenbot greenbot.conf

debug: CXXFLAGS := $(CXXFLAGS) $(CXXFLAGS_DEBUG)
debug: clean all

greenbot.conf:
	echo "user = testbot""$$RANDOM" > greenbot.conf
	echo "server = irc.rizon.net" >> greenbot.conf
	echo "port = 6667" >> greenbot.conf
	echo "password = PASSWORD" >> greenbot.conf
	echo "channels = #greenbot" >> greenbot.conf

greenbot: $(OBJS) $(MODULEOBJS) $(PROTOOBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(MODULEOBJS): | $(MODULEOBJDIR)
$(MODULEOBJDIR):
	mkdir -p $@

$(PROTOOBJS): | $(PROTOOBJDIR)
$(PROTOOBJDIR):
	mkdir -p $@

$(OBJS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $@

$(MODULEOBJDIR)/%.o: $(MODULEDIR)/%.cpp $(wildcard $(MODULEINCDIR)/*.h) Makefile
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(PROTOOBJDIR)/%.o: $(PROTODIR)/%.cpp $(wildcard $(PROTOINCDIR)/*.h) Makefile
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(wildcard $(INCDIR)/*.h) Makefile
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -rf $(OBJDIR) greenbot

