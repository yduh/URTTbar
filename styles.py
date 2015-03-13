# Histogram styles:
# put in styles the stlye you want to apply to each sample you are processing
# in format key : {style_dict}, according to rootpy StyleView https://github.com/rootpy/rootpy/blob/0.7.1/rootpy/plotting/views.py
# prescriptions, any key named title will be strupped off for you and applied in a
# separate TitleView
#
# The keys are allowed to contain POSIX-style regular expressions, if multiple
# matches are found the longest key is used.

styles = {
   'ZJets*' : {
      'legendstyle' : 'f',
      'drawstyle' : 'hist',
      'fillcolor' : '#FFCC66',
      'linecolor' : '#FFCC66',
      'name' : "Z + jets",
      'fillstyle': 'solid',
      },
   'WJets*' : {
      'legendstyle' : 'f',
      'drawstyle' : 'hist',
      'fillcolor' : '#990000',
      'name' : "W + jets",
      'fillstyle': 'solid',
      },
   'single*' : {
      'legendstyle' : 'f',
      'drawstyle' : 'hist',
      'fillcolor' : '#2aa198',
      'linecolor' : '#2aa198',
      'name' : "single top",
      'fillstyle': 'solid',
      },
   'data*' : {
      'legendstyle' : 'pe',
      'drawstyle' : 'pe',
      'markerstyle' : 20,
      #'markersize'  : 2,
      'name' : "Observed",
    },
   'tt*' : {
      'legendstyle' : 'f',
      'drawstyle' : 'hist',
      'fillcolor' : '#9999CC',
      'linecolor' : '#9999CC',
      'name' : "ttbar",
      'fillstyle': 'solid',
      },
   }
