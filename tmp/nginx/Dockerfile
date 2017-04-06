## ocean_backdjango
FROM ubuntu:16.10


## PRE CONFIGURATION, optimization for docker-compose building
RUN mkdir /data
RUN mkdir /workdir


## INSTALL SECTION
# Updating image
RUN apt-get -qq update && apt-get -qqy upgrade

# Installing needed applications using package manager
RUN apt-get install -qqy \
  nginx \
  supervisor \
  wget \
  bash-completion


## AFTER CONFIGURATION
WORKDIR /workdir
