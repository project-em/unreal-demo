import threading
import socket
from heroku_logger import p
import time
from Queue import Queue
# from alexa import Alexa

class UnrealCommand(object):
    QueryWorld = 0
    PressRed = 1
    PressBlue = 2
    PressGreen = 3
    PressYellow = 4
    QueryNumbers = 5

class UnrealSocket(object):

    threads = []
    active_socket = None
    has_credentials = False
    def cleanup():
        if active_socket:
            active_socket.exit()

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.queued_commands = Queue()
        self.exit = False
        UnrealSocket.has_credentials = True
        self.command_name = ''

    def connect(self):
        # active_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # p("Socket Created.")
        # active_socket.connect((self.host, self.port))
        # p("Socket Connected")
        # active_socket.send(command)

        # while 1:
        #     if self.exit:
        #         self.thread.exit(0)
        #     elif self.queued_commands.qsize() == 0:
        #         continue
        #     else:
        #         command = self.queued_commands.get(True)
        #         active_socket.send(command)
        #         p('Sent {0} to {1}:{2}'.format(command, self.host, self.port))
        return

    def exit(self):
        self.exit = True

    def process_command(self):
        command = self.command_name
        p("Socket Created.")
        active_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        active_socket.connect((self.host, self.port))
        p("Socket Connected")
        p('Queued command: {0}'.format(str(command)))
        active_socket.send(str(command))
        p('Send command: {0}'.format(str(command)))
        active_socket.close()
        # Step 5 and 6 are done via rest call
