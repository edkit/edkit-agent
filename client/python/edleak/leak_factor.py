from numpy import *
from scipy import linalg

class LeakFactor(object):
#   def __init__(self, asset):
#      self.asset = asset

   def getAllocerFactor(self, allocer_data):
      """
      Computes the leak factor for the provided array. The leak factor is a dict
      with:
         - 'coef' : the coefficients of the 2nd degree polynom computed via regression
         - 'leak' : the amount of leaked memory computed from this polynom.
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

      coefs = self.getPolynomCoefs(data)
      leak = self.getLeakAmount(coefs, max-min)
      return { 'coef' : coefs, 'leak' : leak}

   def getPolynomCoefs(self, allocer_data):
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

   def getLeakAmount(self, polynom, original_max):
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


