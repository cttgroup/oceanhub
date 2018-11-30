#!/bin/bash

redis-cli -h backredis config set stop-writes-on-bgsave-error no
celery -A app worker --loglevel=INFO --concurrency=10 -n worker1@%h
