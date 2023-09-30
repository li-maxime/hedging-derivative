rm -rf "pricer-skel/Outputs"
rm -rf "pricer-skel/Results"
python ./PythonForModProg.py --hedge --exec="pricer-skel/release/hedge" --datadir="pricer-skel/data-hedge" --outdir="pricer-skel/hedge"
