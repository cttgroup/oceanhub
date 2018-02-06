#!/bin/bash

# set -e

# pg_dump -U postgres port > /backup/port_init.20161206
psql -U postgres -f ./backup/port_init postgres

# psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" <<-EOSQL
    # CREATE USER django;
    # CREATE DATABASE port;
    # GRANT ALL PRIVILEGES ON DATABASE port TO django;
# EOSQL
