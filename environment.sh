# Project-related environment variables 

#This part should be changed by the user(s)
export jobid=NOTSET
if [ -e jobid.sh ] 
then
		source jobid.sh
else
		echo "I did not find jobid.sh, are you sure you do not want to set the jobid and leave it to $jobid?"
fi
export URA_NTHREADS=1
#export URA_PROJECT_LIBS='-lwhatever_you_need_to_make_it_compile' #<-- Add here needed libraries for the project compilation

#HERE ARE LIONS!
#This part should be handled automatically by the scripts,
#touch it carefully
export URA_PROJECT=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
#source URA env
ura_dir=`dirname $URA_PROJECT`
source $ura_dir/URAnalysis/environment.sh
export PATH=$URA_PROJECT/bin:$PATH

#FNAL PATCH
if [[ $HOSTNAME == *"fnal.gov"* ]]
then
  LD_LIBRARY_PATH=/cvmfs/cms.cern.ch/$SCRAM_ARCH/external/boost/1.57.0/lib/:$LD_LIBRARY_PATH
fi
