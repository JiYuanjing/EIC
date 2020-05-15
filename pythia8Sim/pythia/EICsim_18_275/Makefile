PROGRAM  =  genEP
Library = /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core
PYTHIAPATH   = /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/pythia8243
PYTHIA_LIB   = $(PYTHIAPATH)/lib
LHAPDFPATH   = /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/LHAPDF-6.2.3
ROOTLIBS = $(shell root-config --libs)
CXX      =  g++
# CXX      =  gcc
CXXFLAGS =  -m64 -fno-inline -O  -W -Wall -pthread -std=c++11 -Wno-deprecated-declarations 
CPPFLAGS = -I$(PYTHIAPATH)/include -I$(ROOTSYS)/include -I$(LHAPDFPATH)/include
LDFLAGS  = -L$(PYTHIA_LIB) -Wl,-rpath,$(PYTHIA_LIB) -lpythia8 -ldl -L$(LHAPDFPATH)/lib -lLHAPDF  $(ROOTLIBS) 
# LDFLAGS  = -L$(PYTHIA_LIB) -Wl,-rpath,$(PYTHIA_LIB) -lpythia8 -ldl -lLHAPDF  $(ROOTLIBS) 

$(PROGRAM):	$(PROGRAM).cpp Makefile
		$(CXX) $(CXXFLAGS) $(PROGRAM).cpp $(CPPFLAGS) $(LDFLAGS) -o $(PROGRAM) 

