#ep collisions with pythia 8
Code Author: Yuanjing Ji
## how to run the file
The default environment is sPHENIX, but it can be submitted to the STAR node at RCF after set up the environment in the shell.

## 1, how to run and submit jobs
```bash
./runEP.sh (num of jobs) (start num of cards) 
```
or can set it in submit.sh

## 2, before run
gen.cpp is the main function, to make a execuable file, do:
```bash
make
```
to test the run runBunch.sh

