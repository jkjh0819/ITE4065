#!/bin/bash
export MY_DB_PATH=$HOME/TA/Multicore/project4/mariadb # change this path
cmake \
-DCMAKE_INSTALL_PREFIX=$MY_DB_PATH/run \
-DSYSCONFDIR=$MY_DB_PATH/run \
-DMYSQL_TCP_PORT=3306 \
-DDEFAULT_CHARSET=utf8 \
-DWITH_EXTRA_CHARSETS=all \
-DDEFAULT_COLLATION=utf8_general_ci \
-DMYSQL_UNIX_ADDR=$MY_DB_PATH/run/mariadb.sock \
-DMYSQL_DATADIR=$MY_DB_PATH/data \
-DWITHOUT_TOKUDB_STORAGE_ENGINE=YES \
-DWITHOUT_MROONGA_STORAGE_ENGINE=YES \
-DWITHOUT_ROCKSDB_STORAGE_ENGINE=YES \