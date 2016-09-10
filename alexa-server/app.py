from flask import Flask, request
from unreal_socket import UnrealSocket
import sys

app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, World!'

def p(*args):
  print args[0] % (len(args) > 1 and args[1:] or [])
  sys.stdout.flush()

# Step 1
@app.route('/register', methods=['POST'])
def register_client():
    host = request.json['host']
    port = request.json['port']
    UnrealSocket(host, port)
    return 'ok'
    
# Step 3
@app.route('/alexa', methods=['POST'])
def execute_command():
    command_name = request.json['command']
    params = request.json['params']
    # Step 4
    UnrealSocket.active_socket.process_command(command_name, params)