import os
import time
from flask import Flask, request, jsonify

app = Flask(__name__)
last_data = {}
db = open('sensor_db.csv', 'a+')

@app.route('/')
def index():
    return 'Hello World!'

@app.route('/report', methods=['POST'])
def report():
    global last_data
    data = request.get_json()
    print(data)
    if 'time' in data:
        local_time = time.localtime(data['time'])
        time_str = time.strftime('%Y/%m/%d %H:%M:%S', local_time)
        data['local_time'] = time_str
        last_data = data
        db.write(f'{time_str},{data["time"]},{data["temperature"]},{data["humidity"]}\n')
        db.flush()
        return 'OK'
    else:
        return 'ERROR'

@app.route('/query', methods=['GET'])
def query():
    return jsonify(last_data)

if __name__ == '__main__':
    app.run(port=11111, host='127.0.0.1', debug=False)
