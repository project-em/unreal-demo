import threading
import socket
# from alexa import Alexa

class UnrealCommand(object):

    Pickup = 1

    def translate_route(api_route):
        if api_route == 'pickup':
            return Pickup
        elif api_route == 'foo':
            return Pickup
        else:
            return -1

class UnrealSocket(object):

    threads = []
    active_socket = None

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.thread = threading.Thread(target=self.connect)
        threads.append(self.thread)
        self.thread.start()

    def connect(self):
        active_socket = socket.socket(AF_INET, SOCK_STREAM)
        active_socket.connect(self.host, self.port)
    
    def process_command(self, api_route, params=[]):
        params.append('[end]')
        active_socket.send(UnrealCommand.translate_route(api_route), params.join(' '))
        # Step 5
        (command, params) = parse_command(self.recv(1024))
        # Step 6
        # Alexa.execute(command, params)

    def parse_command(return_date):
        split_data = return_date.split(' ')
        command = return_date[0]
        params = return_date[0:-1]