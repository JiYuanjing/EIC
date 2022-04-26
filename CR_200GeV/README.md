# Run PYTHIA simulation with root and LHAPDF:

## 1, Install software
install LHAPDF6 https://lhapdf.hepforge.org  
>(used for changing proton PDF, not necessary for the current study. If skipping this step, then remove lhapdf options in your Makefile when running pythia)
>Install instruction: https://lhapdf.hepforge.org/install.html
>Then add path into the local library, see step 2.

install root v6  https://root.cern/install/

 install the latest Pythia8 version: https://pythia.org
>cd /your/pythia/directory
>./configure --with-lhapdf6-lib=$LHAPDF/lib
>make
  
## 2, add Pythia and LHAPDF into libarary path:
 For example for ~/.bashrc:
 >export PYTHIA8="/global/home/users/yuanjing/software/pythia8/pythia8306"
 >export PATH=${PATH}:$PYTHIA8/bin
 >export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$PYTHIA8/lib
 >export PYTHIA8DATA=$PYTHIA8/share/Pythia8/xmldoc

 >LHAPDF="/global/home/users/yuanjing/software/LHAPDF-6.4.0-build"
 >export PATH=$PATH:$LHAPDF/bin
 >export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LHAPDF/lib
 >export LHAPDF_DATA_PATH=$LHAPDF/share/LHAPDF


## 3, Example of running Pythia:
See CR_200GeV.zip

How to run:
make   # compile the code #
./test.sh

Output file: test.root
Pythia setup file: card/EP.cmnd

Test code on ITD cluster:
>/global/home/users/yuanjing/test/CR_200GeV

Or in git:
> https://github.com/JiYuanjing/EIC/tree/master/CR_200GeV


# Run PYTHIA8.3 on the RCF

We can directly utilize EIC software.

Set up EIC environment:
> setenv EIC_LEVEL dev; source /cvmfs/eic.opensciencegrid.org/x8664_sl7/MCEG/releases/etc/eic_cshrc.csh

An easy way is to put the following command in your ~/.cshrc:
> alias eic 'setenv EIC_LEVEL dev; source /cvmfs/eic.opensciencegrid.org/x8664_sl7/MCEG/releases/etc/eic_cshrc.csh'
Then directly type _“eic”_ in the terminal every time you wanna load EIC software.

This will populate the environment with
>PYTHIA8=/cvmfs/eic.opensciencegrid.org/x8664_sl7/MCEG/releases/env/EIC2021a
>PYTHIA8DIR=/cvmfs/eic.opensciencegrid.org/x8664_sl7/MCEG/releases/env/EIC2021a
>PYTHIA8_ROOT_DIR=/cvmfs/eic.opensciencegrid.org/x8664_sl7/MCEG/releases/env/EIC2021a
>PYTHIA8DATA=/cvmfs/eic.opensciencegrid.org/x8664_sl7/MCEG/releases/env/EIC2021a/share/Pythia8/xmldoc
>Current version is 8.303
>(Maybe newer versions will be installed in the future.) 

And of course pythia6 is installed as well,
>PYTHIA6=/cvmfs/eic.opensciencegrid.org/x8664_sl7/MCEG/releases/env/EIC2021a
>and the executable is pythia-eic.

Note that c++ version should be **-std=c++11** in EIC software. Check if PYTHIA and LHAPDF addresses have been set up properly in Makefile before running.
