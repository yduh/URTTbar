#!/bin/python2
import sys, os, re

def nameandlength(string):
	string = string.strip()
	temp = string.split('[')
	name = temp[0]
	length = -1
	if len(temp) == 2: 
		endpos = temp[1].find(']')
		length = int(temp[1][:endpos])
	return([name, length])


def linesplit(line):
	line = line.strip()
	vec = line.split(':', 1)
	vec[0] = vec[0].strip() 
	vec[1] = vec[1].strip() 
	return vec

def cleandatamembers(string):
	vec = string.split(',')
	vec = [v.strip() for v in vec if '[' not in v]
	return vec

def cleandatavecs(string):
	vec = string.split(',')
	vec = [v.strip() for v in vec if '[' in v]
	res = []
	for d in vec:
		res.append(nameandlength(d))
	return res

def getbranchinfo(string):
	vec = string.split(',')
	return([nameandlength(v) for v in vec])
		


class CLASS:
	ERRORNUM = 0
	TYPS = {'D' : 'Double_t', 'F' : 'Float_t' , 'I' : 'Int_t', 'i' : 'UInt_t', 'L' : 'Long64_t', 'l' : 'ULong64_t', 'B' : 'Char_t', 'b': 'UChar_t', 'S' : 'Short_t', 's' : 'UShort_t', 'O': 'Bool_t'}
	def __init__(self, name, config):
		print name
		print config
		self.datamember = {}
		self.datavecs = {}
		self.vectorclass = False
		self.name = name
		for line in config:
			info = linesplit(line)
			if info[0] == 'BeginClass':
				vinfo = info[1].split(':')
				continue
			for TYP in CLASS.TYPS:
				if info[0] == TYP:
					if TYP in self.datamember:
						self.datamember[TYP] += cleandatamembers(info[1])
					else:
						self.datamember[TYP] = cleandatamembers(info[1])
					if TYP in self.datavecs:
						self.datavecs[TYP] += cleandatavecs(info[1])
					else:
						self.datavecs[TYP] = cleandatavecs(info[1])
					continue
			if info[0].startswith('MemberClass'):
				TYP = info[0].split('MemberClass')[1].strip()
				if TYP in self.datamember:
					self.datamember[TYP] += cleandatamembers(info[1])
				else:
					self.datamember[TYP] = cleandatamembers(info[1])
				if TYP in self.datavecs:
					self.datavecs[TYP] += cleandatavecs(info[1])
				else:
					self.datavecs[TYP] = cleandatavecs(info[1])
				continue
		print self.datamember
		print self.datavecs

	def writeclassdef(self):
		classdef = ''
#		classdef += '#ifndef CLASS_' + self.name.upper() + '\n'		
#		classdef += '#define CLASS_' + self.name.upper() + '\n'		
#		classdef += '#include "Data_' + self.name + '.h"\n'		
#		for typ, des in self.datamember.iteritems():
#			if typ not in CLASS.TYPS:
#				classdef += 'class ' + typ + ';\n'		
#		classdef += 'class BaseIO;\n\n'		

		classdef +=  'class ' + self.name
		classdef += '\n {\n'
		classdef += '\tfriend class BaseIO;\n'
		classdef += '\tprivate:\n'
		classdef += '\t\tstatic BaseIO* baseio;\n'
		classdef += '\t\tUInt_t number_;\n'
		classdef += '\t\tData_' + self.name + '* data_;\n'
		
		classdef += '\tpublic:\n'
		
		classdef += '\t\t'+ self.name + '(Data_' + self.name + '* data, UInt_t number);\n'
		classdef += '\t\t'+ self.name + '(const '+self.name+'& _' + self.name.lower() +');\n'
		classdef += '\t\tvoid Init();\n'
		#Getters
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
			for mem in des:
				classdef += '\t\t' + typ + ' ' + mem + '() const;\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
			for mem in des:
				classdef += '\t\t' + typ + ' ' + mem[0] + '(UInt_t n) const;\n'
				classdef += '\t\t' + 'UInt_t' + ' Num_' + mem[0] + '() const;\n'
		#Setters
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
				for mem in des:
					classdef += '\t\tvoid ' + mem + '('+typ+' _'+mem+');\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
				for mem in des:
					classdef += '\t\tvoid ' + mem[0] + '('+typ+' _'+mem[0]+', UInt_t n);\n'
		classdef += ' };\n'
#		classdef += '#endif\n'		
		return classdef

	def writeclasscode(self):
		classcode = ''
#		classcode += '#include "BaseIO.h"\n'		
#		classcode += '#include "' + self.name + '.h"\n'		
#		for typ, des in self.datamember.iteritems():
#			if typ not in CLASS.TYPS:
#				classcode += '#include "' + typ + '.h"\n'		
		classcode += 'BaseIO* '+self.name+'::baseio = 0;\n'
		classcode += self.name+'::'+self.name + '(Data_' + self.name + '* data, UInt_t number) : \nnumber_(number),\ndata_(data)\n'
		classcode += '{\n'
		classcode += '\tInit();\n'
		classcode += '}\n\n'
		classcode += self.name+'::'+self.name + '(const '+self.name+'& _' + self.name.lower() +') : \nnumber_('+' _' + self.name.lower()+'.number_),\ndata_('+' _' + self.name.lower()+'.data_)\n'
		classcode += '{\n'
		classcode += '}\n\n'
		classcode += 'void ' + self.name+'::Init()\n'
		classcode += '{\n'
		classcode += '\tif(baseio->IsWritable())\n'
		classcode += '\t{\n'
		for typ, des in self.datavecs.iteritems():
			for mem in des:
				classcode += '\t\tif(number_ == 0) {data_->' + mem[0] + '_num_[number_] = 0;}\n'
				classcode += '\t\telse {data_->' + mem[0] + '_num_[number_] = data_->' + mem[0] + '_num_[number_-1];}\n'
		classcode += '\t\tdata_->count_ = number_+1;\n'
		classcode += '\t}\n'
		classcode += '}\n\n'
		#Getters	
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
				for mem in des:
					classcode += typ + ' ' + self.name + '::' + mem + '() const\n'
					classcode += '{\n'
					classcode += '\treturn data_->' + mem +'_[number_];\n' 
					classcode += '}\n\n'
			else:
				for mem in des:
					classcode += typ + ' ' + self.name + '::' + mem + '() const\n'
					classcode += '{\n'
					classcode += '\treturn '+typ+'(data_->' + mem +'_, number_);\n' 
					classcode += '}\n\n'

		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
				for mem in des:
					classcode += 'UInt_t ' + self.name + '::Num_' + mem[0] + '() const\n'
					classcode += '{\n'
					classcode += '\treturn number_ == 0 ? data_->' + mem[0] +'_num_[number_] : data_->' + mem[0] +'_num_[number_] - data_->' + mem[0] +'_num_[number_-1];\n' 
					classcode += '}\n\n'
					classcode += typ + ' ' + self.name + '::' +  mem[0] + '(UInt_t n) const\n'
					classcode += '{\n'
					classcode += '\treturn number_ == 0 ? data_->' + mem[0] +'_[n] : data_->' + mem[0] +'_[data_->' + mem[0] +'_num_[number_-1]  + n];\n' 
					classcode += '}\n\n'
			else:
				for mem in des:
					classcode += 'UInt_t ' +self.name +'::Num_' + mem[0] + '() const\n'
					classcode += '{\n'
					classcode += '\treturn number_ == 0 ? data_->' + mem[0] +'_num_[number_] : data_->' + mem[0] +'_num_[number_] - data_->' + mem[0] +'_num_[number_-1];\n' 
					classcode += '}\n\n'
					classcode += typ + ' ' + self.name + '::' + mem[0] + '(UInt_t n) const\n'
					classcode += '{\n'
					classcode += '\tif(baseio->IsWritable())\n'
					classcode += '\t{\n'
					classcode += '\t\tdata_->' + mem[0] + '_num_[number_]++;\n'
					classcode += '\t}\n'
					classcode += '\treturn number_ == 0 ? '+typ+'(data_->' + mem[0] +'_, n) : '+typ+'(data_->' + mem[0] +'_, data_->' + mem[0] + '_num_[number_-1]  + n);\n' 
					classcode += '}\n\n'
		#Setters
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
				for mem in des:
					classcode += 'void ' + self.name + '::' + mem + '('+typ+' _'+mem+')\n'
					classcode += '{\n'
					classcode += '\tif(number_ >= data_->size_)\n'
					classcode += '\t{\n'
					CLASS.ERRORNUM+=1
					classcode += '\t\tbaseio->SetError('+str(CLASS.ERRORNUM)+');//ERROR'+str(CLASS.ERRORNUM)+'\n'
					classcode += '\t\treturn;\n'
					classcode += '\t}\n'
					classcode += '\tdata_->' + mem + '_[number_] = _'+mem+';\n'
					classcode += '}\n\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
				for mem in des:
					classcode += 'void ' + self.name + '::' + mem[0] + '('+typ+' _'+mem[0]+', UInt_t n)\n'
					classcode += '{\n'
					classcode += '\tif(number_ != 0){n = data_->' + mem[0] + '_num_[number_-1] +n;}\n'
					classcode += '\tif(n >= data_->' + mem[0] + '_countmax_)\n'
					classcode += '\t{\n'
					CLASS.ERRORNUM+=1
					classcode += '\t\tbaseio->SetError('+str(CLASS.ERRORNUM)+');//ERROR'+str(CLASS.ERRORNUM)+'\n'
					classcode += '\t\treturn;\n'
					classcode += '\t}\n'
					classcode += '\tif(n != data_->' + mem[0] + '_count_)\n'
					classcode += '\t{\n'
					classcode += '\t\tcerr << "Index already filled" << endl;\n'
					classcode += '\t\treturn;\n'
					classcode += '\t}\n'
					classcode += '\tdata_->' + mem[0] + '_[n] = _' + mem[0] +';\n'
					classcode += '\tdata_->' + mem[0] + '_num_[number_]++;\n'
					classcode += '\tdata_->' + mem[0] + '_count_++;\n'
					classcode += '}\n\n'
		return classcode

	def writedataclassdef(self):
		classdef = ''
#		classdef += '#ifndef CLASS_DATA_' + self.name.upper() + '\n'		
#		classdef += '#define CLASS_DATA_' + self.name.upper() + '\n'		
#		classdef += '#include <string>\n'		
#		classdef += '#include "TTree.h"\n'		
#		for typ, des in self.datamember.iteritems():
#			if typ not in CLASS.TYPS:
#				#classdef += '#include "Data_' + typ + '.h"\n'		
#				classdef += 'class Data_' + typ + ';\n'		
#		classdef += '\nclass BaseIO;\n'

		classdef += '\nclass ' + 'Data_' + self.name
		classdef += '\n {\n'
		classdef += '\tfriend class ' + self.name +';\n'
		classdef += '\tfriend class BaseIO;\n'
		classdef += '\tprivate:\n'
		classdef += '\t\tstatic BaseIO* baseio;\n'
		classdef += '\t\tUInt_t size_;\n'
		classdef += '\t\tstd::string prefix_;\n'
		classdef += '\t\t' + 'UInt_t count_;\n'
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
				for mem in des:
					classdef += '\t\t' + typ + '* ' + mem + '_;\n'
			else:
				typ = 'Data_' + typ
				for mem in des:
					classdef += '\t\t' + typ + '* ' + mem + '_;\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				typ = CLASS.TYPS[typ]
				for mem in des:
					classdef += '\t\t' + 'UInt_t '+mem[0]+'_count_;\n'
					classdef += '\t\t' + 'UInt_t '+mem[0]+'_countmax_;\n'
					classdef += '\t\t' + 'UInt_t*' + ' ' + mem[0] + '_num_;\n'
					classdef += '\t\t' + typ + '* ' + mem[0] + '_;\n'
			else:
				typ = 'Data_' + typ
				for mem in des:
					classdef += '\t\t' + 'UInt_t*' + ' ' + mem[0] + '_num_;\n'
					classdef += '\t\t' + typ + '* ' + mem[0] + '_' + ';\n'
		classdef += '\tpublic:\n'
		classdef += '\t\tvoid Fill();\n'
		classdef += '\t\t' + 'Data_' + self.name + '(UInt_t size, std::string prefix);\n'
		classdef += '\t\t' + '~Data_' + self.name + '();\n'
		classdef += '\t\tvoid SetUpWrite(TTree* tree);\n'
		classdef += '\t\tvoid SetUpRead(TTree* tree);\n'
		classdef += '\t\tvoid Load(TTree* tree, bool load);\n'
		
		classdef += ' };\n'
#		classdef += '#endif\n'		
		return classdef

	def writedataclasscode(self):
		#constructor
		classcode = ''
#		classcode += '#include "Data_' + self.name + '.h"\n'
#		for typ, des in self.datamember.iteritems():
#			if typ not in CLASS.TYPS:
#				classcode += '#include "Data_' + typ + '.h"\n'		
#
		classcode += 'BaseIO* Data_'+self.name+'::baseio = 0;\n'
		classcode += 'Data_'+self.name+'::Data_'+self.name + '(UInt_t size, std::string prefix = "") : \nsize_(size),\nprefix_(prefix)\n'
		for typ, des in self.datamember.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += ', ' +  mem + '_(new Data_'+typ+'(size_, prefix_ + "_'+mem+'"))\n'
		for typ, des in self.datavecs.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += ', ' +  mem[0] + '_(new Data_'+typ+'(size_*'+str(mem[1])+', prefix_ + "_'+mem[0]+'"))\n'
		classcode += '{\n'
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				ntyp = CLASS.TYPS[typ]
				for mem in des:
					classcode += '\t' + mem + '_ = new ' + ntyp + '[size_];\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				ntyp = CLASS.TYPS[typ]
				for mem in des:
					classcode += '\t' + mem[0] + '_num_ = new UInt_t[size_];\n'
					classcode += '\t' + mem[0] + '_ = new ' + ntyp + '[size_*'+str(mem[1])+'];\n'
					classcode += '\t' + mem[0]+'_countmax_ = size_*'+str(mem[1])+';\n'
		for typ, des in self.datavecs.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += '\t' + mem[0] + '_num_ = new UInt_t[size_];\n'
		classcode += '}\n\n'
		#destructor
		classcode += 'Data_'+self.name+'::~Data_'+self.name + '()\n'
		classcode += '{\n'
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				for mem in des:
					classcode += '\tdelete[] ' + mem + '_;\n'
			else:
				for mem in des:
					classcode += '\tdelete ' + mem + '_;\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				for mem in des:
					classcode += '\tdelete[] ' + mem[0] + '_;\n'
					classcode += '\tdelete[] ' + mem[0] + '_num_;\n'
			else:
				for mem in des:
					classcode += '\tdelete ' + mem[0] + '_;\n'
					classcode += '\tdelete[] ' + mem[0] + '_num_;\n'
		classcode += '}\n\n'
		#Fill
		classcode += 'void Data_' + self.name + '::Fill()\n'
		classcode += '{\n'
		classcode += '\tcount_ = 0;\n'
		for typ, des in self.datamember.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += '\t' + mem +'_->Fill();\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				for mem in des:
					classcode += '\t' + mem[0] + '_count_ = 0;\n'
		for typ, des in self.datavecs.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += '\t' + mem[0] +'_->Fill();\n'
		classcode += '}\n\n'
		#SetUp Tree Writing
		classcode += 'void Data_'+self.name+'::SetUpWrite(TTree* tree)\n'
		classcode += '{\n'
		classcode += '\ttree->Branch((prefix_ + "_count").c_str(), &count_, (prefix_ + "_count/i").c_str());\n'
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				for mem in des:
					classcode += '\ttree->Branch((prefix_ + "_' +mem+'").c_str(), ' + mem + '_, (prefix_ + "_' + mem + '[" + prefix_ + "_count]/'+typ+ '").c_str());\n'
		for typ, des in self.datamember.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += '\t' + mem +'_->SetUpWrite(tree);\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				for mem in des:
					classcode += '\ttree->Branch((prefix_ + "_' +mem[0]+'_count").c_str(), &' + mem[0] + '_count_, (prefix_ + "_' + mem[0] + '_count/i").c_str());\n'
					classcode += '\ttree->Branch((prefix_ + "_' +mem[0]+'_num").c_str(), ' + mem[0] + '_num_, (prefix_ + "_' + mem[0] + '_num[" + prefix_ + "_count]/i").c_str());\n'
					classcode += '\ttree->Branch((prefix_ + "_' +mem[0]+'").c_str(), ' + mem[0] + '_, (prefix_ + "_' + mem[0] + '[" + prefix_ + "_' +mem[0]+'_count]/'+typ+ '").c_str());\n'
		for typ, des in self.datavecs.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += '\ttree->Branch((prefix_ + "_' +mem[0]+'_num").c_str(), ' + mem[0] + '_num_, (prefix_ + "_' + mem[0] + '_num[" + prefix_ + "_count]/i").c_str());\n'
					classcode += '\t' + mem[0] +'_->SetUpWrite(tree);\n'
		classcode += '}\n\n'
		#SetUp Tree Reading
		classcode += 'void Data_'+self.name+'::SetUpRead(TTree* tree)\n'
		classcode += '{\n'
		classcode += '\ttree->SetBranchAddress((prefix_ + "_count").c_str(), &count_);\n'
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				for mem in des:
					classcode += '\ttree->SetBranchAddress((prefix_ + "_' +mem+'").c_str(), ' + mem + '_);\n'
		for typ, des in self.datamember.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += '\t' + mem +'_->SetUpRead(tree);\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				for mem in des:
					classcode += '\ttree->SetBranchAddress((prefix_ + "_' +mem[0]+'_count").c_str(), &' + mem[0] + '_count_);\n'
					classcode += '\ttree->SetBranchAddress((prefix_ + "_' +mem[0]+'_num").c_str(), ' + mem[0] + '_num_);\n'
					classcode += '\ttree->SetBranchAddress((prefix_ + "_' +mem[0]+'").c_str(), ' + mem[0] + '_);\n'
		for typ, des in self.datavecs.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += '\ttree->SetBranchAddress((prefix_ + "_' +mem[0]+'_num").c_str(), ' + mem[0] + '_num_);\n'
					classcode += '\t' + mem[0] +'_->SetUpRead(tree);\n'
		classcode += '}\n\n'
		#SetUp Load
		classcode += 'void Data_'+self.name+'::Load(TTree* tree, bool load)\n'
		classcode += '{\n'
		classcode += '\ttree->SetBranchStatus((prefix_ + "_count").c_str(), load);\n'
		for typ, des in self.datamember.iteritems():
			if typ in CLASS.TYPS:
				for mem in des:
					classcode += '\ttree->SetBranchStatus((prefix_ + "_' +mem+'").c_str(), load);\n'
		for typ, des in self.datamember.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += '\t' + mem +'_->Load(tree, load);\n'
		for typ, des in self.datavecs.iteritems():
			if typ in CLASS.TYPS:
				for mem in des:
					classcode += '\ttree->SetBranchStatus((prefix_ + "_' +mem[0]+'_count").c_str(), load);\n'
					classcode += '\ttree->SetBranchStatus((prefix_ + "_' +mem[0]+'_num").c_str(), load);\n'
					classcode += '\ttree->SetBranchStatus((prefix_ + "_' +mem[0]+'").c_str(), load);\n'
		for typ, des in self.datavecs.iteritems():
			if typ not in CLASS.TYPS:
				for mem in des:
					classcode += '\ttree->SetBranchStatus((prefix_ + "_' +mem[0]+'_num").c_str(), load);\n'
					classcode += '\t' + mem[0] +'_->Load(tree, load);\n'
		classcode += '}\n\n'


		return classcode


configfile = sys.argv[1]

headerfilename = sys.argv[2] + '.h'
sourcefilename = sys.argv[2] + '.cc'


classes = []
branches = {}

configf = open(configfile)

classname = ''
classconfig = []
for l in configf:
	l = l.split('#')[0]
	l = re.sub('\s+', ' ', l).strip()
	if len(l) == 0:
		continue
	if l.startswith('Branch'):
		l = l.split(':')
		branchinfo = getbranchinfo(l[1])
		cname = l[0].split('Branch')[1].strip()
		if cname in branches:
			branches[cname] += branchinfo
		else:
			branches[cname] = branchinfo
		continue

	if l.startswith('BeginClass'):
		classconfig = [l]
		l = l.split(':', 1)[1]
		l=l.strip()
		classname = l.split(' ')[0]
		continue
	if l.startswith('EndClass'):
		classes.append(CLASS(classname, classconfig))
		continue
	classconfig.append(l)

print branches

classdef = ''
classdef += '#ifndef CLASS_'+sys.argv[2]+'\n'	
classdef += '#define CLASS_'+sys.argv[2]+'\n'
classdef += '#include "TTree.h"\n'
classdef += '#include "TFile.h"\n'
classdef += '#include <string>\n'
classdef += '#include <iostream>\n'
classdef += 'using namespace std;\n'
classdef += 'namespace ' + sys.argv[2] + '{\n'
classdef += 'class BaseIO;\n'
for c in classes:
	classdef += 'class Data_' + c.name + ';\n'	
	classdef += 'class ' + c.name + ';\n'	

classcode = ''

classcode += 'namespace ' + sys.argv[2] + '{\n'
for c in classes:
	classdef += c.writedataclassdef()
	classdef += '\n\n'
	classcode += '//'+ ('Data_'+c.name + ' ')*10 + '\n'
	classcode += c.writedataclasscode()
	classcode += '\n\n'
	classdef += c.writeclassdef()
	classdef += '\n\n'
	classcode += '//'+ (c.name + ' == ')*15 + '\n'
	classcode += c.writeclasscode()
	classcode += '\n\n'

classdef += 'class BaseIO\n'	
classdef += '{\n'	
for c in classes:
	classdef += '\tfriend class ' + c.name + ';\n' 	
classdef += '\tprivate:\n'	
for n,L in branches.iteritems():
	for c in L:
		classdef += '\t\tData_' + n + ' ' + c[0] + '_container_;\n' 	
classdef += '\t\tbool writable_;\n'	
classdef += '\t\tInt_t error_[1000];\n'	
classdef += '\t\tUInt_t errorcount_;\n'	
classdef += '\t\tTTree* tree_;\n'	
classdef += '\t\tTTree* copytree_;\n'	
classdef += '\t\tstd::string treename_;\n'	
classdef += '\tpublic:\n'
classdef += '\t\tBaseIO(std::string treename, bool writable);\n'
classdef += '\t\t~BaseIO();\n'
classdef += '\t\tvoid SetFile(TFile* file);\n'
classdef += '\t\tbool IsWritable() const;\n'
classdef += '\t\tvoid Fill();\n'
classdef += '\t\tvoid SetError(Int_t errn);\n'
classdef += '\t\tUInt_t NumErrors();\n'
classdef += '\t\tvoid StartFilling();\n'
classdef += '\t\tUInt_t GetEntries();\n'
classdef += '\t\tvoid GetEntry(UInt_t n);\n'
for n,L in branches.iteritems():
	for c in L:
		classdef += '\t\tUInt_t Num' + c[0] + 's();\n' 	
		classdef += '\t\t'+n+' Get' + c[0] + '(UInt_t n);\n' 	
		classdef += '\t\tvoid Load' + c[0] + '(bool load);\n' 	
classdef += '};\n'	
classdef += '}\n'
classdef += '#endif\n'

classcode += 'BaseIO::BaseIO(std::string treename, bool writable) : \n'
for n,L in branches.iteritems():
	for c in L:
		classcode += c[0] + '_container_(' + str(c[1]) + ', "' + c[0] +'"),\n' 	
classcode += 'writable_(writable),\n'
classcode += 'errorcount_(0),\n'
classcode += 'tree_(0),\n'
classcode += 'copytree_(0),\n'
classcode += 'treename_(treename)\n'
classcode += '{\n'
for c in classes:
	classcode += '\t' + c.name +'::baseio = this;\n'
	classcode += '\tData_' + c.name +'::baseio = this;\n'
classcode += '}\n\n'
classcode += 'BaseIO::~BaseIO()\n'
classcode += '{\n'
classcode += '}\n\n'
classcode += 'void BaseIO::SetError(Int_t errn)\n'
classcode += '{\n'
classcode += '\terror_[errorcount_] = errn;\n'
classcode += '\terrorcount_++;\n'
classcode += '}\n\n'
classcode += 'UInt_t BaseIO::NumErrors()\n'
classcode += '{\n'
classcode += '\treturn(errorcount_);\n'
classcode += '}\n\n'
classcode += 'void BaseIO::SetFile(TFile* file)\n'
classcode += '{\n'
classcode += '\tif(writable_)\n'
classcode += '\t{\n'
classcode += '\t\tfile->cd();\n'
classcode += '\t\ttree_ = new TTree(treename_.c_str(), treename_.c_str(), 1);\n'
classcode += '\t\ttree_->Branch("ERROR_COUNT", &errorcount_, "ERROR_COUNT/i");\n'
classcode += '\t\ttree_->Branch("ERROR", error_, "ERROR[ERROR_COUNT]/I");\n'
for n,L in branches.iteritems():
	for c in L:
		classcode += '\t\t' + c[0] + '_container_.SetUpWrite(tree_);\n' 	
classcode += '\t}\n'
classcode += '\telse\n'
classcode += '\t{\n'
classcode += '\t\tfile->GetObject(treename_.c_str(), tree_);\n'
for n,L in branches.iteritems():
	for c in L:
		classcode += '\t\t' + c[0] + '_container_.SetUpRead(tree_);\n' 	
classcode += '\t}\n'
classcode += '}\n\n'
classcode += 'bool BaseIO::IsWritable() const {return writable_;}\n' 	
classcode += 'void BaseIO::Fill()' 
classcode += '{\n'
classcode += '\ttree_->Fill();\n'
classcode += '}\n'
classcode += 'void BaseIO::StartFilling()' 
classcode += '{\n'
for n,L in branches.iteritems():
	for c in L:
		classcode += '\t' + c[0] + '_container_.Fill();\n'
classcode += '}\n'
classcode += 'UInt_t BaseIO::GetEntries() {return tree_->GetEntries();}\n'
classcode += 'void BaseIO::GetEntry(UInt_t n) {tree_->GetEntry(n);}\n'
for n,L in branches.iteritems():
	for c in L:
		classcode += 'UInt_t BaseIO::Num' + c[0] + 's()\n' 	
		classcode += '{\n'
		classcode += '\treturn ' + c[0] + '_container_.count_;\n'
		classcode += '}\n'
		classcode += n+' BaseIO::Get' + c[0] + '(UInt_t n)\n' 	
		classcode += '{\n'
		classcode += '\treturn ' + n + '(&' + c[0] + '_container_, n);\n'
		classcode += '}\n'
		classcode += 'void BaseIO::Load' + c[0] + '(bool load)\n' 	
		classcode += '{\n'
		classcode += '\t' + c[0] + '_container_.Load(tree_, load);\n'
		classcode += '}\n\n'
classcode += '}\n'
		

headerfile = open(headerfilename, 'w')
headerfile.write(classdef)
headerfile.close()

sourcefile = open(sourcefilename, 'w')
sourcefile.write('#include "'+headerfilename+'"\n\n' + classcode)
sourcefile.close()

headerfile = open('../'+headerfilename, 'w')
headerfile.write(classdef)
headerfile.close()

sourcefile = open('../'+sourcefilename, 'w')
sourcefile.write('#include "../interface/'+headerfilename+'"\n\n' + classcode)
sourcefile.close()
print CLASS.ERRORNUM
