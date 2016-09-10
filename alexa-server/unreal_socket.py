import threading
import socket
import heroku_logger
# from alexa import Alexa

class UnrealCommand(object):
    QueryWorld = 0
    PressRed = 1
    PressBlue = 2
    PressGreen = 3
    PressYellow = 4

class UnrealSocket(object):

    threads = []
    active_socket = None

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.thread = threading.Thread(target=self.connect)
        UnrealSocket.threads.append(self.thread)
        self.thread.start()

    def connect(self):
        active_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        active_socket.connect((self.host, self.port))
    
    def process_command(self, command):
        active_socket.send('{0}\n'.format(command))
        # Step 5 and 6 are done via rest call