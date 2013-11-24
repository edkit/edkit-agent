import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cm

from . import leak_factor

class SlicePlotter(object):
   def __init__(self, asset):
      self.asset = asset
      self.slice_batch = asset.getSliceList()

   def sizePlot(self, scale='log', allocers=None):
      """
      Plots an edleak slice batch as a scatter plot with the size on the x axis.
      This is the default and recommended plot type for visual analysis.
      """
      if allocers == None:
         allocers = self.asset.getAllocerList()

      colormaps = {}
      colormaps[leak_factor.leaker_class_constant] = plt.get_cmap('Greens')
      colormaps[leak_factor.leaker_class_linear] = plt.get_cmap('Reds')
      colormaps[leak_factor.leaker_class_log] = plt.get_cmap('Blues')
      colormaps[leak_factor.leaker_class_exp] = plt.get_cmap('binary')

      ax = plt.subplot(111)

      for allocer in allocers:
         coring = allocer['coring']
         x = np.zeros([len(coring)], int)
         y = np.empty(len(coring))
         y.fill(allocer['id'])

         colors = np.r_[0:len(coring)]

         slice_index = 0
         for alloc_point in coring:
            x[slice_index] = alloc_point
            slice_index += 1

         ax.scatter(x, y, c=colors, s=60, edgecolors='none',
               cmap=colormaps[allocer['leak_factor']['class']])

      if scale == 'log':
         ax.set_xscale('log')
      plt.show()

   def timePlot(self, scale='log', allocers=None):
      """
      Plots an edleak slice batch as a scatter plot with the time on the x axis.
      """
      if allocers == None:
         allocers = self.asset.getAllocerList()

      cnorm  = colors.Normalize(vmin=0, vmax=len(allocers))
      colormaps = {}
      colormaps[leak_factor.leaker_class_constant] = cm.ScalarMappable(norm=cnorm, cmap=plt.get_cmap('Greens'))
      colormaps[leak_factor.leaker_class_linear] = cm.ScalarMappable(norm=cnorm, cmap=plt.get_cmap('Reds'))
      colormaps[leak_factor.leaker_class_log] = cm.ScalarMappable(norm=cnorm, cmap=plt.get_cmap('Blues'))
      colormaps[leak_factor.leaker_class_exp] = cm.ScalarMappable(norm=cnorm, cmap=plt.get_cmap('binary'))

      ax = plt.subplot(111)

      allocer_index = 0
      for allocer in allocers:
         coring = allocer['coring']
         x = np.r_[0:len(coring)]
         y = np.empty(len(coring))


         slice_index = 0
         for alloc_point in coring:
            y[slice_index] = alloc_point
            slice_index += 1

         color_value = colormaps[allocer['leak_factor']['class']].to_rgba(allocer_index)
         ax.scatter(x, y, c=color_value, s=60, edgecolors='none')
         allocer_index += 1

      if scale == 'log':
         ax.set_yscale('log')
      plt.show()

