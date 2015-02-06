# Project-related environment variables 

#This part should be changed by the user(s)
export jobid="Some tag for the Ntuples"
#export URA_PROJECT_LIBS='-lwhatever_you_need_to_make_it_compile' #<-- Add here needed libraries for the project compilation


#HERE ARE LIONS!
#This part should be handled automatically by the scripts,
#touch it carefully
export URA_PROJECT=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
#source URA env
ura_dir=`dirname $URA_PROJECT`
source $ura_dir/URAnalysis/environment.sh
export PATH=$URA_PROJECT/bin:$PATH
