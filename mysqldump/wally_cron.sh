#!/bin/bash
	
cd /var/www/wally-dev.audaxis.com/
svn update
cd /var/www/wally-dev.audaxis.com/mysqldump/
mysql -u wally-dev -pWaVhfryR < mybackup.sql

# DLA - Exxodus 
# create database backup with - #mysqldump --add-drop-table --all-databases -u wally-dev -pwally-dev >mybackup.sql

