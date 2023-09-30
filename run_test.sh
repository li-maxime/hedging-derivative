#!/bin/bash


rm -rf "pricer-skel/Outputs"
rm -rf "pricer-skel/Results"
python ./PythonForModProg.py --price --exec="pricer-skel/release/price0" --datadir="pricer-skel/data" --outdir="pricer-skel"
less "pricer-skel/Results/price0_output_result_price.json"
