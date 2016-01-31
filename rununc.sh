#!/bin/bash
TYP=JOBS10_parton
VER=V3

cp ttbarxsec.cfg ttbarxsec.tmp
./updateconfig.py sigmajet -1
./jobsub ${TYP}_jetm1sig_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py sigmajet 1
./jobsub ${TYP}_jetp1sig_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py jetres 0.1
./jobsub ${TYP}_jetsmear_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py sigmamet -1
./jobsub ${TYP}_metm1sig_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py sigmamet 1
./jobsub ${TYP}_metp1sig_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py topptweight -1
./jobsub ${TYP}_topdown_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py topptweight 1
./jobsub ${TYP}_topup_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py ttptweight -1
./jobsub ${TYP}_ttptdown_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py ttptweight 1
./jobsub ${TYP}_ttptup_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py renscale -1
./jobsub ${TYP}_rsdown_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py renscale 1
./jobsub ${TYP}_rsup_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py facscale -1
./jobsub ${TYP}_fsdown_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py facscale 1
./jobsub ${TYP}_fsup_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py btagunc -1
./jobsub ${TYP}_btagdown_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py btagunc 1
./jobsub ${TYP}_btagup_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py ltagunc -1
./jobsub ${TYP}_ltagdown_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py ltagunc 1
./jobsub ${TYP}_ltagup_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py pileupunc -1
./jobsub ${TYP}_pileupdown_${VER} ttbarxsec.exe ttbarxsec.cfg
./updateconfig.py pileupunc 1
./jobsub ${TYP}_pileupup_${VER} ttbarxsec.exe ttbarxsec.cfg
mv ttbarxsec.tmp ttbarxsec.cfg
