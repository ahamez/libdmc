#! /bin/sh

mkdir -p logs

if [ "$1" = "rm" ] ; then
    rm=1
fi

while read l ; do
    
    HOST=$(echo $l | cut -f 1 -d " ")
    HOST_USER=$(echo $l | cut -f 4 -d " ")

    echo "retreive on $HOST with $HOST_USER"
    if [ -z $rm ] ; then
	scp $HOST_USER@$HOST:/data/dmc*/*.log  logs/
	scp $HOST_USER@$HOST:/data/dmc*/core*  logs/
    else
	ssh -f $HOST_USER@$HOST "rm -rf /data/dmc*"
    fi

done < hosts
