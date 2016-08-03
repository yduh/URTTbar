#!/bin/bash

TYP=yukawa_rootfiles/5jabove
JOBDIR=JOB11
GT='0.0y 1.0y 2.0y 3.0y 4.0y 5.0y'

RUN=true
RUNMAINUNC=true
RUNOUNC=false


if $RUN; then
    for gt in $GT
    do
        echo "Submit jobs for YUKAWA = ${gt} ..."
        mkdir -p ${TYP}
        cp ttbarxsec.cfg ttbarxsec.tmp
    
        rm inputs/$JOBDIR/*txt
        cp inputs/$JOBDIR/backup/*txt inputs/$JOBDIR
        ./updateconfig.py yukawatxt yukawa_reweighting${gt}.root
        ./jobsub ${TYP}/${gt} ttbarxsec.exe ttbarxsec.cfg
    
        mv ttbarxsec.tmp ttbarxsec.cfg
    done
fi


if $RUNMAINUNC; then
    for gt in $GT
    do
        echo "Submit jobs for theoretical uncertainties YUKAWA = ${gt} ..."
        mkdir -p ${TYP}
        cp ttbarxsec.cfg ttbarxsec.tmp
        
        rm inputs/$JOBDIR/*txt
        cp inputs/$JOBDIR/backup_theoreticaluncert/*txt inputs/$JOBDIR
        ./updateconfig.py yukawatxt yukawa_reweighting${gt}.root
        ./jobsub ${TYP}/${gt}/generators ttbarxsec.exe ttbarxsec.cfg
       
        rm inputs/$JOBDIR/*txt
        cp inputs/$JOBDIR/backup_theoreticaluncert/mtop/tt_mtop1695_PowhegP8.txt inputs/$JOBDIR
        ./updateconfig.py yukawatxt yukawa_reweighing${gt}_169.5.root 
        ./jobsub ${TYP}/${gt}/mtdown ttbarxsec.exe ttbarxsec.cfg

        rm inputs/$JOBDIR/*txt
        cp inputs/$JOBDIR/backup_theoreticaluncert/mtop/tt_mtop1755_PowhegP8.txt inputs/$JOBDIR
        ./updateconfig.py yukawatxt yukawa_reweighing${gt}_175.5.root 
        ./jobsub ${TYP}/${gt}/mtup ttbarxsec.exe ttbarxsec.cfg
        
        mv ttbarxsec.tmp ttbarxsec.cfg
    done
fi


if $RUNOUNC; then
    for gt in $GT
    do
        echo "Submit jobs for other uncertainties YUKAWA = ${gt} ..."
        mkdir -p ${TYP}
        cp ttbarxsec.cfg ttbarxsec.tmp

    #PDF uncertainty is estimated with all MCs
    #============================================================#
    rm inputs/$JOBDIR/*txt
    cp inputs/$JOBDIR/backup/*txt inputs/$JOBDIR
    #PDF: total < 1 %
    ./updateconfig.py PDFTEST 1
    ./jobsub ${TYP}/${gt}/pdf ttbarxsec.exe ttbarxsec.cfg

    
    #lepton uncertainties are estimated with all MC except some sparated theoretical signal ones 
    #============================================================#
    rm inputs/$JOBDIR/*txt
    cp inputs/$JOBDIR/backupsmall/*txt inputs/$JOBDIR
    #leptons: varies up/down scale factor gotten by tag and probe method; total ~3 %
    ./updateconfig.py ELECTRONS 0
    ./jobsub ${TYP}/${gt}/mu ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py MUONS 0
    ./jobsub ${TYP}/${gt}/el ttbarxsec.exe ttbarxsec.cfg


    #uncertainties are estimated only with signal MC
    #============================================================#
    rm inputs/$JOBDIR/*txt
    cp inputs/$JOBDIR/backup/tt_PowhegP8.txt inputs/$JOBDIR

    #jer/jes: jer < 1 % + jes ~5-7 % 
    ./updateconfig.py sigmajet -1
    ./jobsub ${TYP}/${gt}/jetm1sig ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py sigmajet 1
    ./jobsub ${TYP}/${gt}/jetp1sig ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py jetres -1
    ./jobsub ${TYP}/${gt}/jetresm ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py jetres 1
    ./jobsub ${TYP}/${gt}/jetresp ttbarxsec.exe ttbarxsec.cfg

    #MET: < 1 %
    ./updateconfig.py sigmamet -1
    ./jobsub ${TYP}/${gt}/metm1sig ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py sigmamet 1
    ./jobsub ${TYP}/${gt}/metp1sig ttbarxsec.exe ttbarxsec.cfg

    #bunch of reweight factors: seems like you don't need those later, but remember to report your 2d YUKAWA reweighting uncertainties
    #./updateconfig.py topptweight -1
    #./jobsub ${TYP}_topdown_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py topptweight 1
    #./jobsub ${TYP}_topup_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py toprapweight -1
    #./jobsub ${TYP}_trapdown_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py toprapweight 1
    #./jobsub ${TYP}_trapup_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py ttptweight -1
    #./jobsub ${TYP}_ttptdown_${VER} ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py ttptweight 1
    #./jobsub ${TYP}_ttptup_${VER} ttbarxsec.exe ttbarxsec.cfg

    #factorization and renormalization: total < 1 %
    ./updateconfig.py renscale -1
    ./jobsub ${TYP}/${gt}/rsdown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py renscale 1
    ./jobsub ${TYP}/${gt}/rsup ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py facscale -1
    ./jobsub ${TYP}/${gt}/fsdown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py facscale 1
    ./jobsub ${TYP}/${gt}/fsup ttbarxsec.exe ttbarxsec.cfg

    #btag: includes b and light jets tags, vary scale factor up/down; total ~2-3 % depends on the pT and b jets
    ./updateconfig.py btagunc -1
    ./jobsub ${TYP}/${gt}/btagdown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py btagunc 1
    ./jobsub ${TYP}/${gt}/btagup ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py ltagunc -1
    ./jobsub ${TYP}/${gt}/ltagdown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py ltagunc 1
    ./jobsub ${TYP}/${gt}/ltagup ttbarxsec.exe ttbarxsec.cfg

    #pileup: varies the minimum bias cross section by 5%, reweighting the MC; < 1 % 
    ./updateconfig.py pileupunc -1
    ./jobsub ${TYP}/${gt}/pileupdown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py pileupunc 1
    ./jobsub ${TYP}/${gt}/pileupup ttbarxsec.exe ttbarxsec.cfg
    
    mv ttbarxsec.tmp ttbarxsec.cfg
done
fi

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




