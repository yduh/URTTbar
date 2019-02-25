#!/bin/bash

njets="$1"
TYP=results/${njets}
TYPUNC=results/${njets}unc
JOBDIR=JOB15
GT='noEW2'

#Submit on your own: for nominal case of SR, sideband, and uncertainties with dedicate MC smples!!!

RUN_YtCases=false	#txt: ttbar + copy INPUT root files
RUNUNC_TH1=true		#txt: ttbar
RUNUNC_TH2=false	#txt: ttbar, st
RUNUNC_EXP1=false	#txt: ttbar, st
RUNUNC_EXP2=false	#txt: all


GTCASES='0.0y 1.0y 2.0y 3.0y 4.0y 5.0y'
if $RUN_YtCases; then
    for gt in $GTCASES
    do
        echo "Submit jobs for YUKAWA secarino= ${gt} ..."
        mkdir -p ${TYP}
        cp ttbarxsec.cfg ttbarxsec.tmp
   
        if [ "${gt}" = "noEW" ]; then
            ./updateconfig.py yukawatxt ${gt}
            ./jobsub ${TYP}/${gt} ttbarxsec.exe ttbarxsec.cfg
        else
            ./updateconfig.py yukawatxt yukawa_reweighting${gt}.root
            ./jobsub ${TYP}/${gt} ttbarxsec.exe ttbarxsec.cfg
        fi
        
        mv ttbarxsec.tmp ttbarxsec.cfg
    done
fi

if $RUNUNC_TH1; then
    echo "Submit jobs for uncertainties at YUKAWA = "${GT}
    mkdir -p ${TYPUNC}/${GT}

    #fractorization scale
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py facscale -1
    ./jobsub ${TYPUNC}/${GT}/fsDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py facscale 1
    ./jobsub ${TYPUNC}/${GT}/fsUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg
    
    #renormalization scale
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py renscale -1
    ./jobsub ${TYPUNC}/${GT}/rsDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py renscale 1
    ./jobsub ${TYPUNC}/${GT}/rsUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

    #renormalization + fractorization scales
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py renscale -1 facscale -1
    ./jobsub ${TYPUNC}/${GT}/rsfsDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py renscale 1 facscale 1
    ./jobsub ${TYPUNC}/${GT}/rsfsUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

	#for the stupid post unblinding test (not part of the top PAG recommendation)
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py renscale 1 facscale -1
    ./jobsub ${TYPUNC}/${GT}/rsfsOSDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py renscale -1 facscale 1
    ./jobsub ${TYPUNC}/${GT}/rsfsOSUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg


	#pT reweight to nnlo calculation
    #cp ttbarxsec.cfg ttbarxsec.tmp
    #./updateconfig.py nnlopT 1
    #./jobsub ${TYPUNC}/${GT}/nnlopt ttbarxsec.exe ttbarxsec.cfg
    #mv ttbarxsec.tmp ttbarxsec.cfg
	
fi

# input txt copy: STs.txt  STt_top.txt  STt_topbar.txt  Wt.txt  Wtbar.txt  tt_PowhegP8.txt
if $RUNUNC_TH2; then
    echo "Submit jobs for uncertainties at YUKAWA = "${GT}
    mkdir -p ${TYPUNC}/${GT}

	#PDF: total < 1 %
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py PDFTEST 1
    ./jobsub ${TYPUNC}/${GT}/pdf ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

    #PS: b-jet fragmentation
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py bfrag -1
    ./jobsub ${TYPUNC}/${GT}/bfragDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py bfrag 1
    ./jobsub ${TYPUNC}/${GT}/bfragUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

    #PS: B decay BR
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py bdecay -1
    ./jobsub ${TYPUNC}/${GT}/bdecayDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py bdecay 1
    ./jobsub ${TYPUNC}/${GT}/bdecayUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

fi


# input txt copy: STs.txt  STt_top.txt  STt_topbar.txt  Wt.txt  Wtbar.txt  tt_PowhegP8.txt
if $RUNUNC_EXP1; then
    echo "Submit jobs for uncertainties at YUKAWA = "${GT}
    mkdir -p ${TYPUNC}/${GT}

    #leptons: varies up/down scale factor gotten by tag and probe method
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py sigmalep -1
    ./jobsub ${TYPUNC}/${GT}/lepDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py sigmalep 1
    ./jobsub ${TYPUNC}/${GT}/lepUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

    #jer: jer < 1 % 
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py jetres -1
    ./jobsub ${TYPUNC}/${GT}/JERDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py jetres 1
    ./jobsub ${TYPUNC}/${GT}/JERUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

    #MET: < 1 %
    #cp ttbarxsec.cfg ttbarxsec.tmp
    #./updateconfig.py sigmamet -1
    #./jobsub ${TYPUNC}/${GT}/METDown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py sigmamet 1
    #./jobsub ${TYPUNC}/${GT}/METUp ttbarxsec.exe ttbarxsec.cfg
    #mv ttbarxsec.tmp ttbarxsec.cfg


    #btag: includes b and light jets tags, vary scale factor up/down; total ~2-3 % depends on the pT and b jets
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py btagunc -1
    ./jobsub ${TYPUNC}/${GT}/btagDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py btagunc 1
    ./jobsub ${TYPUNC}/${GT}/btagUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

    #mis-btag: 
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py ltagunc -1
    ./jobsub ${TYPUNC}/${GT}/ltagDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py ltagunc 1
    ./jobsub ${TYPUNC}/${GT}/ltagUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

    #pileup: varies the minimum bias cross section by 5%, reweighting the MC; < 1 % 
    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py pileupunc -1
    ./jobsub ${TYPUNC}/${GT}/pileupDown ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py pileupunc 1
    ./jobsub ${TYPUNC}/${GT}/pileupUp ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

fi

#input txt copy: all
#DATAEL.txt  DYJets.txt    QCDEM170.txt  QCDEM50.txt  QCDEMInf.txt   QCDMu120.txt  QCDMu300.txt  QCDMu50.txt   QCDMu80.txt   QCDMuInf.txt  STt_top.txt     Wt.txt     tt_PowhegP8.txt
#DATAMU.txt  QCDEM120.txt  QCDEM300.txt  QCDEM80.txt  QCDMu1000.txt  QCDMu170.txt  QCDMu470.txt  QCDMu600.txt  QCDMu800.txt  STs.txt       STt_topbar.txt  Wtbar.txt
if $RUNUNC_EXP2; then
    echo "Submit jobs for uncertainties at YUKAWA = "${GT}
    mkdir -p ${TYPUNC}/${GT}

    cp ttbarxsec.cfg ttbarxsec.tmp
    ./updateconfig.py runSR 0 runCSV -1
    ./jobsub ${TYPUNC}/${GT}/csv2Down ttbarxsec.exe ttbarxsec.cfg
    ./updateconfig.py runSR 0 runCSV 1
    ./jobsub ${TYPUNC}/${GT}/csv2Up ttbarxsec.exe ttbarxsec.cfg
    mv ttbarxsec.tmp ttbarxsec.cfg

fi

    #bunch of things that it seems you don't need it anymore
    #cp ttbarxsec.cfg ttbarxsec.tmp
    #./updateconfig.py sigmajet -1
    #./jobsub ${TYPUNC}/${GT}/JESDown ttbarxsec.exe ttbarxsec.cfg
    #./updateconfig.py sigmajet 1
    #./jobsub ${TYPUNC}/${GT}/JESUp ttbarxsec.exe ttbarxsec.cfg
    #mv ttbarxsec.tmp ttbarxsec.cfg

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

#if $RUNMASS; then
#    echo "submit jobs for YUKAWA = 1.0y ..."
#    cp ttbarxsec.cfg ttbarxsec.tmp
#    #cp inputs/$JOBDIR/INPUT_TEMP/yukawa_reweighting${gt}_169.5.root inputs/$JOBDIR/INPUT
#    ./updateconfig.py yukawatxt yukawa_reweighting1.0y_169.5.root 
#    ./jobsub ${TYPUNC}/1.0y/mtDown ttbarxsec.exe ttbarxsec.cfg
#    mv ttbarxsec.tmp ttbarxsec.cfg
#fi





