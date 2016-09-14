import requests
import threading
from json import dumps
from heroku_logger import p

class RequestType(object):

	Get = 1
	Post = 2
	Put = 3
	Delete = 4

class ThreadedRequest(object):

	def __init__(self, url, request_type=RequestType.Get, data={}, headers={}):
		self.url = url
		self.request_type = request_type
		self.data = data
		self.headers = headers
		self.thread = threading.Thread(target=self.run)
		self.thread.daemon = True
		self.response = None
		self.is_done = False
		self.thread.start()

	def run(self):
		if self.request_type == RequestType.Get:
			self.response = requests.get(self.url, data=self.data, headers=self.headers)
		elif self.request_type == RequestType.Post:
			self.headers['Content-type'] = 'application/json'
			self.response = requests.post(self.url, data=dumps(self.data), headers=self.headers)
			p(str(self.url))
		elif self.RequestType == RequestType.Put:
			self.response = requests.put(self.url, data=self.data, headers=self.headers)
		elif self.RequestType == RequestType.Delete:
			self.response = requests.delete(self.url, data=self.data, headers=self.headers)
		else:
			raise AttributeError('Invalid HTTP Request type')
		self.is_done = True
