#!/bin/bash

# redis-cli -h backredis config set stop-writes-on-bgsave-error no
DJANGO_ENV=production celery -A app worker --loglevel=INFO --concurrency=10 -n worker2@%h
