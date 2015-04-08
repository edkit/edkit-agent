import sys
import rpc.ws
import edleak.api
import edleak.slice_runner

def usage():
   print('autodetect [period] [duration]')

def print_leaker(leaker):
   print('-------------------------------')
   print('class     : ' + leaker['leak_factor']['class'])
   print('leak size : ' + str(leaker['leak_factor']['leak']))
   print('call-stack: ')
   for caller in leaker['stack']:
      print('    ' + caller)

if __name__ == '__main__':
   if len(sys.argv) != 3:
       usage()
       sys.exit(-1)

   period = int(sys.argv[1])
   duration = int(sys.argv[2])
   ws_rpc = rpc.ws.WebService("localhost", 8080)
   el = edleak.api.EdLeak(ws_rpc)
   runner = edleak.slice_runner.SliceRunner(el)

   # First run, to find the leakers
   print('Starting 1st run...')
   asset = runner.run(period, duration)
   allocers = asset.getAllocerList()
   leakers = [l for l in allocers if l['leak_factor']['leak'] > 0 and
      (l['leak_factor']['class'] == 'linear' or
       l['leak_factor']['class'] == 'exp')]

   if len(leakers) == 0:
      print('No leaks found.')
      sys.exit(0)

   print(str(len(leakers)) + ' leaks found. Starting 2nd run to retrieve callstacks...')
   for leaker in leakers:
      el.addStackWatch(leaker['id'])

   asset = runner.run(period, duration)
   allocers = asset.getAllocerList()
   leakers = [l for l in allocers if l['leak_factor']['leak'] > 0 and
      (l['leak_factor']['class'] == 'linear' or
       l['leak_factor']['class'] == 'exp')]


   for leaker in leakers:
      if len(leaker['stack']) > 1:
         print_leaker(leaker)
