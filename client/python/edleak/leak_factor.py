from numpy import *
from scipy import linalg

leaker_class_constant   = 1
leaker_class_linear     = 2
leaker_class_log        = 3
leaker_class_exp        = 4

def get_leaker_classification(coefs, leak):
   """
      Returns the leaker classification, computed from the provided 2nd order
      polynom coefficients and the leak amout.
   """
   if leak <= 1:
      return leaker_class_constant
   if coefs[0] < -0.2:
      return leaker_class_log
   if coefs[0] > 0.2:
      return leaker_class_exp

   return leaker_class_linear

class LeakFactor(object):
#   def __init__(self, asset):
#      self.asset = asset

   def getAllocerFactor(self, allocer_data):
      """
      Computes the leak factor for the provided array. The leak factor is a dict
      with:
         - 'min_size' : Minimum allocated size for this allocer
         - 'max_size' : Maximum allocated size for this allocer
         - 'coef' : the coefficients of the 2nd degree polynom computed via regression
         - 'leak' : the amount of leaked memory computed from this polynom.
         - 'class' : The classification of the leak
      """

      if allocer_data == None:
         return None

      data = array(allocer_data, dtype=float)
      min = data.min()
      if min < 0:
         return None
      max = data.max()
      if max == min:
         max = min+1
      data -= min          # shift array to start at 0
      data /= max-min      # normalize to [0:1]

      coefs = self.__getPolynomCoefs(data)
      leak = self.__getLeakAmount(coefs, max-min)
      leaker_class = get_leaker_classification(coefs, leak)
      return { 'min_size': min, 'max_size': max,
               'coef' : coefs.tolist(),
               'leak' : leak,
               'class': leaker_class}

   def __getPolynomCoefs(self, allocer_data):
      """
      Does a 2nd order polynomial regression on the provided array and returns
      its coefficients as a list.
      """
      xi=r_[0.0:len(allocer_data)]
      xi /= len(allocer_data)    # normalize to [0:1]

      A = vander(xi, 3)
      c,resid,rank,sigma=linalg.lstsq(A,allocer_data)
#      print "polynom is ", c[0], " + ", c[1], "x + ", c[2], "x^2"
      return c

   def __getLeakAmount(self, polynom, original_max):
      """
      Computes the amount of leaked memory
      """
      f = poly1d(polynom)
      min = f(0)
      max = f(1)
#      print "min = ", min
#      print "max = ", max
#      print "leak amount is ", (max - min)*original_max
      return (int)((max - min)*original_max)


