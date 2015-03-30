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
import rootpy
from fnmatch import fnmatch
rootpy.log["/"].setLevel(rootpy.log.INFO)
ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetOptStat(0)
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument('--noplots', dest='noplots', action='store_true',
                    help='skip plot making')
parser.add_argument('--noshapes', dest='noshapes', action='store_true',
                    help='skip shape making')
args = parser.parse_args()

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
            'tt, right cmb',
            '#6666b3'
            )
         }
      self.views['ttJets_rightHad'] = {
         'view' : self.create_tt_subsample(
            'semilep_right_thad', 
            'tt, right t_{h}',
            ),
         }
      self.views['ttJets_rightWHad'] = {
         'view' : self.create_tt_subsample(
            'semilep_right_whad', 
            'tt, right W_{h}',
            '#cccce6'
            )
         }
      self.views['ttJets_rightWLep'] = {
         'view' : self.create_tt_subsample(
            'semilep_right_tlep', 
            'tt, right t_{l}',
            '#aac0d5'
            )
         }
      self.views['ttJets_semiWrong'] = {
         'view' : self.create_tt_subsample(
            'semilep_wrong', 
            'tt, wrong cmb',
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

   @staticmethod
   def ttsub(inpath, subdir):
      outpath = inpath.replace('all', subdir, 1)
      logging.debug("%s --> %s" % (inpath, outpath))
      return outpath

   def create_tt_subsample(self, subdir, title, color='#9999CC'):
      return views.StyleView(
         views.TitleView(
            views.PathModifierView(
               self.views['ttJets_pu30']['view'],
               lambda x: self.ttsub(x, subdir)
               ),
            title
            ),
         fillcolor = color,
         linecolor = color
         )

   def write_mass_discriminant_shapes(self, tfolder, folder, rebin=1, preprocess=None):
      tfolder.cd()
      path = os.path.join(folder, 'mass_discriminant')
      ritghtW_view = views.SumView(
         self.get_view('ttJets_allRight'),
         self.get_view('ttJets_rightHad'),
         self.get_view('ttJets_rightWHad')
         )
      wrongW_view = views.SumView(
         self.get_view('ttJets_rightWLep'),
         self.get_view('ttJets_semiWrong')
         )
      other_view = self.get_view('ttJets_other')
      singlet_view = self.get_view('single*')

      if preprocess:
         ritghtW_view = preprocess(ritghtW_view)
         wrongW_view  = preprocess(wrongW_view )
         other_view   = preprocess(other_view  )
         singlet_view = preprocess(singlet_view)

      shape_views = [ritghtW_view, wrongW_view,
                     other_view, singlet_view]
      shape_names = ['right_whad', 'wrong_whad',
                     'nonsemi_tt', 'single_top']
      fake_data = ritghtW_view.Get(path).Clone()
      fake_data.Reset()
      fake_data.SetName('data_obs')
      fake_data.Write()

      for view, name in zip(shape_views, shape_names):
         histo = view.Get(path)
         if name == 'right_whad':
            histo.Scale(1./histo.Integral())
         histo.SetName(name)
         histo.Write()

   def plot_mc_shapes(self, folder, variable, rebin=1, xaxis='',
                      leftside=True, xrange=None, preprocess=None,
                      normalize=False, logy=False, filt=None, show_err=False):
      mc_views = None
      if not filt:
         mc_views = self.mc_views(rebin, preprocess, folder)
      else:
         mc_views = [views.SubdirectoryView(
               self.rebin_view(
                  self.get_view(i), 
                  rebin
                  ), 
               folder
               ) for i in filt]
         if preprocess:
            mc_views = [preprocess(i) for i in mc_views]

      histos = [i.Get(variable) for i in mc_views]

      if normalize:
         #set_trace()
         for i in histos:
            if i.Integral():
               i.Scale(1./i.Integral())
      m = 0.
      first = True
      for hist in histos:
         hist.fillstyle = 'hollow'
         hist.legendstyle = 'l'
         hist.linewidth = 2
         if show_err:
            hist.drawstyle = 'pe'
            hist.markerstyle = 20
            hist.legendstyle = 'pe'
            hist.markercolor = hist.linecolor

         hist.GetXaxis().SetTitle(xaxis)
         if first:
            hist.Draw()
            first = False
         else:
            hist.Draw('same')
         
         self.keep.append(hist)
         m = max(m,max(bin for bin in hist))
      histos[0].GetYaxis().SetRangeUser(0., m*1.2)
      if xrange:
         histos[0].GetXaxis().SetRangeUser(*tuple(xrange))
      self.add_legend(histos, leftside)
   
   def make_flavor_table(self, histo, fname):
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
      format = '%10s %10s %10s\n'

      with open(os.path.join(self.outputdir, fname), 'w') as f:
         total = histo.Integral()
         f.write('Total events: %.0f\n' % total)
         header = format % ('quarks', 'yield', 'relative')
         f.write(header)
         f.write('-'*len(header)+'\n')
         for idx, name in ids.iteritems():
            entries  = histo.GetBinContent(histo.FindBin(idx))
            entries += histo.GetBinContent(histo.FindBin(-idx))
            f.write(format % (name, '%.0f' % entries, '%.0f%%' % ((entries/total)*100)))
      
   def bake_pie(self, path):
      var = '%s/pflav_smart' % path
      mc_views = self.mc_views()
      mc_histos = [i.Get(var) for i in mc_views]
      integrals = [i.Integral() for i in mc_histos]
      total = sum(integrals)
      ratios = [i/total for i in integrals]
      names = [i.GetTitle() for i in mc_histos]
      colors = [i.GetFillColor('root') for i in mc_histos]
      #set_trace()
      format = '%20s %10s %10s\n'
      with open(os.path.join(self.outputdir, 'yields.raw_txt'), 'w') as f:
         f.write('Total events: %.0f\n' % total)
         header = format % ('process', 'yield', 'relative')
         f.write(header)
         f.write('-'*len(header)+'\n')
         for name, entries, ratio in zip(names, integrals, ratios):
            f.write(format % (name, '%.0f' % entries, '%.0f%%' % (ratio*100)))

      hsum = sum(mc_histos)
      self.make_flavor_table(hsum, 'flavors.raw_txt')

      right_cmb = self.get_view('ttJets_allRight').Get(var)
      self.make_flavor_table(right_cmb, 'flavors_rightcmb.raw_txt')

      wright_view = views.SumView(
         *[self.get_view(i) for i in ['ttJets_rightHad', 'ttJets_rightWHad']]
          )
      wright = wright_view.Get(var) + right_cmb
      self.make_flavor_table(wright, 'flavors_rightw.raw_txt')
   
plotter = BTagPlotter()

jet_variables = [
   ('pt' ,  10, '%s jet p_{T} (GeV)', None),
   ('eta',   1, '%s jet #eta', None),
   ('phi',  10, '%s jet #varphi', None),
   ('pflav_smart', 1, '%s jet parton flavour', [-7, 22]),
   ("ncharged", 1, "%s jet charged constituents", None),
   ("nneutral", 1, "%s jet neutral constituents", None),
   ("ntotal"  , 1, "%s jet total constituents", None),
]

variables = [
  ("njets"    , "# of selected jets", 1, [0, 12]),
  ("nbjets"   , "# of bjets", 1, [0, 12]),
  ("b_pt"     , "p_{T}(b) (GeV)", 10, None),
  ("bbar_pt"  , "p_{T}(b) (GeV)", 10, None),
  ("lep_pt"   , "p_{T}(l) (GeV)", 10, None),
  #("Wlep_mass", "m_{W}(lep) (GeV)", 10, None),
  ("Whad_mass", "m_{W}(had) (GeV)", 10, None),
  ("Whad_DR"  , "#DeltaR(jj) W_{had} (GeV)", 1, [0,7]),
  ("Whad_pt"  , "p_{T}(W_{had}) (GeV)", 10, None),
  ("Whad_leading_DR", "#DeltaR(W_{had}, leading jet)"   , 1, [0,7]),
  ("Whad_sublead_DR", "#DeltaR(W_{had}, subleading jet)", 1, [0,7]),
  ("thad_mass", "m_{t}(had) (GeV)", 10, None),
  ("nu_chisq"         , "nu_chisq"         , 1, [0, 20]),
	("nu_discriminant"	, "nu_discriminant"	 , 1, None),
	("btag_discriminant", "btag_discriminant", 1, [-11, -2]),
	("mass_discriminant", "mass_discriminant", 2, [1, 7]),
	("full_discriminant", "full_discriminant", 1, None),
]

shapes = set([
      'ncharged', 'nneutral', 'ntotal', 'mass_discriminant', 
      'full_discriminant', 'Whad_sublead_DR', 'Whad_pt', 
      'Whad_leading_DR', 'Whad_DR'
])

orders = [
   ## "nu_chisq"         , 
   ## "nu_discriminant"  , 
   ## "btag_discriminant", 
   "mass_discriminant", 
   "full_discriminant",
]

jet_categories = [
   "all", "lead_tagged", "sublead_tagged", 
   "both_tagged", "both_untagged"
]
jet_types = ['leading', 'subleading']

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
   'nu_chisq' : {'xrange' : [0, 20]},
}

if not args.noplots:
   for order in orders:
      for jet_cat in jet_categories:
        base = os.path.join('all', order, jet_cat)
        for jtype in jet_types:
           folder = os.path.join(base, jtype)
           plotter.set_subdir(os.path.join(order, jet_cat, jtype))
           for var, rebin, axis, x_range in jet_variables:
              plotter.plot_mc_vs_data(
                 folder, var, rebin, sort=True,
                 xaxis=axis % jtype, leftside=False, 
                 xrange=x_range)
              if 'pflav' in var:
                 for h in plotter.keep:
                    if isinstance(h, plotting.HistStack):
                       set_pdg_bins(h)
              plotter.save('_'.join((jtype,var)), pdf=False)
              if var in shapes:
                 plotter.plot_mc_shapes(
                    folder, var, rebin,
                    xaxis=axis % jtype, leftside=False,
                    xrange=x_range)
                 plotter.save('shape_'+'_'.join((jtype,var)), pdf=False)
           plotter.bake_pie(folder)

        plotter.set_subdir(os.path.join(order, jet_cat))
        for var, axis, rebin, x_range in variables:
           plotter.plot_mc_vs_data(
              base, var, rebin, sort=True,
              xaxis=axis, leftside=False,
              xrange=x_range)
           plotter.save(var, pdf=False)
           if var in shapes:
              if var == "mass_discriminant" and "_tagged" in jet_cat: rebin = 4
              plotter.plot_mc_shapes(
                 base, var, rebin,
                 xaxis=axis, leftside=False,
                 xrange=x_range)
              plotter.save('shape_%s' % var, pdf=False)
              if var <> "mass_discriminant": continue
              plotter.plot_mc_shapes(
                 base, var, rebin, normalize=True,
                 xaxis=order.replace('_', ' '), leftside=False,
                 xrange=x_range)
              plotter.save('normshape_%s' % order, pdf=False)

              plotter.plot_mc_shapes(
                 base, var, rebin, normalize=True, show_err=True,
                 xaxis=order.replace('_', ' '), leftside=False,
                 xrange=x_range, filt=['ttJets_allRight', 'ttJets_rightHad', 'ttJets_rightWHad'])
              plotter.save('normshape_rightwhad_%s' % order, pdf=False)

              plotter.plot_mc_shapes(
                 base, var, rebin, normalize=True, show_err=True,
                 xaxis=order.replace('_', ' '), leftside=False,
                 xrange=x_range, filt=['ttJets_rightWLep', 'ttJets_semiWrong'])
              plotter.save('normshape_wrongwhad_%s' % order, pdf=False)

      plotter.set_subdir("discriminants")
      ## plotter.plot_mc_vs_data(
      ##    'all/discriminators', order, 1, sort=True,
      ##    xaxis=order.replace('_', ' '), leftside=False,
      ##    **additional_opts.get(order,{}))
      ## plotter.save(order, pdf=False)
      plotter.plot_mc_shapes(
         'all/discriminators', order, 1,
         xaxis=order.replace('_', ' '), leftside=False,
         **additional_opts.get(order,{}))
      plotter.save('shape_%s' % order, pdf=False)

      plotter.plot_mc_shapes(
         'all/discriminators', order, 1, normalize=True,
         xaxis=order.replace('_', ' '), leftside=False,
         **additional_opts.get(order,{}))
      plotter.save('normshape_%s' % order, pdf=False)

if not args.noshapes:
   fname = os.path.join(plotter.base_out_dir, 'shapes.root')
   shape_file = ROOT.TFile(fname, 'RECREATE')
   plotter.write_mass_discriminant_shapes(
      shape_file.mkdir('notag'),
      os.path.join('all', 'mass_discriminant', 'both_untagged'), 
      rebin=2
   )
   plotter.write_mass_discriminant_shapes(
      shape_file.mkdir('leadtag'),
      os.path.join('all', 'mass_discriminant', 'lead_tagged'), 
      rebin=4
   )
   plotter.write_mass_discriminant_shapes(
      shape_file.mkdir('subtag'),
      os.path.join('all', 'mass_discriminant', 'sublead_tagged'), 
      rebin=4
      )
   shape_file.Close()
   logging.info("%s created" % fname)
