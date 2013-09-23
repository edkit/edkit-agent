import time

class SliceRunner(object):
   def __init__(self, edleak_rpc):
      self.rpc = edleak_rpc

   def run(self, interval, length):
      slice_list = []
      while length > 0:
         status, data = self.rpc.getSlice()
         if status == 0:
            slice_list.append(data)
         time.sleep(interval)
         length -= interval

      return slice_list
