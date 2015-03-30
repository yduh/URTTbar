$project_dir = ENV['URA_PROJECT']
$fwk_dir = ENV['URA']+'/AnalysisFW'

tools = "#{$fwk_dir}/rake/tools.rb"
require tools

analysis_rules = "#{$fwk_dir}/rake/analysis.rake"
import analysis_rules

meta_rules = "#{$fwk_dir}/rake/meta.rake"
import meta_rules

task :local_build => make_libs($project_dir, $fwk_dir)
task :build => make_libs($fwk_dir)

task :testBtag => ['bin/btag_efficiency.exe'] do |t|
  sh 'time btag_efficiency.exe test.txt ttJets_pu30.root -c  btag_efficiency.cfg --threads 1 -v '
end

