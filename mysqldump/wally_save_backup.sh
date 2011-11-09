#!/bin/bash
        
cd /var/www/wally-demo/mysqldump/
#mysql -u wallydemo -pwallydemo < mybackup.sql
# DLA - Exxodus 
#create database backup with -mysqldump
mysqldump --add-drop-table --all-databases -u wallydemo -pwallydemo >mybackup.sql









