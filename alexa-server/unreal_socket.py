import threading
import socket
from heroku_logger import p

class UnrealCommand(object):
    QueryWorld = 0
    PressRed = 1
    PressBlue = 2
    PressGreen = 3
    PressYellow = 4
    QueryNumbers = 5
    NumbersSuccess = 6

class UnrealSocket(object):

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.command_name = ''
        self.thread = None

    def send(self, command):
        thread = threading.Thread(target=self.execute_command, args=[command])
        thread.daemon = True
        thread.start()

    def execute_command(self, command):
        p("Socket Created.")
        active_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        active_socket.connect((self.host, self.port))
        active_socket.send(str(command))
        p('Send command: {0}'.format(str(command)))
        active_socket.close()