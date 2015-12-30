#!/usr/bin/env python2
#run this script in the job collection directory to add all output files in the subdirectories
import sys, os, time

allfiles = os.listdir('.')

jobdirs = [d for d in allfiles if os.path.isdir(d)]

for dirn in jobdirs:
	print dirn
	if dirn + '.root' in allfiles:
		 continue
	
	files = os.listdir(dirn)
	nums = [int(f.split('_')[-1].split('.jdl')[0]) for f in files if '.jdl' in f]
	#confile = open(dir+'/condor.jdl', 'r')
	#info = confile.read().split('\n')
	#confile.close()
	#info = [l.strip() for l in info if 'Queue' in l]
	num =  max(nums)+1
	files = os.listdir(dirn)
	files = [f for f in files if ('%s_out_' % dirn) in f]
	print num, len(files)
	missing = [n for n in range(0, num) if '{0}_out_{1}.root'.format(dirn, n) not in files]
	print missing
	for j in missing:
		command = 'cd '+dirn+' && condor_submit condor_' + str(j) + '.jdl && cd ..'
		print command
		os.system(command)
