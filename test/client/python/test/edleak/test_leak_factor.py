import unittest
import edleak.leak_factor


class TestCaseEdleakLeakFactor(unittest.TestCase):

   def setUp(self):
      self.seq = range(10)

   def test_build(self):
      factor = edleak.leak_factor.LeakFactor()
      self.assertIsNotNone(factor)

   def test_get_allocer_factor(self):
      # this polynom is  -2.50626566416  +  257.042606516 x +  322.706766917 x^2
      allocer_data =[200, 400, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000,
                     3200, 3400, 3600, 3800, 4000, 4200]

      factor = edleak.leak_factor.LeakFactor()
      leak_factor = factor.getAllocerFactor(allocer_data)

      self.assertTrue(leak_factor['coef'][0] < -0.21)
      self.assertTrue(leak_factor['coef'][0] > -0.23)

      self.assertTrue(leak_factor['coef'][1] > 1.22)
      self.assertTrue(leak_factor['coef'][1] < 1.23)

      self.assertTrue(leak_factor['coef'][2] > 0.03)
      self.assertTrue(leak_factor['coef'][2] < 0.04)

      self.assertTrue(leak_factor['leak'] < 4100)
      self.assertTrue(leak_factor['leak'] > 3900)
   """
   def test_allocer_list(self):
      asset = edleak.asset.Asset()
      asset.setAllocerList(allocer_list1)
      self.assertEqual(asset.getAllocerList(), allocer_list1)
   """

