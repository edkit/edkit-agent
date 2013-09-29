import cPickle

class Asset(object):
   def __init__(self):
      self.version = 1
      self.slice_list = []
      self.allocer_list = []

   @staticmethod
   def load(filename):
      """
      Loads the asset from a saved file.
      """
      file = open(filename, 'r')
      return cPickle.load(file)

   def save(self, filename):
      """
      Saves the asset to the specified file.
      """
      file = open(filename, 'w')
      cPickle.dump(self, file, 2)

   def setSliceList(self, slice_list):
      self.slice_list = slice_list

   def getSliceList(self):
      return self.slice_list

   def setAllocerList(self, allocer_list):
      self.allocer_list = allocer_list

   def getAllocerList(self):
      return self.allocer_list

