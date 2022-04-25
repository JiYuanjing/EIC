#!/bin/bash
#four argv: (name of cmnd) (total copy num) (start counting num) (a random num for set seed)
((ifile=$3))
((nfiles=$3+$2))
while [ $ifile -lt $nfiles ]; do
  echo $ifile
  cp -f $1.cmnd $1_$ifile.cmnd
  sed -e "s/seed = 0/seed = "$4$ifile"/g" $1_$ifile.cmnd > temp && mv -f temp $1_$ifile.cmnd
  let "ifile+=1";
done
