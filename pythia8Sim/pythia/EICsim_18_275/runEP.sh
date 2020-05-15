#!/bin/bash
date
ipro=0
cd cards
let "rdsd = $(date +%s)%80000"
echo $rdsd
ntimes=10
sh copyCards.sh EP $1 $2 ${rdsd} $ntimes
cd ..

cp run.job run_ep.job
ijob=$2
((Njobs=$1+$2))
while [ $ijob -lt $Njobs ]; do
 # echo $ijob
 chmod a+x run.sh
 echo "Executable       = run.sh">>run_ep.job
 echo "Arguments        = cards/EP_${ijob} out/EP_${ijob} $ntimes">>run_ep.job
 echo "Output           = log/EP_${ijob}.out">>run_ep.job
 echo "Error            = log/EP_${ijob}.err">>run_ep.job
 echo "Log              = log/EP_${ijob}">>run_ep.job
 echo  "Queue " >>run_ep.job
 echo  "     " >>run_ep.job
 let "ijob+=1";
done

condor_submit run_ep.job

