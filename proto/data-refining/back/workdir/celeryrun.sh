#!/bin/bash

# celery -A app worker --loglevel=INFO --concurrency=2 -n worker1@%h
DJANGO_ENV=production celery -A app worker --loglevel=INFO --concurrency=2 -n worker1@%h
