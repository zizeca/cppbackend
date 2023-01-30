#!/bin/bash
export GAME_DB_URL=postgres://postgres:Mys3Cr3t@192.168.1.200:30432/test_db
#export GAME_DB_URL=postgres://postgres:Mys3Cr3t@localhost:30432/test_db
./build/bin/game_server --config-file ./data/config.json --www-root static/ --tick-period 100 --randomize-spawn-points --state-file ./save/out.txt --save-state-period 5000

# psql postgres://postgres:Mys3Cr3t@localhost:30432/test_db