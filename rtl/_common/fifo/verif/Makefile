ODIR = $(DUMP_DIR)
CCFLAGS = -fPIC -std=c++11

all:
	cd tests && $(MAKE) -e CCFLAGS="$(CCFLAGS)"
	#cd env && $(MAKE) -e CCFLAGS="$(CCFLAGS)"

cleanall:
	rm -f $(ODIR)/*.o

