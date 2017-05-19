#!/bin/bash

COMMENT=$1
# COMMENT=$@

# echo $COMMENT
git add . && git commit -a -m $COMMENT && git push && git status

