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

   def test_get_classification_constant(self):
      coefs = [0.0, 0.0, 0.1]
      leak = 0
      self.assertEqual(edleak.leak_factor.leaker_class_constant,
            edleak.leak_factor.get_leaker_classification(coefs, leak))
      leak = 1
      self.assertEqual(edleak.leak_factor.leaker_class_constant,
            edleak.leak_factor.get_leaker_classification(coefs, leak))

   def test_get_classification_log(self):
      coefs = [-0.21, 1.2, 0.0]
      leak = 12000
      self.assertEqual(edleak.leak_factor.leaker_class_log,
            edleak.leak_factor.get_leaker_classification(coefs, leak))
      coefs = [-0.92, 1.8, 1.0]
      self.assertEqual(edleak.leak_factor.leaker_class_log,
            edleak.leak_factor.get_leaker_classification(coefs, leak))

   def test_get_classification_exp(self):
      coefs = [0.21, 0.2, 0.0]
      leak = 12000
      self.assertEqual(edleak.leak_factor.leaker_class_exp,
            edleak.leak_factor.get_leaker_classification(coefs, leak))
      coefs = [0.92, 0.8, 1.0]
      self.assertEqual(edleak.leak_factor.leaker_class_exp,
            edleak.leak_factor.get_leaker_classification(coefs, leak))

   def test_get_classification_linear(self):
      coefs = [0.19, 1.2, 0.0]
      leak = 12000
      self.assertEqual(edleak.leak_factor.leaker_class_linear,
            edleak.leak_factor.get_leaker_classification(coefs, leak))
      coefs = [-0.19, 1.1, 1.0]
      self.assertEqual(edleak.leak_factor.leaker_class_linear,
            edleak.leak_factor.get_leaker_classification(coefs, leak))
      coefs = [0.01, 1.0, 1.0]
      self.assertEqual(edleak.leak_factor.leaker_class_linear,
            edleak.leak_factor.get_leaker_classification(coefs, leak))


   """
   def test_allocer_list(self):
      asset = edleak.asset.Asset()
      asset.setAllocerList(allocer_list1)
      self.assertEqual(asset.getAllocerList(), allocer_list1)
   """

