ODIR = $(DUMP_DIR)

all:
	cd BitVector && $(MAKE) -e CCFLAGS="-fPIC -std=c++11"
	cd Pli && $(MAKE) -e CCFLAGS="-fPIC -std=c++11"
	cd Signals && $(MAKE) -e CCFLAGS="-fPIC -std=c++11"
	cd Test && $(MAKE) -e CCFLAGS="-fPIC -std=c++11"

