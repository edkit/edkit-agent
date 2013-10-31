import cPickle

class Asset(object):
   def __init__(self):
      self.version = 1
      self.slice_list = None
      self.allocer_list = None

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
      self.__computeCoring()

   def getSliceList(self):
      return self.slice_list

   def setAllocerList(self, allocer_list):
      self.allocer_list = allocer_list
      self.__computeCoring()

   def getAllocerList(self):
      return self.allocer_list

   def __computeCoring(self):
      if self.slice_list == None or self.allocer_list == None:
         return

      for allocer in self.allocer_list:
         allocer['coring'] = []

      for slice in self.slice_list:
         for slice_entry in slice:
            self.allocer_list[slice_entry['alc']]['coring'].append(slice_entry['mem'])

