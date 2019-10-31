TARGET = bin/ArenaQuickClose.dll
LIBS = -lminhook
CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -s -shared -static -O3 -Wl,--exclude-all-symbols -Iinclude -Llib

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = src/ArenaQuickClose.cc

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(CXXFLAGS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
