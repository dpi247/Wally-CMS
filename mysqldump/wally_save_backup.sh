#!/bin/bash

$OUTUPUT_FOLDER=/tmp
if [ $4 ]
  then
    $OUTUPUT_FOLDER="$4"
fi

cd "$OUTUPUT_FOLDER"
#mysql -u wallydemo -pwallydemo < mybackup.sql
# DLA - Exxodus 
#create database backup with -mysqldump
mysqldump --add-drop-table  -u $1 -p$2 $3>mybackup.sql
