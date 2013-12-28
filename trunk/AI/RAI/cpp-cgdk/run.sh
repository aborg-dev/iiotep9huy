#/bin/bash

int_handler() {
  echo "Interrupted"
  killall java
  exit
}

trap int_handler INT

#./compile-g++.sh

./local-runner.sh

sleep 3

./MyStrategy

