## ocean_backextras
FROM ubuntu:16.04


## PRE CONFIGURATION, optimization for docker-compose building
RUN mkdir /data
RUN mkdir /workdir

## INSTALL SECTION
# Updating image
RUN apt-get -qq update && apt-get -qqy upgrade

RUN apt-get install -qqy \
  g++ \
  gdal-bin \
  libgdal-dev \
  wget \
  bash-completion


## SUFFIX CONFIGURATION
WORKDIR /workdir
