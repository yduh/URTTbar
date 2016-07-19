#!/bin/bash
DIR=JOBS10_parton
VER=V10
COMMAND2="../addjobs.py"
COMMAND="../resubmit.py R"
#COMMAND="../resubmitallmissing.py"
#cd ${DIR}_central_${VER}
#../addjobs.py
#cd ..
cd ${DIR}_central_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_fsdown_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_fsup_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_jetm1sig_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_jetp1sig_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_jetsmear_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_metm1sig_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_metp1sig_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_rsdown_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_rsup_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_topdown_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_topup_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_ttptdown_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_ttptup_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_btagdown_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_btagup_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_ltagdown_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_ltagup_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_pileupup_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
cd ${DIR}_pileupdown_${VER}
pwd
$COMMAND
$COMMAND2
cd ..
