FROM python:3.4


## PRE CONFIGURATION, optimization for docker-compose building
RUN mkdir /data
RUN mkdir /workdir


## INSTALL SECTION
# Manualy installing PyPy
WORKDIR /opt
COPY ./pypy-c-jit-90060-9f7edc16f16c-linux64.tar.bz2 /opt/.
RUN tar xf pypy-c-jit-90060-9f7edc16f16c-linux64.tar.bz2
RUN rm pypy-c-jit-90060-9f7edc16f16c-linux64.tar.bz2
RUN ln -s /opt/pypy-c-jit-90060-9f7edc16f16c-linux64/bin/pypy3 /usr/local/bin/pypy
RUN pypy -m ensurepip
RUN ln -s /opt/pypy-c-jit-90060-9f7edc16f16c-linux64/bin/pip3 /usr/local/bin/pypip

# Installing needed applications using package manager
RUN apt-get update && apt-get install -y \
  nginx \
  redis-server \
  supervisor \
  wget \
  bash-completion


# Installing modules needed by PyPy
ADD requirements-pypy.txt /tmp/
RUN pypip install --upgrade pip
RUN pypip install -r /tmp/requirements-pypy.txt


## CONFIG SECTION
# Pluging ngix config of your application into nginx
RUN ln -s /workdir/nginx-app.conf /etc/nginx/sites-enabled/
# Running Celery as root
RUN export C_FORCE_ROOT=1
# Pluging supervisor applications links
RUN ln -s /workdir/supervisor/app.conf /etc/supervisor/conf.d/.
RUN ln -s /workdir/supervisor/celery.conf /etc/supervisor/conf.d/.
RUN ln -s /workdir/supervisor/celerybeat.conf /etc/supervisor/conf.d/.
# Creating Celery logs files
RUN mkdir /var/log/celery
RUN touch /var/log/celery/app-celery.log
RUN touch /var/log/celery/app-celerybeat.log


## LAUNCHING SECTION
RUN supervisorctl start app-celery
RUN supervisorctl start app-celerybeat
RUN supervisord -c /etc/supervisor/supervisord.conf


## SUFFIX CONFIGURATION
WORKDIR /workdir
