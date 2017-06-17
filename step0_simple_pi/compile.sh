#!/bin/sh

mpic++ -o alpsdemo -I $ALPSCore_DIR/include main.cpp -L $ALPSCore_DIR/lib -lalps-mc -lalps-accumulators -lalps-params -lalps-utilities -lalps-hdf5 -lhdf5_serial -lboost_program_options
