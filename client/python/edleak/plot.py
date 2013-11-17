import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cm

from . import leak_factor

class SlicePlotter(object):
   def __init__(self, asset):
      self.asset = asset
      self.slice_batch = asset.getSliceList()

   def sizePlot(self, scale='linear'):
      """
      Plots an edleak slice batch as a scatter plot with the size on the x axis.
      This is the default and recommended plot type for visual analysis.
      """
      allocers = self.asset.getAllocerList()

      slice_count = len(allocers[0]['coring'])
      allocer_count = len(allocers)
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

   def timePlot(self, scale='linear', allocers=None):
      """
      Plots an edleak slice batch as a scatter plot with the time on the x axis.
      """
      slice_count = len(self.slice_batch)
      context_count = len(self.slice_batch[slice_count-1])

      x = np.r_[0:slice_count]
      y = np.zeros((context_count, slice_count), int)

      greens = plt.get_cmap('Greens')
      cnorm  = colors.Normalize(vmin=0, vmax=context_count)
      scalar_map = cm.ScalarMappable(norm=cnorm, cmap=greens)

      ax = plt.subplot(111)
      slice_index = 0
      for current_slice in self.slice_batch:
         for alloc_point in current_slice:
            y[alloc_point['alc']][slice_index] = alloc_point['mem']
         slice_index += 1

      context_index = 0
      while context_index < context_count:
         if allocers == None or context_index in allocers:
            color_value = scalar_map.to_rgba(context_index)
            ax.scatter(x, y[context_index], c=color_value, s=60, edgecolors='none')
         context_index += 1

      if scale == 'log':
         ax.set_yscale('log')
      plt.show()
