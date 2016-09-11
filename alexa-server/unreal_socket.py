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

class UnrealSocket(object):

    threads = []
    active_socket = None

    def cleanup():
        if active_socket:
            active_socket.exit()

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.queued_commands = Queue()
        self.exit = False
        self.thread = threading.Thread(target=self.connect)
        UnrealSocket.active_socket = self
        self.thread.start()

    def connect(self):
        active_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        p("Socket Created.")
        active_socket.connect((self.host, self.port))
        p("Socket Connected")
        while 1:
            if self.exit:
                self.thread.exit(0)
            elif len(self.queued_commands) == 0:
                time.sleep(0.01)
            else:
                command = self.queued_commands.get(True)
                active_socket.send(command)
                p('Sent {0} to {1}:{2}'.format(command, self.host, self.port))

    def exit(self):
        self.exit = True

    def process_command(self, command):
        self.queued_commands.put('{0}\n'.format(str(command)), True)
        # Step 5 and 6 are done via rest call