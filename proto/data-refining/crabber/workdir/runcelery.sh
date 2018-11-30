#!/bin/bash

celery -A app worker --loglevel=INFO --concurrency=2 -n worker1@%h
