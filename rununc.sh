#!/bin/bash
TYP=results/5j
GT='1.0y 2.0y 3.0y 4.0y 5.0y'
UNCERT=''
OPT=''
NAME='mtdown mtup'
VER=V0

#for ((i=0;i<=${#im1_files[@]};i++)); do
#       run_black.sh "${im1_files[i]}" "${im2_files[i]}"
#   done

cp ttbarxsec.cfg ttbarxsec.tmp
for gt in $GT
do
    ./updateconfig.py yukawatxt yukawa_reweighing1.0_169.5.root 
    ./jobsub ${TYP}/${gt}/mtdown ttbarxsec.exe ttbarxsec.cfg

    ./updateconfig.py yukawatxt yukawa_reweighing1.0_175.5.root 
    ./jobsub ${TYP}/${gt}/mtup ttbarxsec.exe ttbarxsec.cfg

    ./updateconfig.py sigmajet -1
    ./jobsub ${TYP}/${gt}/jetm1sig ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py sigmajet 1
    ./jobsub ${TYP}/${gt}/jetp1sig ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py jetres 0.1
    ./jobsub ${TYP}/${gt}/jetsmear ttbarxsec.exe ttbarxsec.cfg

#./updateconfig.py sigmamet -1
#./jobsub ${TYP}_metm1sig_${VER} ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py sigmamet 1
#./jobsub ${TYP}_metp1sig_${VER} ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py topptweight -1
#./jobsub ${TYP}/topdown ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py topptweight 1
#./jobsub ${TYP}/topup ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py ttptweight -1
#./jobsub ${TYP}_ttptdown_${VER} ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py ttptweight 1
#./jobsub ${TYP}_ttptup_${VER} ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py renscale -1
#./jobsub ${TYP}_rsdown_${VER} ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py renscale 1
#./jobsub ${TYP}_rsup_${VER} ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py facscale -1
#./jobsub ${TYP}_fsdown_${VER} ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py facscale 1
#./jobsub ${TYP}_fsup_${VER} ttbarxsec.exe ttbarxsec.cfg

#./updateconfig.py btagunc -1
#./jobsub ${TYP}/${GT}/btagdown ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py btagunc 1
#./jobsub ${TYP}/${GT}/btagup ttbarxsec.exe ttbarxsec.cfg

#./updateconfig.py ltagunc -1
#./jobsub ${TYP}/ltagdown ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py ltagunc 1
#./jobsub ${TYP}/ltagup ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py pileupunc -1
#./jobsub ${TYP}_pileupdown_${VER} ttbarxsec.exe ttbarxsec.cfg
#./updateconfig.py pileupunc 1
#./jobsub ${TYP}_pileupup_${VER} ttbarxsec.exe ttbarxsec.cfg


mv ttbarxsec.tmp ttbarxsec.cfg
'


