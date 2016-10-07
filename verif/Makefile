ODIR = $(DUMP_DIR)
CCFLAGS = -fPIC -std=c++11

all:
	cd BitVector && $(MAKE) -e CCFLAGS="$(CCFLAGS)"
	cd Pli && $(MAKE) -e CCFLAGS="$(CCFLAGS)"
	cd Signals && $(MAKE) -e CCFLAGS="$(CCFLAGS)"
	cd Test && $(MAKE) -e CCFLAGS="$(CCFLAGS)"
	cd Text && $(MAKE) -e CCFLAGS="$(CCFLAGS)"
	cd Logging && $(MAKE) -e CCFLAGS="$(CCFLAGS)"
	cd Environment && $(MAKE) -e CCFLAGS="$(CCFLAGS)"

cleanall:
	rm -f $(ODIR)/*.o

