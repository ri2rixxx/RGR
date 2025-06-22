CXX := g++

CXXFLAGS := -Iinclude -fPIC
LIBRARIES := libshamir.so libhill.so libhyuz.so libfilework.so

PREFIX := /usr
BINDIR := $(PREFIX)/bin
LIBDIR := $(PREFIX)/lib/myapp
DATADIR := $(PREFIX)/share/myapp

LDFLAGS := -L. -lshamir -lhill -lhyuz -lfilework -Wl,-rpath=$(LIBDIR)

all: $(LIBRARIES) main


lib%.so: src/%.cpp include/%.hpp
	$(CXX) $(CXXFLAGS) -shared $< -o $@


main: src/main.cpp $(LIBRARIES)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

install: all
	install -d $(DESTDIR)$(BINDIR)
	install -d $(DESTDIR)$(LIBDIR)
	install -d $(DESTDIR)$(DATADIR)
	install -m 755 main $(DESTDIR)$(BINDIR)/myapp
	install -m 644 $(LIBRARIES) $(DESTDIR)$(LIBDIR)/

run: main
	LD_LIBRARY_PATH=. ./main


clean:
	rm -f main $(LIBRARIES)

.PHONY: all run clean install
