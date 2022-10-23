BUILDDIR=bin
TARGETS=$(BUILDDIR)/scan1

.phony: all
all: $(TARGETS)

$(BUILDDIR)/scan1: scan1.cpp
	mkdir -p $(BUILDDIR)
	g++ -fopenmp scan1.cpp -o bin/scan1

