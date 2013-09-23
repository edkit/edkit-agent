import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cm

class SlicePlotter(object):
   def __init__(self, slice_batch):
      self.slice_batch = slice_batch

   def sizePlot(self, scale='linear'):
      """
      Plots an edleak slice batch as a scatter plot with the size on the x axis.
      This is the default and recommended plot type for visual analysis.
      """
      slice_count = len(self.slice_batch)
      context_count = len(self.slice_batch[slice_count-1])

      x = np.zeros([context_count], int)
      y = np.r_[0:context_count]

      greens = plt.get_cmap('Greens')
      cnorm  = colors.Normalize(vmin=0, vmax=slice_count)
      scalar_map = cm.ScalarMappable(norm=cnorm, cmap=greens)

      ax = plt.subplot(111)
      slice_index = 0
      for current_slice in self.slice_batch:
         for alloc_point in current_slice:
            x[alloc_point['alc']] = alloc_point['mem']
         color_value = scalar_map.to_rgba(slice_index)
         ax.scatter(x, y, c=color_value, s=60, edgecolors='none')
         slice_index += 1

      if scale == 'log':
         ax.set_xscale('log')
      plt.show()
