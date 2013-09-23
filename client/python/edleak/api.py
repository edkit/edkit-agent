from rpc import ws

class EdLeak(object):
   def __init__(self, rpc):
      self.rpc = rpc

   def getSlice(self):
      """
      Returns the call status and a memory slice as a list of dictionnary. Each
      dictionnay contains 2 keys:
      - alc : The allocer id.
      - mem : The current memory size in byte allocated by the allocer.
      """
      status, data = self.rpc.call('Edleak', 'GetSlice')
      if status != 0:
         return status, None
      return status, data['slice'][0]

   def getAllocers(self):
      """
      Returns the list of allocers.
      """
      status, data = self.rpc.call('Edleak', 'GetSlice')
      if status != 0:
         return status, None
      return status, data['allocer']

   def addStackWatch(self, allocer_id):
      """
      Adds an allocer id to the list of allocations points where a call stack is
      dumped.
      """
      status, data = self.rpc.call('Edleak', 'AddStackWatch', {'id': allocer_id})
      if status != 0:
         return status, None
      return status


