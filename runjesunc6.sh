#!/bin/bash

TYPUNC=results/6junc
JOBDIR=JOB15
#GT='0.0y 1.0y 2.0y 3.0y 4.0y 5.0y N1.0y N2.0y N3.0y N4.0y N5.0y'
#GT='0.0y 1.0y 2.0y 3.0y 4.0y 5.0y'
#GT='1.0y'
GT='noEW'


echo "Submit jobs for uncertainties at YUKAWA = "${GT}
mkdir -p ${TYPUNC}/${GT}

#jes1
cp ttbarxsec.cfg ttbarxsec.tmp
./updateconfig.py jecuncertainty SubTotalPileUp sigmajet -1
./jobsub ${TYPUNC}/${GT}/JES1Down ttbarxsec.exe ttbarxsec.cfg

./updateconfig.py jecuncertainty SubTotalPileUp sigmajet 1
./jobsub ${TYPUNC}/${GT}/JES1Up ttbarxsec.exe ttbarxsec.cfg
mv ttbarxsec.tmp ttbarxsec.cfg
    
#jes2
cp ttbarxsec.cfg ttbarxsec.tmp
./updateconfig.py jecuncertainty SubTotalRelative sigmajet -1
./jobsub ${TYPUNC}/${GT}/JES2Down ttbarxsec.exe ttbarxsec.cfg

./updateconfig.py jecuncertainty SubTotalRelative sigmajet 1
./jobsub ${TYPUNC}/${GT}/JES2Up ttbarxsec.exe ttbarxsec.cfg
mv ttbarxsec.tmp ttbarxsec.cfg

#jes3
cp ttbarxsec.cfg ttbarxsec.tmp
./updateconfig.py jecuncertainty SubTotalPt sigmajet -1
./jobsub ${TYPUNC}/${GT}/JES3Down ttbarxsec.exe ttbarxsec.cfg

./updateconfig.py jecuncertainty SubTotalPt sigmajet 1
./jobsub ${TYPUNC}/${GT}/JES3Up ttbarxsec.exe ttbarxsec.cfg
mv ttbarxsec.tmp ttbarxsec.cfg

#jes4
cp ttbarxsec.cfg ttbarxsec.tmp
./updateconfig.py jecuncertainty SubTotalScale sigmajet -1
./jobsub ${TYPUNC}/${GT}/JES4Down ttbarxsec.exe ttbarxsec.cfg

./updateconfig.py jecuncertainty SubTotalScale sigmajet 1
./jobsub ${TYPUNC}/${GT}/JES4Up ttbarxsec.exe ttbarxsec.cfg
mv ttbarxsec.tmp ttbarxsec.cfg

#jes5
cp ttbarxsec.cfg ttbarxsec.tmp
./updateconfig.py jecuncertainty SubTotalAbsolute sigmajet -1
./jobsub ${TYPUNC}/${GT}/JES5Down ttbarxsec.exe ttbarxsec.cfg

./updateconfig.py jecuncertainty SubTotalAbsolute sigmajet 1
./jobsub ${TYPUNC}/${GT}/JES5Up ttbarxsec.exe ttbarxsec.cfg
mv ttbarxsec.tmp ttbarxsec.cfg

#jes6
cp ttbarxsec.cfg ttbarxsec.tmp
./updateconfig.py jecuncertainty SubTotalMC sigmajet -1
./jobsub ${TYPUNC}/${GT}/JES6Down ttbarxsec.exe ttbarxsec.cfg

./updateconfig.py jecuncertainty SubTotalMC sigmajet 1
./jobsub ${TYPUNC}/${GT}/JES6Up ttbarxsec.exe ttbarxsec.cfg
mv ttbarxsec.tmp ttbarxsec.cfg

