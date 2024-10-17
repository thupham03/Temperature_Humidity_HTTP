from flask import Flask, request, jsonify
from pymongo import MongoClient
from datetime import datetime

client_db = MongoClient(
    "mongodb+srv://admin:Ua3rWeU0S3SUd214@temhum.ve2zq.mongodb.net/?retryWrites=true&w=majority&appName=temhum"
)
db = client_db['dht11']
collection = db['sensor']

app = Flask(__name__)

@app.route('/dht11', methods=['POST'])
def receive_dht11_data():
    try:
        data = request.get_json()
        temperature = data.get('temperature')
        humidity = data.get('humidity')

        if temperature is not None and humidity is not None:
            timestamp = datetime.now()

            print(f"Received Temperature: {temperature}C, Humidity: {humidity}% at {timestamp}")

            sensor_data = {
                'temperature': temperature,
                'humidity': humidity,
                'timestamp': timestamp
            }
            collection.insert_one(sensor_data)

            return jsonify({'status': 'success', 'message': 'Data received and stored'}), 200
        else:
            return jsonify({'status': 'error', 'message': 'Invalid data'}), 400
    except Exception as e:
        return jsonify({'status': 'error', 'message': str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
