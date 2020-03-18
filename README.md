## Random-number generators
You can git submodule this repo to provide a selection of single-cycle random-number generators of various widths, including a software behavioural model.

All the clever stuff is in a [2013 paper](http://cas.ee.ic.ac.uk/people/dt10/research/rngs-fpga-lut_sr.html) by [David Thomas](http://cas.ee.ic.ac.uk/people/dt10/index.html).

The build is currently configured to produce RNGs for 32, 64 and 96-bit widths (including testbenches), each of which produce a pseudo-random sequence that can be verified in software (see gen-rng subdir).

See [BuildInfra](https://github.com/makestuff/ws-tools/blob/master/README.md) for details of how to incorporate this into your project.

You can install it in a new workspace `$HOME/my-workspace` like this:

    cd $HOME
    export ALTERA=/usr/local/altera-16.1  # or wherever
    mkws.sh my-workspace makestuff:dvr-rng
    export PROJ_HOME=$HOME/my-workspace

Then assuming you have ModelSim in your `PATH`, you can run the tests:

    cd $PROJ_HOME/ip/makestuff/dvr-rng
    make test

You can then compare the ModelSim results with the software model like this:

    # Verify the 32-bit RNG results
    grep RAND tb-wrap32/transcript | awk '{print $5}' > actual32.txt
    gen-rng/get_seq 1 256 | od -tx1 -w4 -An | awk '{print $4$3$2$1}' | tr '[a-f]' '[A-F]' > expected32.txt
    diff expected32.txt actual32.txt 
    
    # Verify the 64-bit RNG results
    grep RAND tb-wrap64/transcript | awk '{print $5}' > actual64.txt
    gen-rng/get_seq 2 256 | od -tx1 -w8 -An | awk '{print $8$7$6$5$4$3$2$1}' | tr '[a-f]' '[A-F]' > expected64.txt
    diff expected64.txt actual64.txt 
    
    # Verify the 96-bit RNG results
    grep RAND tb-wrap96/transcript | awk '{print $5}' > actual96.txt
    gen-rng/get_seq 3 256 | od -tx1 -w12 -An | awk '{print $12$11$10$9$8$7$6$5$4$3$2$1}' | tr '[a-f]' '[A-F]' > expected96.txt
    diff expected96.txt actual96.txt 

It would be nice if the wrapper modules `dvr_rng*.vhdl` could be generated too. Also it would be nice if it generated SystemVerilog rather than VHDL.
