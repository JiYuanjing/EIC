#! /bin/csh
source ~/.cshrc
sphenix
setenv LD_LIBRARY_PATH /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/LHAPDF-6.2.3/lib:$LD_LIBRARY_PATH
setenv LHAPDF_DATA_PATH  /sphenix/user/jiyj/Program/lhapdf_share
# setenv PYTHIA8 /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/pythia8243/share/Pythia8
setenv PYTHIA8 /sphenix/user/jiyj/Program/pythia_xml
setenv PYTHIA8DATA ${PYTHIA8}/xmldoc
echo $PYTHIA8DATA
# lhapdf --version
# lhapdf --pdfdir=/sphenix/user/jiyj/lhapdf 
#  ./genEP cards/EP.cmnd out/test.root $PYTHIA8DATA 
@ x = 1 
while ($x <= 1)
# for i in $(seq 101 200 )
 ./genEP cards/EP.cmnd out/test_${x}.root $PYTHIA8DATA 
 @ x++
end
