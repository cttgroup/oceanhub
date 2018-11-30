FROM centos:7.3.1611


## PRE CONFIGURATION, optimization for docker-compose building
RUN mkdir /data
RUN mkdir /workdir


## INSTALL SECTION
# Updating image
RUN yum update -y && yum clean all

# Install basic packages
RUN yum install -y wget vim bash-completion lbzip2 bzip2 gcc make \
  gcc-gfortran gcc-c++ unzip cmake

# Pluggin EPEL repository
RUN yum install -y epel-release.noarch

# Install python 3.4
RUN yum install -y python34-setuptools
RUN easy_install-3.4 pip

# Install other packages
RUN yum install -y \
  supervisor

# Installing modules needed by CPython
COPY requirements.txt /tmp/
RUN pip install --upgrade pip
RUN pip install -r /tmp/requirements.txt


## CONFIG SECTION
# Running Celery as root
RUN export C_FORCE_ROOT=1
# Pluging supervisor applications links
RUN ln -s /workdir/supervisor/app.conf /etc/supervisord.d/.
RUN ln -s /workdir/supervisor/celery.conf /etc/supervisord.d/.
RUN ln -s /workdir/supervisor/celerybeat.conf /etc/supervisord.d/.
# Creating Celery logs files
RUN mkdir /var/log/celery
RUN touch /var/log/celery/app-celery.log
RUN touch /var/log/celery/app-celerybeat.log


## LAUNCHING SECTION
RUN supervisorctl start app-celery
RUN supervisorctl start app-celerybeat
RUN supervisord -c /etc/supervisor/supervisord.conf



## AFTER CONFIGURATION
WORKDIR /workdir
