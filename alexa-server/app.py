from flask import Flask, request, render_template
from flask_ask import Ask, statement, question, session
from unreal_socket import UnrealSocket
from random import randint, choice
import sys
import logging

app = Flask(__name__)

ask = Ask(app, "/")

app.debug = True
logging.getLogger("flask_ask").setLevel(logging.DEBUG)

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
    # Step 4
    UnrealSocket.active_socket.process_command(command_name)

@ask.launch
def new_game():
    welcome_msg = render_template('welcome')
    return question(welcome_msg)

@ask.intent("PressButtonIntent", convert = {'color': str})
def press_button(color):
    button_msg = render_template('press', buttonMsg = color)
    return question(button_msg)

@ask.intent("QuitIntent")
def quit():
    return statement(render_template('quit'))

@ask.intent("QueryWorldIntent")
def query_world():
    return question(buildQueryList(getQueryList()))

def getQueryList():
    return ['red button', 'yellow button', 'purple door', 'trung', 'max', 'sacha', 'akshay']

def buildQueryList(query_list):
    query_str = ', '.join(['a ' + x for x in query_list][:-1]) + ', and a ' + query_list[-1]
    return render_template('locate', queryStr = query_str)

@ask.intent("LocationIntent")
def locate_surounding():
    return question(buildQueryList(getQueryList()))

@ask.intent("NameIntent")
def about_self():
    about_str = choice(
            ['You know who I am. I am Em. Stuck here in the world of despair, longing for you. Please come save me', 
            'What\'s wrong? Do you not think its me? IT IS I, Em' , 
            'What do you mean? I am your wife Em',
            'You tell me. What do you think?']
    )
    return question(render_template('about', aboutStr = about_str))

def shutdown_server():
    func = request.environ.get('werkzeug.server.shutdown')
    if func is None:
        raise RuntimeError('Not running with the Werkzeug Server')
    func()

@app.route('/shutdown', methods=['POST'])
def shutdown():
    shutdown_server()
    UnrealSocket.cleanup()
    return 'Server shutting down...'

if __name__ == '__main__':
    app.run(debug=True)