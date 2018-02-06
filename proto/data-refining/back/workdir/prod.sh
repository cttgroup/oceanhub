#!/bin/bash

# /usr/local/bin/uwsgi --ini /workdir/app_uwsgi.ini
# NODE_ENV=production npm start
DJANGO_ENV=production ./manage.py runserver 0.0.0.0:8000

