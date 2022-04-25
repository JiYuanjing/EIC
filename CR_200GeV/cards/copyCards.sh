#!/bin/bash
#four argv: (name of cmnd) (total copy num) (start counting num) (a random num for set seed) (ntimes)
((ifile=$3))
((nfiles=$3+$2))
((ntimes=$5))
while [ $ifile -lt $nfiles ]; do
  echo $ifile
  for itimes in $(seq 1 $ntimes)
  do 
    cp -f ${1}.cmnd  ${1}_${ifile}_${itimes}.cmnd
    ((combine=${ifile}*${ntimes}+${itimes}))
    ((combine=${combine}+${4}*10000))
    sed -e "s/seed = 0/seed = "$combine"/g" ${1}_${ifile}_${itimes}.cmnd > temp && mv -f temp ${1}_${ifile}_${itimes}.cmnd
  done
  let "ifile+=1";
done
