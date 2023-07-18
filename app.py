from flask import Flask, request, jsonify
import multiprocessing
import json
import requests
import sys
import subprocess
import os
import helpers

required_env_vars = ['QUERY_SERVICE_ADDR', 'QUERY_SERVICE_PORT', 'POOL_SIZE']

for var in required_env_vars:
    if var not in os.environ:
        raise Exception(f"Required environment variable {var} is not set")

QUERY_SERVICE_ADDR = os.environ['QUERY_SERVICE_ADDR']
QUERY_SERVICE_PORT = os.environ['QUERY_SERVICE_PORT']
POOL_SIZE = int(os.environ['POOL_SIZE'])

app = Flask(__name__)
pool = multiprocessing.Pool(POOL_SIZE)


def handle_result(result):
    requests.put(f'http://{QUERY_SERVICE_ADDR}:{QUERY_SERVICE_PORT}/update-query', json = result)


@app.route('/', methods=['POST'])
def receive_request():
    if not "id" in request.json.keys() or not "query" in request.json.keys():
        return jsonify({'message': 'JSON with "id" and "query" needed'}), 400

    request_data = request.get_json()
    pool.apply_async(helpers.process_request, (request_data, QUERY_SERVICE_ADDR, QUERY_SERVICE_PORT), callback=handle_result)
    return jsonify({'message': 'Query successfully added to queue'}), 200


if __name__ == '__main__':
    app.run(debug=True)
