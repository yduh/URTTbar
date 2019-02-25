#!/usr/bin/env python2
#run this script in the job collection directory to add all output files in the subdirectories
import sys, os, time

GT = ['0.0y','1.0y','2.0y','3.0y','4.0y','5.0y']

for gt in GT:
    allfiles = os.listdir(gt)

    jobdirs = [d for d in allfiles if os.path.isdir(d)]

    for dir in jobdirs:
            print dir
            if dir + '.root' in allfiles:
                print dir + "root files exits !"
                continue

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



