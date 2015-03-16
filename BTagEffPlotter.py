from pdb import set_trace
from URAnalysis.PlotTools.Plotter import Plotter
import os
import glob
from styles import styles
import sys
import logging
#import rootpy.plotting.views as views
import rootpy.plotting as plotting
views = plotting.views
from array import array
import ROOT
from pdb import set_trace

class BTagPlotter(Plotter):
   def __init__(self):
      jobid = os.environ['jobid']
      files = glob.glob('results/%s/btag_efficiency/*.root' % jobid)
      lumis = glob.glob('inputs/%s/*.lumi' % jobid)
      outdir= 'plots/%s/btageff' % jobid
      super(BTagPlotter, self).__init__(
         files, lumis, outdir, styles, None, 1000
         )
      self.jobid = jobid

      self.views['ttJets_allRight'] = {
         'view' : self.create_tt_subsample(
            'semilep_visible_right', 
            'tt, right combination',
            '#6666b3'
            )
         }
      self.views['ttJets_rightHad'] = {
         'view' : self.create_tt_subsample(
            'semilep_right_thad', 
            'tt, right had top',
            ),
         }
      self.views['ttJets_rightWHad'] = {
         'view' : self.create_tt_subsample(
            'semilep_right_whad', 
            'tt, right had W',
            '#cccce6'
            )
         }
      self.views['ttJets_rightWLep'] = {
         'view' : self.create_tt_subsample(
            'semilep_right_tlep', 
            'tt, right lep W',
            '#aac0d5'
            )
         }
      self.views['ttJets_semiWrong'] = {
         'view' : self.create_tt_subsample(
            'semilep_wrong', 
            'tt, wrong combination',
            '#88a7c4'
            )
         }
      self.views['ttJets_other'] = {
         'view' : self.create_tt_subsample(
            'other', 
            'Other tt decay',
            '#668db3',
            )
         }

      self.mc_samples = [
         '[WZ]Jets',
         'single*',
         'ttJets_allRight',
         'ttJets_rightHad',
         'ttJets_rightWHad',
         'ttJets_rightWLep',
         'ttJets_semiWrong',
         'ttJets_other'
         ]

   def create_tt_subsample(self, subdir, title, color='#9999CC'):
      return views.StyleView(
         views.TitleView(
            views.PathModifierView(
               self.views['ttJets_pu30']['view'],
               lambda x: x.replace('all', subdir, 1)
               ),
            title
            ),
         fillcolor = color,
         linecolor = color
         )

   def bake_pie(self, var):
      var = 'all/%s/leading/all/pflav_smart' % var
      mc_views = self.mc_views()
      mc_histos = [i.Get(var) for i in mc_views]
      integrals = [i.Integral() for i in mc_histos]
      total = sum(integrals)
      ratios = [i/total for i in integrals]
      names = [i.GetTitle() for i in mc_histos]
      colors = [i.GetFillColor('root') for i in mc_histos]
      #set_trace()
      format = '%30s %10s %10s\n'
      with open(os.path.join(self.outputdir, 'yields.raw_txt'), 'w') as f:
         f.write('Total events: %.0f\n' % total)
         header = format % ('process', 'yield', 'relative')
         f.write(header)
         f.write('-'*len(header)+'\n')
         for name, entries, ratio in zip(names, integrals, ratios):
            f.write(format % (name, '%.0f' % entries, '%.0f%%' % (ratio*100)))

      with open(os.path.join(self.outputdir, 'flavors.raw_txt'), 'w') as f:
         f.write('Total events: %.0f' % total)
         header = format % ('quanks', 'yield', 'relative')
         f.write(header)
         f.write('-'*len(header)+'\n')
         htotal = sum(mc_histos)
         ids = {
            0 : 'pile-up',
            1 : 'down',
            2 : 'up',
            3 : 'strange',
            4 : 'charm',
            5 : 'beauty',
            6 : 'top',
            21: 'gluon',
            }

         for idx, name in ids.iteritems():
            entries  = htotal.GetBinContent(htotal.FindBin(idx))
            entries += htotal.GetBinContent(htotal.FindBin(-idx))
            f.write(format % (name, '%.0f' % entries, '%.0f%%' % ((entries/total)*100)))
      ## pie = ROOT.TPie('pie','', len(ratios), array('d', ratios), array('i', colors))
      ## #pie.SetRadius(.15)
      ## self.pad.Divide(2)
      ## p1 = self.pad.cd(1)
      ## p1.Draw()
      ## self.keep.append(p1)
      ## for idx, _ in enumerate(ratios):
      ##    pie.SetEntryLabel(idx, '')
      ## self.keep.append(pie)
      ## pie.Draw('nol')
      ## 
      ## p2 = self.pad.cd(2)
      ## p2.Draw()
      ## self.keep.append(p2)
      ## for h, ratio in zip(mc_histos, ratios):
      ##    h.title += ' (%.0f%%)' % ratio
      ## self.add_legend(mc_histos, False)
      
   
plotter = BTagPlotter()

main = ['leading', 'subleading']
sub  = ["tagged", "all"]
jet_variables = [
   ('pt' ,  10, '%s jet p_{T}'),
   ('eta',   1, '%s jet #eta'),
   ('phi',  10, '%s jet #varphi'),
   ('pflav', 1, '%s jet parton flavour'),
   ('pflav_smart', 1, '%s jet parton flavour'),
]

variables = [
  ("njets"    , "# of selected jets", 1),
  ("nbjets"   , "# of bjets", 1),
  ("b_pt"     , "p_{T}(b) (GeV)", 10),
  ("bbar_pt"  , "p_{T}(b) (GeV)", 10),
  ("lep_pt"   , "p_{T}(#ell) (GeV)", 10),
  #("Wlep_mass", "m_{W}(lep) (GeV)", 10),
  ("Whad_mass", "m_{W}(had) (GeV)", 10),
  ("thad_mass", "m_{t}(had) (GeV)", 10),
  ("nu_chisq"         , "nu_chisq"         , 1),
	("nu_discriminant"	, "nu_discriminant"	 , 1),
	("btag_discriminant", "btag_discriminant", 1),
	("mass_discriminant", "mass_discriminant", 1),
	("full_discriminant", "full_discriminant", 1),
]

orders = [
   "nu_chisq"         , 
   "nu_discriminant"  , 
   "btag_discriminant", 
   "mass_discriminant", 
   "full_discriminant",
]

def set_pdg_bins(histo):
   ids = {
      1 : 'd', -1 : 'd',
      2 : 'u', -2 : 'u',
      3 : 's', -3 : 's',
      4 : 'c', -4 : 'c',
      5 : 'b', -5 : 'b',
      6 : 't', -6 : 't',
      21: 'g',
      }
   
   for idx, name in ids.iteritems():
      bin_idx = histo.GetXaxis().FindBin(idx)
      histo.GetXaxis().SetBinLabel(bin_idx, name)
   return histo

additional_opts = {
   'btag_discriminant' : {
      'xrange' : [-11, -2]
      },
   'pflav' : {
      'xrange' : [-7, 22],
      #'preprocess' : PDGIdView,
      },
   'nbjets' : { 'xrange' : [0, 12] },
   'njets' : { 'xrange' : [0, 12] },
   'nu_chisq' : {'xrange' : [0, 20]},
}

for order in orders:
   base = os.path.join('all', order)
   for m in main:
      for s in sub:    
         folder = os.path.join(base, m, s)
         plotter.set_subdir(os.path.join(order, m, s))
         for var, rebin, axis in jet_variables:
            plotter.plot_mc_vs_data(
               folder, var, rebin, sort=True,
               xaxis=axis % m, leftside=False, 
               **additional_opts.get(var,{}))
            if 'pflav' in var:
               for h in plotter.keep:
                  if isinstance(h, plotting.HistStack):
                     set_pdg_bins(h)
            plotter.save('_'.join((m,s,var)), pdf=False)
   plotter.set_subdir(order)
   
   plotter.bake_pie(order)
   #plotter.save('contributions', pdf=False)

   for var, axis, rebin in variables:
      plotter.plot_mc_vs_data(
         base, var, rebin, sort=True,
         xaxis=axis, leftside=False,
         **additional_opts.get(var,{}))
      plotter.save(var, pdf=False)

   plotter.set_subdir("discriminants")
   plotter.plot_mc_vs_data(
      'all/discriminators', order, 1, sort=True,
      xaxis=order.replace('_', ' '), leftside=False,
      **additional_opts.get(order,{}))
   plotter.save(order, pdf=False)
