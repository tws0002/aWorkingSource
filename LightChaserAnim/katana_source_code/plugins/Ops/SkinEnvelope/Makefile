# See README for instructions on how to use this Makefile

# The output objects dir
OBJDIR = ./out
SRCDIR = ./src
INSTALLDIR = $(KATANA_HOME)/plugins/Resources/Examples/Ops

OUTFILENAME = SkinEnvelopeOp.so
OUTFILEPATH = $(OBJDIR)/$(OUTFILENAME)

# Plugin sources and includes
PLUGIN_SRC = $(KATANA_HOME)/plugin_apis/src
SOURCES = $(shell find $(SRCDIR) -name \*.cpp)

# Directly suck in the FnAttribute/FnGeolibOp/FnGeolibUtil/pystring cpp files
SOURCES += $(shell find $(PLUGIN_SRC)/FnAttribute -name \*.cpp)
SOURCES += $(shell find $(PLUGIN_SRC)/FnGeolib/op -name \*.cpp)
SOURCES += $(shell find $(PLUGIN_SRC)/FnGeolib/util -name \*.cpp)
SOURCES += $(shell find $(PLUGIN_SRC)/FnPluginManager -name \*.cpp)
SOURCES += $(shell find $(PLUGIN_SRC)/pystring -name \*.cpp)
SOURCES += $(PLUGIN_SRC)/FnPluginSystem/FnPlugin.cpp

INCLUDES += -I$(KATANA_HOME)/plugin_apis/include
INCLUDES += -I$(OPENEXR_HOME)/include/

LIBPATH += -L$(KATANA_HOME)/bin/Geolib3/internal/CEL

LIBS = -lCEL

CXXFLAGS= -Wall -Wextra -Wshadow -Wconversion -Wcast-qual -fPIC -DPIC

# OpenEXR Custom Namespace
# If you compile against a version of OpenEXR that supports
# custom namespaces, define them here, otherwise comment them out.
CXXFLAGS += -Wno-shadow -Wno-conversion
CXXFLAGS += -DImath=Imath_Foundry
CXXFLAGS += -DIex=Iex_Foundry
CXXFLAGS += -DIlmThread=IlmThread_Foundry
CXXFLAGS += -DImf=Imf_Foundry

# Object files and flags
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Targets:
all: $(OUTFILEPATH)

$(OUTFILEPATH): $(OBJS)
	@echo "  Compiling SkinEnvelope "
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBPATH) $(LIBS) -shared -o $(OUTFILEPATH) -Wl,-soname,$(OUTFILENAME)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "  Cleaning SkinEnvelope"
	@rm -rf $(OBJDIR)

install:
	@echo "  Installing to $(INSTALLDIR)"
	@mkdir -p $(INSTALLDIR)
	@cp $(OUTFILEPATH) $(INSTALLDIR)
