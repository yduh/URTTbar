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
#import rootpy
#logging.getLogger('root').setLevel(logging.WARNING)
#set_trace()
#rootpy.log.setLevel(logging.INFO)

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
            'tt, right combination'
            )
         }
      self.views['ttJets_rightHad'] = {
         'view' : self.create_tt_subsample(
            'semilep_right_thad', 
            'tt, right had top',
            '#6666b3'
            ),
         }
      self.views['ttJets_rightWHad'] = {
         'view' : self.create_tt_subsample(
            'semilep_right_whad', 
            'tt, right had W',
            '#cccce6'
            )
         }
      self.views['ttJets_semiWrong'] = {
         'view' : self.create_tt_subsample(
            'semilep_wrong', 
            'tt, wrong combination',
            '#aac0d5'
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
         'WJets',
         'ZJets',
         'single*',
         'ttJets_allRight',
         'ttJets_rightHad',
         'ttJets_rightWHad',
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
   
plotter = BTagPlotter()

main = ['leading', 'subleading']
sub  = ["tagged", "all"]
jet_variables = [
   ('pt' ,  10, '%s jet p_{T}'),
   ('eta',   1, '%s jet #eta'),
   ('phi',  10, '%s jet #varphi'),
   ('pflav', 1, '%s jet parton flavour')
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
            if var == 'pflav':
               for h in plotter.keep:
                  if isinstance(h, plotting.HistStack):
                     set_pdg_bins(h)
            plotter.save('_'.join((m,s,var)), pdf=False)
   plotter.set_subdir(order)
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
