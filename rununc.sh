#!/bin/bash

TYP=results/3j
TYPUNC=results/3junc
JOBDIR=JOB13
#GT='0.0y 1.0y 2.0y 3.0y 4.0y 5.0y N1.0y N2.0y N3.0y N4.0y N5.0y'
#GT='0.0y 2.0y 3.0y 4.0y 5.0y'
GT='1.0y'

RUN=false
RUNOUNC=true
RUNMASS=false


if $RUN; then
    for gt in $GT
    do
        echo "Submit jobs for YUKAWA = ${gt} ..."
        mkdir -p ${TYP}
        cp ttbarxsec.cfg ttbarxsec.tmp
   
        #rm inputs/$JOBDIR/*txt
        #rm inputs/$JOBDIR/INPUT/yukawa_reweighting*.root

        if [ "${gt}" = "1.0y" ]; then
            #cp inputs/$JOBDIR/backup/*txt inputs/$JOBDIR
            ##cp inputs/$JOBDIR/backup_theoreticaluncert/*txt inputs/$JOBDIR
            #cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting${gt}.root inputs/$JOBDIR/INPUT
            ./updateconfig.py yukawatxt yukawa_reweighting${gt}.root
            ./jobsub ${TYP}/${gt} ttbarxsec.exe ttbarxsec.cfg
        else
            #cp inputs/$JOBDIR/backup/tt_PowhegP8.txt inputs/$JOBDIR
            #cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting${gt}.root inputs/$JOBDIR/INPUT 
            ./updateconfig.py yukawatxt yukawa_reweighting${gt}.root
            ./jobsub ${TYP}/${gt} ttbarxsec.exe ttbarxsec.cfg
        fi
        
        mv ttbarxsec.tmp ttbarxsec.cfg
    done
fi



if $RUNMASS; then
    echo "submit jobs for YUKAWA = 1.0y ..."
    cp ttbarxsec.cfg ttbarxsec.tmp

    #rm inputs/$JOBDIR/*txt
    #cp inputs/$JOBDIR/backup_theoreticaluncert/mtop/tt_mtop1695_PowhegP8.txt inputs/$JOBDIR
    #cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting${gt}_169.5.root inputs/$JOBDIR/INPUT
    #./updateconfig.py yukawatxt yukawa_reweighting1.0y_169.5.root 
    #./jobsub ${TYPUNC}/1.0y/mtDown ttbarxsec.exe ttbarxsec.cfg

    #rm inputs/$JOBDIR/*txt
    #cp inputs/$JOBDIR/backup_theoreticaluncert/mtop/tt_mtop1755_PowhegP8.txt inputs/$JOBDIR
    #cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting${gt}_175.5.root inputs/$JOBDIR/INPUT
    #./updateconfig.py yukawatxt yukawa_reweighting1.0y_175.5.root 
    #./jobsub ${TYPUNC}/1.0y/mtUp ttbarxsec.exe ttbarxsec.cfg

    mv ttbarxsec.tmp ttbarxsec.cfg
fi


if $RUNOUNC; then
    echo "Submit jobs for other uncertainties YUKAWA = 1.0y ..."
    mkdir -p ${TYPUNC}/1.0y

    #rm inputs/$JOBDIR/INPUT/yukawa_reweighting*.root
    #cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting1.0y.root inputs/$JOBDIR/INPUT

    #PDF uncertainty is estimated with all MCs
    #============================================================#
    #rm inputs/$JOBDIR/*txt
    #cp inputs/$JOBDIR/backup/*txt inputs/$JOBDIR
    #PDF: total < 1 %
    #cp ttbarxsec.cfg ttbarxsec.tmp
    #./updateconfig.py PDFTEST 1
    #./jobsub ${TYPUNC}/1.0y/pdf ttbarxsec.exe ttbarxsec.cfg
    #mv ttbarxsec.tmp ttbarxsec.cfg

    
    #lepton uncertainties are estimated with all MC except some sparated theoretical signal ones 
    #============================================================#
    #rm inputs/$JOBDIR/*txt
    #cp inputs/$JOBDIR/backupsmall/*txt inputs/$JOBDIR
    #leptons: varies up/down scale factor gotten by tag and probe method; total ~3 %
    #./updateconfig.py ELECTRONS 0
    #./jobsub ${TYPUNC}/1.0y/el ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py MUONS 0
    #./jobsub ${TYPUNC}/1.0y/mu ttbarxsec.exe ttbarxsec.cfg


    #uncertainties are estimated only with signal MC
    #============================================================#
    #rm inputs/$JOBDIR/*txt
    #cp inputs/$JOBDIR/backup/tt_PowhegP8.txt inputs/$JOBDIR

    #NLO:
    #factorization and renormalization: total < 1 %
    #cp ttbarxsec.cfg ttbarxsec.tmp
    #./updateconfig.py hdamp -1
    #./jobsub ${TYPUNC}/1.0y/hdampDown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py hdamp 1
    #./jobsub ${TYPUNC}/1.0y/hdampUp ttbarxsec.exe ttbarxsec.cfg
    #mv ttbarxsec.tmp ttbarxsec.cfg

    #cp ttbarxsec.cfg ttbarxsec.tmp
    #./updateconfig.py facscale -1
    #./jobsub ${TYPUNC}/1.0y/fsDown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py facscale 1
    #./jobsub ${TYPUNC}/1.0y/fsUp ttbarxsec.exe ttbarxsec.cfg
    #mv ttbarxsec.tmp ttbarxsec.cfg
    
    #cp ttbarxsec.cfg ttbarxsec.tmp
    #./updateconfig.py renscale -1
    #./jobsub ${TYPUNC}/1.0y/rsDown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py renscale 1
    #./jobsub ${TYPUNC}/1.0y/rsUp ttbarxsec.exe ttbarxsec.cfg
    #mv ttbarxsec.tmp ttbarxsec.cfg

    #jer/jes: jer < 1 % + jes ~5-7 % 
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py sigmajet -1
    ./jobsub ${TYPUNC}/1.0y/JESDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py sigmajet 1
    ./jobsub ${TYPUNC}/1.0y/JESUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg
    
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py jetres -1
    ./jobsub ${TYPUNC}/1.0y/JERDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py jetres 1
    ./jobsub ${TYPUNC}/1.0y/JERUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

    #MET: < 1 %
    #cp ttbarxsec.cfg ttbarxsec.tmp
    #./updateconfig.py sigmamet -1
    #./jobsub ${TYPUNC}/1.0y/METDown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py sigmamet 1
    #./jobsub ${TYPUNC}/1.0y/METUp ttbarxsec.exe ttbarxsec.cfg
    #mv ttbarxsec.tmp ttbarxsec.cfg

    #bunch of reweight factors: seems like you don't need those later, but remember to report your 2d YUKAWA reweighting uncertainties
    #./updateconfig.py topptweight -1
    #./jobsub ${TYPUNC}_topdown_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py topptweight 1
    #./jobsub ${TYPUNC}_topup_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py toprapweight -1
    #./jobsub ${TYPUNC}_trapdown_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py toprapweight 1
    #./jobsub ${TYPUNC}_trapup_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py ttptweight -1
    #./jobsub ${TYPUNC}_ttptdown_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py ttptweight 1
    #./jobsub ${TYPUNC}_ttptup_${VER} ttbarxsec.exe ttbarxsec.cfg


    #btag: includes b and light jets tags, vary scale factor up/down; total ~2-3 % depends on the pT and b jets
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py btagunc -1
    ./jobsub ${TYPUNC}/1.0y/btagDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py btagunc 1
    ./jobsub ${TYPUNC}/1.0y/btagUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg
    #mis-btag: 
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py ltagunc -1
    ./jobsub ${TYPUNC}/1.0y/ltagDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py ltagunc 1
    ./jobsub ${TYPUNC}/1.0y/ltagUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

    #pileup: varies the minimum bias cross section by 5%, reweighting the MC; < 1 % 
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py pileupunc -1
    ./jobsub ${TYPUNC}/1.0y/pileupDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py pileupunc 1
    ./jobsub ${TYPUNC}/1.0y/pileupUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg
    
fi





