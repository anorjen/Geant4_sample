name := ExampleG4
G4TARGET := $(name)
G4EXLIB := true
G4DEBUG=1
CPPFLAGS+=-Wno-overloaded-virtual -Wno-unused

.PHONY: all 
all: lib bin

CPPFLAGS+=-Wno-overloaded-virtual -Wno-unused

include $(G4INSTALL)/config/binmake.gmk

