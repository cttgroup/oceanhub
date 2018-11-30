FROM ubuntu:16.04


## PRE CONFIGURATION, optimization for docker-compose building
RUN mkdir /data
RUN mkdir /workdir


## INSTALL SECTION
# Updating image
RUN apt-get -qq update && apt-get -qqy upgrade

# Installing needed applications using package manager
RUN apt-get install -qqy \
  python3 \
  gdal-bin \
  libgdal-dev \
  libhdf5-dev \
  libnetcdf-dev \
  redis-server \
  supervisor \
  wget \
  bash-completion

RUN apt-get install -qqy \
  python3-pip

# Installing modules needed by CPython
ADD requirements.txt /tmp/
RUN pip3 install --upgrade pip
RUN pip3 install -r /tmp/requirements.txt


## AFTER CONFIGURATION
WORKDIR /workdir
