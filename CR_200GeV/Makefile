PROGRAM  =  genEP
PYTHIAPATH   =$(PYTHIA8)
PYTHIA_LIB   = $(PYTHIAPATH)/lib
# LHAPDFPATH   = /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/LHAPDF-6.2.3
ROOTLIBS = $(shell root-config --libs)
CXX      =  g++
# CXX      =  gcc
CXXFLAGS =  -m64 -fno-inline -O  -W -Wall -pthread -std=c++11  -Wno-deprecated-declarations
# load LHAPDF if LHAPDF is used
LHAPDFFLAGS = $(shell lhapdf-config --cflags --ldflags)
CPPFLAGS = -I$(PYTHIAPATH)/include -I$(ROOTSYS)/include  
LDFLAGS  = -L$(PYTHIA_LIB) -Wl,-rpath,$(PYTHIA_LIB) -lpythia8 -ldl  $(ROOTLIBS) 

$(PROGRAM):	$(PROGRAM).cpp Makefile
		$(CXX) $(CXXFLAGS)  $(PROGRAM).cpp $(CPPFLAGS) $(LDFLAGS)  $(LHAPDFFLAGS) -o $(PROGRAM) 

