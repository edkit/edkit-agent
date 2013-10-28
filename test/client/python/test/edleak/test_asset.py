import unittest
import edleak.asset
import tempfile

allocer_list1 = [  {'id' : 0, 'stack' : [ 'caller1', 'caller2']},
              {'id' : 1, 'stack' : [ 'caller11', 'caller12']}]
slice_list1 = [ { 'mem' : 10, 'alc' : 0},
            { 'mem' : 15, 'alc' : 1}]

class TestCaseEdleakAsset(unittest.TestCase):

   def setUp(self):
      self.seq = range(10)

   def test_build(self):
      asset = edleak.asset.Asset()
      self.assertIsNotNone(asset)

   def test_allocer_list(self):
      asset = edleak.asset.Asset()
      asset.setAllocerList(allocer_list1)
      self.assertEqual(asset.getAllocerList(), allocer_list1)

   def test_slice_list(self):
      asset = edleak.asset.Asset()
      asset.setSliceList(slice_list1)
      self.assertEqual(asset.getSliceList(), slice_list1)

   def test_save(self):
      file, name = tempfile.mkstemp()
      asset = edleak.asset.Asset()
      asset.setAllocerList(allocer_list1)
      asset.setSliceList(slice_list1)

      asset.save(name)
      asset2 = edleak.asset.Asset.load(name)
      self.assertEqual(asset.getSliceList(), asset2.getSliceList())
      self.assertEqual(asset.getAllocerList(), asset2.getAllocerList())
