import cPickle
from . import leak_factor
from copy import deepcopy

class Asset(object):
   def __init__(self, from_file = None):
      '''
      Creates a new asset object. A previously saved asset file can be provided
      to initialize it from existing data.
      '''
      self.__version = 2
      self.__slice_list = None
      self.__allocer_list = None
      self.__pickle = { 'version' : self.__version}
      if from_file != None:
         file = open(from_file, 'r')
         asset_data = cPickle.load(file)
         self.setSliceList(asset_data['slice_list'])
         self.setAllocerList(asset_data['allocer_list'])

   def save(self, filename):
      """
      Saves the asset to the specified file.
      """
      file = open(filename, 'w')
      cPickle.dump(self.__pickle, file, 2)

   def setSliceList(self, slice_list):
      self.__slice_list = slice_list
      self.__pickle['slice_list'] = deepcopy(slice_list)
      self.__update()

   def getSliceList(self):
      return self.__slice_list

   def setAllocerList(self, allocer_list):
      '''
      Sets the allocer list, received from edleak api. This is a list of allocer
      dictionnaries with:
      - 'id' : id of the allocer.
      - 'stack' : list of call stack symbols.
      '''
      self.__allocer_list = allocer_list
      self.__pickle['allocer_list'] = deepcopy(allocer_list)
      self.__update()

   def getAllocerList(self):
      '''
      Retrieves the allocer list. This is a list of allocer dictionnaries with:
      - 'id' : id of the allocer.
      - 'stack' : list of call stack symbols.
      - 'coring' : Allocer memory coring.
      - 'leak_factor' : leak factor.

      '''
      return self.__allocer_list

   def __update(self):
      if self.__computeCoring() == True:
         self.__computeLeakFactor()

   def __computeCoring(self):
      '''
      Updates the allocers memory coring if possible. returns True if coring was
      updated, False otherwise
      '''
      if self.__slice_list == None or self.__allocer_list == None:
         return False

      for allocer in self.__allocer_list:
         allocer['coring'] = []

      for slice in self.__slice_list:
         for slice_entry in slice:
            self.__allocer_list[slice_entry['alc']]['coring'].append(slice_entry['mem'])

      return True

   def __computeLeakFactor(self):
      factor_runner = leak_factor.LeakFactor()
      for allocer in self.__allocer_list:
         allocer['leak_factor'] = factor_runner.getAllocerFactor(allocer['coring'])
      return

