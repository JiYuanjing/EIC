#!/bin/bash
#four argv: (name of cmnd) (total copy num) (start counting num) (a random num for set seed)
((ifile=$3))
((nfiles=$3+$2))

((rdnb=$4))
while [ $ifile -lt $nfiles ]; do
  # echo "file $ifile nprocess $5"
  cp -f $1.cmnd $1_${ifile}.cmnd
  echo "$1_${ifile}.cmnd"
  ((rdnb=$4+${ifile}*100000))
  sed -e "s/seed = 0/seed = ${rdnb}/g" $1_${ifile}.cmnd > temp && mv -f temp $1_${ifile}.cmnd
  let "ifile+=1";
done
