#! /bin/sh

if [ -z $1 ] ; then
    echo "No executable name given!"
    exit
fi

while read l ; do
    
    HOST=$(echo $l | cut -f 1 -d " ")
    HOST_USER=$(echo $l | cut -f 4 -d " ")

    echo "kill on $HOST with $HOST_USER"
    ssh -f $HOST_USER@$HOST "killall $1"
    

done < hosts
