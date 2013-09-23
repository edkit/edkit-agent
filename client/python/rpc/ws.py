import urllib2
import json
from . import rpc

class WebService(rpc.Rpc):
   def __init__(self, ip, port = 8080):
      super(WebService, self).__init__()
      self.url = 'http://' + str(ip) + ':' + str(port) + '/ws'

   def call(self, interface, method, parameters = None):
      """
      Calls a webservice API and returns its status and data.
      """
      if parameters == None:
         parameters = {}

      params =  {'InterfaceName' : interface,
                  'MethodName' : method,
                  'MethodParams' : parameters}

      request = urllib2.Request(self.url, json.dumps(params).encode('utf8'))
      response = urllib2.urlopen(request)
#      encoding = response.headers.get_content_charset()
      json_data = response.read() # .decode(encoding)
      data = json.loads(json_data)

      if data['status'] != 0:
         return data['status'], None
      return data['status'], data['data']


