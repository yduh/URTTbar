#!/usr/bin/env python2
import sys, os

updates = {}

for i in range(0, (len(sys.argv)-1)/2):
	updates[sys.argv[i+1]] = sys.argv[i+2]


print updates


inf = open('ttbarxsec.tmp', 'r')
outf = open('ttbarxsec.cfg', 'w')

for l in inf:
	if '=' in l:
		val = l.split('=')[0].strip()
		if val in updates:
			outf.write(val + ' = ' + updates[val] + '\n')
		else:
			outf.write(l)


inf.close()
outf.close()
