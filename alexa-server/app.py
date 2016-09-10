from flask import Flask, request, render_template
from flask_ask import Ask, statement, question, session
from unreal_socket import UnrealSocket
from random import randint
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
    params = request.json['params']
    # Step 4
    UnrealSocket.active_socket.process_command(command_name, params)

@ask.launch
def new_game():
    welcome_msg = render_template('welcome')
    return question(welcome_msg)


@ask.intent("YesIntent")
def next_round():
    numbers = [randint(0, 9) for _ in range(3)]
    round_msg = render_template('round', numbers=numbers)
    session.attributes['numbers'] = numbers[::-1]  # reverse
    return question(round_msg)


@ask.intent("AnswerIntent", convert={'first': int, 'second': int, 'third': int})
def answer(first, second, third):
    print 'seesion attributes: ',session.attributes
    winning_numbers = session.attributes['numbers']
    if [first, second, third] == winning_numbers:
        msg = render_template('win')
    else:
        msg = render_template('lose')
    return statement(msg)

if __name__ == '__main__':
    app.run(debug=True)