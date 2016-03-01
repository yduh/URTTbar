#!/usr/bin/env python2
#run this script in the job collection directory to add all output files in the subdirectories
import sys, os, time

typ = 'D'
if len(sys.argv) == 2:
	typ = sys.argv[1]

allfiles = os.listdir('.')

jobdirs = [d for d in allfiles if os.path.isdir(d)]

for dir in jobdirs:

	files = os.listdir(dir)
	nums = [int(f.split('_')[-1].split('.jdl')[0]) for f in files if '.jdl' in f]
	#confile = open(dir+'/condor.jdl', 'r')
	#info = confile.read().split('\n')
	#confile.close()
	#info = [l.strip() for l in info if 'Queue' in l]
	num =  max(nums)+1
	files = os.listdir(dir)
	files = [f for f in files if ('%s_out_' % dir) in f]
	failed = [f for f in files if os.path.getsize(os.path.join(dir, f)) < 1024*100]
	failed_suspicious= [f for f in files if os.path.getsize(os.path.join(dir, f)) < 1024*100]

	for n in failed:
		num = n.split('_')[-1].split('.')[0]
		command = 'cd ' + dir + ' && rm ' + n + ' && condor_submit condor_' + num + '.jdl && cd ..'
		print dir, command
		if typ == 'R':
			os.system(command)
'''
        for n in failed_suspicious:
            num = n.split('_')[-1].split('.')[0]
            command = 'cd' + dir
'''
