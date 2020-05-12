FROM debian:latest

VOLUME [ "/home/sachi" ]

WORKDIR /home

RUN apt-get upgrade -y && \
    apt-get update -y && \
    apt-get install -y build-essential automake autoconf git curl wget

CMD [ "bash" ]
