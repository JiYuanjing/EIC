#! /bin/bash

echo "PYTHIA8DATA: $PYTHIA8DATA" 
./genEP cards/EP.cmnd test.root $PYTHIA8DATA 
