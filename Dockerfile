FROM ubuntu:20.04

COPY requirements.txt .

RUN apt update -y
RUN apt upgrade -y
RUN apt install build-essential gcc g++ python3 python3-pip -y
RUN pip install --trusted-host pypi.python.org -r requirements.txt

WORKDIR /app
COPY . /app


RUN make


CMD flask run --host 0.0.0.0 --debug
