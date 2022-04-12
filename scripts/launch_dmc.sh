#! /bin/sh

# script to launch libdmc based apps
# it needs a configuration file

#set -x

ssh_error()
{
    if [ $? -ne 0 ] ; then
	echo "Cannot connect to $HOST with ssh."
	echo "Exiting."
	exit
    fi
}

HOSTS_CONFIG="hosts"
GLOBAL_CONF_FILE="global.conf"
NODE_CONF_FILE="node.conf"
SHELL_SCRIPT="dmc.sh"

DMC_DIR_PREFIX="dmc"

while getopts "lf:u:n:s:d:vp:z:tcw:y" ARG ; do

	case $ARG in
	t)
	    NO_STRIP="true"
	    ;;
	c)
	    COREDUMP="true"
	    ;;
	l)
	    NO_EXEC="true"
	    ;;
	f)
	    HOSTS_CONFIG=$OPTARG
 	    ;;
	u)
	    USER=$OPTARG
	    ;;
	n)
	    NB_USED_NODES=$OPTARG
	    ;;
	s)
	    STUFF=$OPTARG
	    ;;
	d)
	    DEST=$OPTARG
		WAIT_DEST=$DEST
	    ;;
	p)
	    DEFAULT_PORT=$OPTARG
	    ;;
	v)
	    VERBOSE="true"
	    ;;
	z)
	    SLEEP_TIME=$OPTARG
	    ;;
	w)
		WAIT_LIBDMC="$OPTARG"
		;;
	y)
		NO_COPY="true"
		;;
    esac
done

shift $(($OPTIND - 1))

\rm -f $GLOBAL_CONF_FILE
\rm -f $NODE_CONF_FILE
\rm -f "$GLOBAL_CONF_FILE".tmp

if [ ! -s $HOSTS_CONFIG ] ; then
    echo "'$HOSTS_CONFIG' does not exist or is empty." 
    echo "Exiting."
    exit
fi

if [ -n "$DEST" ] ; then
    DMC_DIR_PREFIX=${DEST%*/}/$DMC_DIR_PREFIX
fi

# get rid of comments in the config file
grep -v '#' $HOSTS_CONFIG > "$GLOBAL_CONF_FILE".tmp


unset l
while read l ; do
    
    HOST=$(echo $l | cut -f 1 -d " ")
    PORT=$(echo $l | cut -f 2 -d " ")
    NTHREADS=$(echo $l | cut -f 3 -d " ")
    HOST_USER=$(echo $l | cut -f 4 -d " ")

    if [ -z $HOST ] ; then
		continue
    fi
    
    if [ ! -z $DEFAULT_PORT ] ; then
		echo $HOST $DEFAULT_PORT $NTHREADS $HOST_USER >> $GLOBAL_CONF_FILE
    else
		echo $HOST $PORT $NTHREADS $HOST_USER >> $GLOBAL_CONF_FILE
    fi
    
done < "$GLOBAL_CONF_FILE".tmp

\rm -f "$GLOBAL_CONF_FILE".tmp

if [ -z "$NB_USED_NODES" ] ; then
    NB_USED_NODES=$(($(cat $GLOBAL_CONF_FILE | wc -l )+0))
fi

if [ -z "$1" ] ; then
     echo "No executable given."
     echo "Exiting."
     exit
else
    EXE="$1"
    if [ ! -x $EXE ] ; then
	echo "'$EXE' is not an executable or does no exist. "
	echo "Exiting."
	exit
    fi
fi

# arguments given to the distributed app, if there are any
shift
EXE_ARGS=$@

if [ -n "$VERBOSE" ] ; then
    echo
    echo "----------------------------------------------------------"
    echo
fi

# brief of given parameters
if [ -n "$VERBOSE" ] ; then
	print()
	{
		ONE=$1
		shift
		if [ -z "$1" ] ; then
	    	TWO="none"
			else
	    	TWO="$@"
		fi
		printf "%25ls\t%s\n" "$ONE" "$TWO"
	}

	echo "Deployment script launched with the following parameters:"
	echo
	print "Hosts configuration file" $HOSTS_CONFIG 
	print "Numbers of nodes" $NB_USED_NODES
	print "Executable" $EXE
	print "Executable args" $EXE_ARGS
	print "Additional stuff" "$STUFF"
	if [ ! -z $NO_EXEC ] ; then
		print "Launching phase" "deactivated"
	else
		print "Lauching phase" "activated"
	fi
	print "Distant working directory" $DEST
	print "Default port" $DEFAULT_PORT
	print "Launching delay" $SLEEP_TIME
	echo
	echo "----------------------------------------------------------"
	echo
fi    

echo
echo "Starting deployment phase..."
echo

# get rid of symbols
if [ -z $NO_STRIP ] ; then
    cp $EXE "$EXE".strip
    strip "$EXE".strip
    EXE="$EXE".strip
fi

# deployement phase
unset l
node_id=0
MASTER=""
while read l ; do
	
	HOST=$(echo $l | cut -f 1 -d " ")
	PORT=$(echo $l | cut -f 2 -d " ")
	NTHREADS=$(echo $l | cut -f 3 -d " ")
	HOST_USER=$(echo $l | cut -f 4 -d " ")
	ARCHI=$(echo $l | cut -f 5 -d " ")
	
	if [ -z "$MASTER" ] ; then
		MASTER=$HOST
	fi
	
	if [ -n "$HOST_USER" ] ; then
		U=$HOST_USER
	else
		U=$USER
	fi
	
	DEST=$DMC_DIR_PREFIX$node_id
	if [ -n "$VERBOSE" ] ; then
	    echo "Deploying '${EXE##*/}' to $HOST:$DEST with account $U..."
	fi

	if [ -z "$NO_COPY" ] ; then
	    RM='\rm -rf *'
	else
	    RM='\rm -f stats *.log'
	fi
	ssh -f $U@$HOST "\mkdir -p $DEST ; cd $DEST ; $RM"
	ssh_error
	
	if [ -n "$STUFF" ] && [ -z "$NO_COPY" ] ; then
		i=0
		scp -q -r $STUFF $U@$HOST:$DEST/
		ssh_error
	fi
	
	if [ -n $DEFAULT_PORT ] ; then
		PORT=$DEFAULT_PORT
	fi
	
	echo "$NB_USED_NODES $node_id $NTHREADS $PORT" > $NODE_CONF_FILE
	
	# generation of a script shell to launch the distant exe
	# easier when we need to lauch exe by hand
	echo "#! /bin/sh" > $SHELL_SCRIPT
	echo >> $SHELL_SCRIPT
	
	if [ -n "$COREDUMP" ] ; then
		echo "# We want a core" >> $SHELL_SCRIPT
		echo "ulimit -c unlimited" >> $SHELL_SCRIPT
		echo >> $SHELL_SCRIPT
	fi
	
	echo "./${EXE##*/} $EXE_ARGS" >> $SHELL_SCRIPT

	# TODO: manage different architectures
	if [ -z "$NO_COPY" ] ; then
	    scp -q $NODE_CONF_FILE $GLOBAL_CONF_FILE $EXE $SHELL_SCRIPT $U@$HOST:$DEST/
	    ssh_error
	fi

	# exit when we have deployed enough nodes
	if [ $node_id -eq $(($NB_USED_NODES - 1 )) ] ; then
		break
	fi

	node_id=$(($node_id+1))

done < $GLOBAL_CONF_FILE

if [ -n "$VERBOSE" ] ; then 
    echo
    echo "----------------------------------------------------------"
    echo
fi

# launching phase
max_node=$(($node_id - 1))
if [ -z $NO_EXEC ] ; then

    echo "Starting launching phase..."
    echo
    
	if [ ! -z $SLEEP_TIME ] ; then
		sleep $SLEEP_TIME
	fi
	
	
	node_id=0
	while read l ; do
	
	HOST=$(echo $l | cut -f 1 -d " ")
	HOST_USER=$(echo $l | cut -f 4 -d " ")
	
	if [ -n "$HOST_USER" ] ; then
		U=$HOST_USER
	else
		U=$USER
	fi

	if [ -n "$VERBOSE" ] ; then
	    echo "Launching $EXE $EXE_ARGS on $HOST with account $U"
	fi
	    
	DEST=$DMC_DIR_PREFIX$node_id
	ssh -f $U@$HOST "cd $DEST && sh dmc.sh > node$node_id.log 2>node"$node_id"_err.log"
	ssh_error
	
	# exit when we have launched enough nodes
	if [ $node_id -eq $(($NB_USED_NODES - 1)) ] ; then
		break
	fi

	node_id=$(($node_id + 1))
	
	done < $GLOBAL_CONF_FILE

	if [ -n "$VERBOSE" ] ; then 
	    echo
	    echo "----------------------------------------------------------"
	    echo
	fi

fi

\rm -f $GLOBAL_CONF_FILE
\rm -f $NODE_CONF_FILE

if [ -n "$WAIT_LIBDMC" ] ; then

	echo "Waiting for the end of ${EXE##*/}..."
	echo

	while true ; do
		ssh $U@$MASTER "test -s $DMC_DIR_PREFIX"0"/stats"
		if [ $? -eq 0 ]; then
			break
		fi
		sleep $WAIT_LIBDMC
	done

	#STATS_DIR=stats/"$NB_USED_NODES"_"$NTHREADS"/$(date "+%m%d_%Hh%Mm%Ss")
	STATS_DIR=stats/$(date "+%m%d_%Hh%Mm%Ss")
	mkdir -p $STATS_DIR

	scp $U@$MASTER:$DMC_DIR_PREFIX"0"/stats $STATS_DIR/.
	scp $U@$MASTER:$DMC_DIR_PREFIX"0"/*.log $STATS_DIR/.
     
fi
