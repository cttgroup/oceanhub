#!/bin/bash

git add . 
echo " "
DATE=`date +%Y-%m-%d:%H:%M`
git commit -a -m $DATE
echo " "
git push 
echo " "
echo "***************************************"
echo "*************** STATUS ****************"
echo "***************************************"
git status
echo " "

