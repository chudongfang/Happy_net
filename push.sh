#!/bin/bash


#ls_date=`date +%Y-%m-%d`
ls_date='reactor_socket'
git pull 
git add .
git commit -m ${ls_date}
git push origin
ctags -R .
