import cPickle
from . import leak_factor

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
      self.__update()

   def getSliceList(self):
      return self.slice_list

   def setAllocerList(self, allocer_list):
      self.allocer_list = allocer_list
      self.__update()

   def getAllocerList(self):
      '''
      Retrieves the allocer list. This is a list of allocer dictionnaries with:
      - 'id' : id of the allocer.
      - 'stack' : list of call stack symbols.
      - 'coring' : Allocer memory coring.
      - 'leak_factor' : leak factor.

      '''
      return self.allocer_list

   def __update(self):
      if self.__computeCoring() == True:
         self.__computeLeakFactor()

   def __computeCoring(self):
      '''
      Updates the allocers memory coring if possible. returns True if coring was
      updated, False otherwise
      '''
      if self.slice_list == None or self.allocer_list == None:
         return False

      for allocer in self.allocer_list:
         allocer['coring'] = []

      for slice in self.slice_list:
         for slice_entry in slice:
            self.allocer_list[slice_entry['alc']]['coring'].append(slice_entry['mem'])

      return True

   def __computeLeakFactor(self):
      factor_runner = leak_factor.LeakFactor()
      for allocer in self.allocer_list:
         allocer['leak_factor'] = factor_runner.getAllocerFactor(allocer['coring'])
      return

