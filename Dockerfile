FROM debian:latest

VOLUME [ "/home/sachi" ]

WORKDIR /home

RUN apt-get upgrade -y && \
    apt-get update -y && \
    apt-get install -y build-essential libtool-bin automake autoconf git curl wget nano lcov

CMD [ "bash" ]
