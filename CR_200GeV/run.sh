#! /bin/csh
source ~/.cshrc
sphenix
setenv LD_LIBRARY_PATH /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/LHAPDF-6.2.3/lib:$LD_LIBRARY_PATH
# setenv LHAPDF_DATA_PATH  /sphenix/user/jiyj/lhapdf
setenv LHAPDF_DATA_PATH  /sphenix/user/jiyj/Program/lhapdf_share
# setenv PYTHIA8 /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/pythia8243/share/Pythia8
setenv PYTHIA8 /sphenix/user/jiyj/PYTHIA/pythia8244 
setenv PYTHIA8DATA ${PYTHIA8}/share/Pythia8/xmldoc

# setenv PYTHIA8  /sphenix/user/jiyj/Program/pythia_xml
# setenv PYTHIA8DATA ${PYTHIA8}/xmldoc
# lhapdf --version
# lhapdf --pdfdir=/sphenix/user/jiyj/lhapdf 

 ./genEP ${1}_1.cmnd $2_1.root $PYTHIA8DATA 

# @ x = 1 
# @ ntimes = 1
# while ($x <= $ntimes)
# #  ./genEP cards/EP.cmnd out/test_${x}.root $PYTHIA8DATA 
#  ./genEP ${1}_${x}.cmnd $2_${x}.root $PYTHIA8DATA 
#  @ x++
# end
