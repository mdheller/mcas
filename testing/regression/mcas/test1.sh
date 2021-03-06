#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/dist/lib
NODE_IP=$(ip addr | grep -Po 'inet \K10.0.0.[0-9]+' | head -1)
DESC=hstore-8-8
TESTID=1

if [ -z "$DISPLAY" ]; then
    echo '$DISPLAY not set; xterm would fail'
    exit 1
fi

# launch MCAS server
DAX_RESET=1 ./dist/bin/mcas --conf ./dist/testing/hstore-0.conf --debug 0 &> test$TESTID-server.log &
SERVER_PID=$!

#launch client
./dist/bin/kvstore-perf --port 11911 --cores 20 --server $NODE_IP --test put --component mcas --elements 2000000 --size 500000 --skip_json_reporting --device_name mlx5_0 --key_length 8 --value_length 8 --debug_level 0 &> test$TESTID-client.log &
CLIENT_PID=$!

# arm cleanup
trap "kill -9 $SERVER_PID $CLIENT_PID &> /dev/null" EXIT

# wait for client to complete
tail --pid=$CLIENT_PID -f /dev/null

# check result
iops=$(cat test$TESTID-client.log | grep -Po 'IOPS: \K[0-9]*')
echo "Test $TESTID: $DESC IOPS ($iops)"

if [ "$1" == "release" ]; then
    LIMIT="170000"
else
    LIMIT="110000"
fi

if [ -z "$iops" ]; then
    echo -e "Test $TESTID: \e[31mfail (no data)\e[0m"
elif [ "$iops" -lt $LIMIT ]; then
    echo -e "Test $TESTID: \e[31mfail\e[0m"
else
    echo -e "Test $TESTID: \e[32mpassed\e[0m"
fi
