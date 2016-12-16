#!/bin/bash

TYP=results/newdataset/4j
TYPUNC=results/6junc
JOBDIR=JOB13
#GT='0.0y 1.0y 2.0y 3.0y 4.0y 5.0y N1.0y N2.0y N3.0y N4.0y N5.0y'
#GT='0.0y 1.0y 2.0y 3.0y 4.0y 5.0y'
GT='1.0y'

RUN=true
RUNOUNC=false


if $RUN; then
    for gt in $GT
    do
        echo "Submit jobs for YUKAWA = ${gt} ..."
        mkdir -p ${TYP}
        cp ttbarxsec.cfg ttbarxsec.tmp
   
        rm inputs/$JOBDIR/*txt
        rm inputs/$JOBDIR/INPUT/yukawa_reweighting*.root

        if [ "${gt}" = "1.0y" ]; then
            cp inputs/$JOBDIR/backup/*txt inputs/$JOBDIR
            #cp inputs/$JOBDIR/backup_theoreticaluncert/*txt inputs/$JOBDIR
            cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting${gt}.root inputs/$JOBDIR/INPUT
            ./updateconfig.py yukawatxt yukawa_reweighting${gt}.root
            ./jobsub ${TYP}/${gt} ttbarxsec.exe ttbarxsec.cfg
        else
            cp inputs/$JOBDIR/backup/tt_PowhegP8.txt inputs/$JOBDIR
            cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting${gt}.root inputs/$JOBDIR/INPUT 
            ./updateconfig.py yukawatxt yukawa_reweighting${gt}.root
            ./jobsub ${TYP}/${gt} ttbarxsec.exe ttbarxsec.cfg
        fi

        #if [ "${gt}" = "1.0y" ]; then
        #    rm inputs/$JOBDIR/*txt
        #    cp inputs/$JOBDIR/backup_theoreticaluncert/mtop/tt_mtop1695_PowhegP8.txt inputs/$JOBDIR
        #    cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting${gt}_169.5.root inputs/$JOBDIR/INPUT
        #    ./updateconfig.py yukawatxt yukawa_reweighting${gt}_169.5.root 
        #    ./jobsub ${TYP}/${gt}/mtdown ttbarxsec.exe ttbarxsec.cfg

        #    rm inputs/$JOBDIR/*txt
        #    cp inputs/$JOBDIR/backup_theoreticaluncert/mtop/tt_mtop1755_PowhegP8.txt inputs/$JOBDIR
        #    cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting${gt}_175.5.root inputs/$JOBDIR/INPUT
        #    ./updateconfig.py yukawatxt yukawa_reweighting${gt}_175.5.root 
        #    ./jobsub ${TYP}/${gt}/mtup ttbarxsec.exe ttbarxsec.cfg
        #fi

        mv ttbarxsec.tmp ttbarxsec.cfg
    done
fi


if $RUNOUNC; then
    echo "Submit jobs for other uncertainties YUKAWA = 1.0y ..."
    mkdir -p ${TYPUNC}/1.0y
    cp ttbarxsec.cfg ttbarxsec.tmp

    rm inputs/$JOBDIR/INPUT/yukawa_reweighting*.root
    cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting1.0y.root inputs/$JOBDIR/INPUT

    #PDF uncertainty is estimated with all MCs
    #============================================================#
    #rm inputs/$JOBDIR/*txt
    #cp inputs/$JOBDIR/backup/*txt inputs/$JOBDIR
    #PDF: total < 1 %
    #./updateconfig.py PDFTEST 1
    #./jobsub ${TYPUNC}/1.0y/pdf ttbarxsec.exe ttbarxsec.cfg

    
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

    #jer/jes: jer < 1 % + jes ~5-7 % 
    ./updateconfig.py sigmajet -1
    ./jobsub ${TYPUNC}/1.0y/jetm1sig ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py sigmajet 1
    ./jobsub ${TYPUNC}/1.0y/jetp1sig ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py jetres -1
    #./jobsub ${TYPUNC}/1.0y/jetresm ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py jetres 1
    #./jobsub ${TYPUNC}/1.0y/jetresp ttbarxsec.exe ttbarxsec.cfg

    #MET: < 1 %
    #./updateconfig.py sigmamet -1
    #./jobsub ${TYPUNC}/1.0y/metm1sig ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py sigmamet 1
    #./jobsub ${TYPUNC}/1.0y/metp1sig ttbarxsec.exe ttbarxsec.cfg

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

    #factorization and renormalization: total < 1 %
    #./updateconfig.py renscale -1
    #./jobsub ${TYPUNC}/1.0y/rsdown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py renscale 1
    #./jobsub ${TYPUNC}/1.0y/rsup ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py facscale -1
    #./jobsub ${TYPUNC}/1.0y/fsdown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py facscale 1
    #./jobsub ${TYPUNC}/1.0y/fsup ttbarxsec.exe ttbarxsec.cfg

    #btag: includes b and light jets tags, vary scale factor up/down; total ~2-3 % depends on the pT and b jets
    #./updateconfig.py btagunc -1
    #./jobsub ${TYPUNC}/1.0y/btagdown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py btagunc 1
    #./jobsub ${TYPUNC}/1.0y/btagup ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py ltagunc -1
    #./jobsub ${TYPUNC}/1.0y/ltagdown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py ltagunc 1
    #./jobsub ${TYPUNC}/1.0y/ltagup ttbarxsec.exe ttbarxsec.cfg

    #pileup: varies the minimum bias cross section by 5%, reweighting the MC; < 1 % 
    #./updateconfig.py pileupunc -1
    #./jobsub ${TYPUNC}/1.0y/pileupdown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py pileupunc 1
    #./jobsub ${TYPUNC}/1.0y/pileupup ttbarxsec.exe ttbarxsec.cfg
    
    mv ttbarxsec.tmp ttbarxsec.cfg
fi





