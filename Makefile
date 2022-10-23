BUILDDIR=bin
TARGETS=$(BUILDDIR)/scan1 $(BUILDDIR)/scan2

.phony: all
all: $(TARGETS)

$(BUILDDIR)/scan1: scan1.cpp
	mkdir -p $(BUILDDIR)
	g++ -fopenmp scan1.cpp -o bin/scan1

$(BUILDDIR)/scan2: scan2.cpp
	mkdir -p $(BUILDDIR)
	g++ -std=c++17 -fopenmp scan2.cpp -o bin/scan2

