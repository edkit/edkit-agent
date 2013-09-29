import time
from . import asset

class SliceRunner(object):
   def __init__(self, edleak_rpc):
      self.rpc = edleak_rpc

   def run(self, interval, length):
      """
      Captures some slices each interval seconds during length seconds. Returns
      an edleak asset object.
      """
      slice_run = asset.Asset()
      slice_list = []
      while length > 0:
         status, data = self.rpc.getSlice()
         if status == 0:
            slice_list.append(data)
         time.sleep(interval)
         length -= interval

      status, allocer_list = self.rpc.getAllocers()
      if status == 0:
         slice_run.setSliceList(slice_list)
         slice_run.setAllocerList(allocer_list)
      else:
         slice_run = None
      return slice_run
