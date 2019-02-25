#!/bin/bash

TYPUNC=results/4junc
JOBDIR=JOB15
#GT='0.0y 1.0y 2.0y 3.0y 4.0y 5.0y'
#GT='1.0y'
GT='noEW3'


echo "Submit jobs for uncertainties at YUKAWA = "${GT}
mkdir -p ${TYPUNC}/${GT}

#JESList='AbsoluteStat AbsoluteScale AbsoluteMPFBias Fragmentation SinglePionECAL SinglePionHCAL TimePtEta RelativePtBB RelativePtEC1 RelativePtEC2 RelativeBal RelativeFSR RelativeStatFSR RelativeStatEC PileUpDataMC PileUpPtRef PileUpPtBB PileUpPtEC1 PileUpPtEC2 FlavorQCD'

#JESList='AbsoluteScale AbsoluteMPFBias SinglePionHCAL RelativePtBB RelativePtEC1 RelativeBal RelativeFSR PileUpDataMC PileUpPtEC1 PileUpPtEC2 FlavorQCD' #3j
#JESList='AbsoluteScale AbsoluteMPFBias Fragmentation SinglePionECAL SinglePionHCAL RelativePtBB RelativePtEC1 RelativeBal RelativeFSR PileUpDataMC PileUpPtRef PileUpPtBB PileUpPtEC1 PileUpPtEC2 FlavorQCD' #4j 
#JESList='AbsoluteStat AbsoluteScale AbsoluteMPFBias Fragmentation SinglePionECAL SinglePionHCAL RelativePtBB RelativePtEC1 RelativeBal RelativeFSR PileUpDataMC PileUpPtRef PileUpPtBB PileUpPtEC1 PileUpPtEC2 FlavorQCD' #5j

#JESList='PileUpPtEC2'

for jes in $JESList
do
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py jecuncertainty ${jes} sigmajet -1
    ./jobsub ${TYPUNC}/${GT}/JESDown${jes} ttbarxsec.exe ttbarxsec.cfg

    ./updateconfig.py jecuncertainty ${jes} sigmajet 1
    ./jobsub ${TYPUNC}/${GT}/JESUp${jes} ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg
done
    
