from flask import Flask, request, jsonify
import multiprocessing
import subprocess
import requests
import json
import sys
import os


def write_query_file(request_id, query):
    query_points_cnt = 1 + len(query["points"])
    query_vehicles = query["vehicles"]
    query_capacity = query["capacity"]
    query_gen_size = query["generationSize"]
    query_gen_cnt = query["generationCount"]

    query_depot = (int(query["depot"]["x"]), int(query["depot"]["y"]))

    query_points = list(map(lambda point: (int(point["x"]), int(point["y"]), int(point["demand"])), query["points"]))

    with open(f'{request_id}.txt', "w") as w:
        w.write(f'Points: {query_points_cnt}\n')
        w.write(f'Capacity: {query_capacity}\n')
        w.write(f'Vehicles: {query_vehicles}\n')
        w.write(f'Generation size: {query_gen_size}\n')
        w.write(f'Generation count: {query_gen_cnt}\n')
        w.write(f'Depot: {query_depot[0]} {query_depot[1]}\n')
        for point in query_points:
            w.write(f'{point[0]} {point[1]} {point[2]}\n')


def run_vrp(request_id):
    p = subprocess.Popen(['./vrp', f'./{request_id}.txt', f'./{request_id}_sol.txt', './params.txt', 'pthread'], stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
    p.wait()

def read_vrp_result(request_id):
    raw_data = open(f'{request_id}_sol.txt', 'r').read().split('\n')
    result = {'cost': float(raw_data[-1].split()[1])}

    routes = []
    for raw_line in raw_data[:-1]:
        routes.append(list(map(lambda x: int(x) - 2, raw_line.split(': ')[1].split())))

    result.update({'routes': routes})
    
    return result

def delete_query_file(request_id):
    os.system(f"rm {request_id}.txt {request_id}_sol.txt")

def process_request(request_data, QUERY_SERVICE_ADDR, QUERY_SERVICE_PORT):
    request_id = request_data['id']
    query = request_data['query']

    # set the query as running
    requests.put(f'http://{QUERY_SERVICE_ADDR}:{QUERY_SERVICE_PORT}/set-running-query', json = {'id': request_id})

    write_query_file(request_id, query)
    run_vrp(request_id)
    query_result = read_vrp_result(request_id)
    delete_query_file(request_id)

    result = {'id': request_id, 'result': query_result}
    return result
