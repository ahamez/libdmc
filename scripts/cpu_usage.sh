#! /bin/sh

while getopts "lf:" ARG ; do
    
    case $ARG in
	l)
	    LAUNCH_MODE=1
	    ;;
	f)
	    HOSTS_CONFIG=$OPTARG
 	    ;;
    esac
done
shift $(($OPTIND - 1))

if [ -z $HOSTS_CONFIG ] ; then
    HOSTS_CONFIG=hosts
fi

if [ ! -z $LAUNCH_MODE  ] ; then
    # launcher
    
    exe=$1

    # get rid of comments in the config file
    grep -v '#' $HOSTS_CONFIG > "$GLOBAL_CONF_FILE".tmp

    node_id=0
    while read l ; do
	
	HOST=$(echo $l | cut -f 1 -d " ")
	HOST_USER=$(echo $l | cut -f 4 -d " ")
	
	scp  ./$0 $HOST_USER@$HOST:/tmp/
	ssh -f $HOST_USER@$HOST "sh /tmp/${0##*/} $node_id $exe"
	
	node_id=$(($node_id + 1))

    done < "$GLOBAL_CONF_FILE".tmp

    \rm -f "$GLOBAL_CONF_FILE".tmp

else
    # launched
    
    node_id=$1
    exe=$2

    CPU_LOG=cpu"$node_id".log
    \rm -f $CPU_LOG

    
    true=1
    while [ $true ] ; do
	
	ps u | grep $exe | grep -v 'grep' | grep -v "${0##*/}" | awk '{print $3}' >> $CPU_LOG
	
	sleep 5

    done
fi


