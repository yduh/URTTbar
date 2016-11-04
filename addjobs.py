#!/usr/bin/env python2
#run this script in the job collection directory to add all output files in the subdirectories
import sys, os, time
import pprint

if(os.path.exists("./mtup")):
    os.system("mv mtup/tt_mtop1755_PowhegP8 ./")
    os.system("rm -r mtup")
if(os.path.exists("./mtdown")):
    os.system("mv mtdown/tt_mtop1695_PowhegP8 ./")
    os.system("rm -r mtdown")

allfiles = os.listdir("./")

jobdirs = [d for d in allfiles if os.path.isdir(d)]

for dir in jobdirs:
    if dir + '.root' in allfiles:
        print dir + ".root files exits !"
        continue
    print dir

    files = os.listdir(dir)
    nums = [int(f.split('_')[-1].split('.jdl')[0]) for f in files if '.jdl' in f]
    #confile = open(dir+'/condor.jdl', 'r')
    #info = confile.read().split('\n')
    #confile.close()
    #info = [l.strip() for l in info if 'Queue' in l]
    num =  max(nums)+1
    files = os.listdir(dir)
    files = [f for f in files if ('%s_out_' % dir) in f]
    print num, len(files)
    if num == len(files):
        files = [dir + '/' + f for f in files]
        command = 'hadd ' + dir + '.root ' + ' '.join(files)
        print command
        os.system(command)
    else:
        print "missing " + dir + ", check missing.txt"
        txt = open("missing.txt", 'wt')
        txt.write("HAVE: " + dir + "\n\n")
        for f in files:
            txt.write(f + "\n")
        txt.close()



