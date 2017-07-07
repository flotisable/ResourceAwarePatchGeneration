INC := -I$(HOME)/abc/src/ -I$(HOME)/abc/src/base/abc -I$(HOME)/abc/src/base/abci -I$(HOME)/abc/src/base/cmd -I$(HOME)/abc/src/base/io -I$(HOME)/abc/src/base/main -I$(HOME)/abc/src/base/exor \
        -I$(HOME)/abc/src/base/ver -I$(HOME)/abc/src/base/wlc -I$(HOME)/abc/src/base/acb -I$(HOME)/abc/src/base/bac -I$(HOME)/abc/src/base/cba -I$(HOME)/abc/src/base/pla -I$(HOME)/abc/src/base/test \
        -I$(HOME)/abc/src/map/mapper -I$(HOME)/abc/src/map/mio -I$(HOME)/abc/src/map/super -I$(HOME)/abc/src/map/if \
        -I$(HOME)/abc/src/map/amap -I$(HOME)/abc/src/map/cov -I$(HOME)/abc/src/map/scl -I$(HOME)/abc/src/map/mpm \
        -I$(HOME)/abc/src/misc/extra -I$(HOME)/abc/src/misc/mvc -I$(HOME)/abc/src/misc/st -I$(HOME)/abc/src/misc/util -I$(HOME)/abc/src/misc/nm \
        -I$(HOME)/abc/src/misc/vec -I$(HOME)/abc/src/misc/hash -I$(HOME)/abc/src/misc/tim -I$(HOME)/abc/src/misc/bzlib -I$(HOME)/abc/src/misc/zlib \
        -I$(HOME)/abc/src/misc/mem -I$(HOME)/abc/src/misc/bar -I$(HOME)/abc/src/misc/bbl -I$(HOME)/abc/src/misc/parse \
        -I$(HOME)/abc/src/opt/cut -I$(HOME)/abc/src/opt/fxu -I$(HOME)/abc/src/opt/fxch -I$(HOME)/abc/src/opt/rwr -I$(HOME)/abc/src/opt/mfs -I$(HOME)/abc/src/opt/sim \
        -I$(HOME)/abc/src/opt/ret -I$(HOME)/abc/src/opt/fret -I$(HOME)/abc/src/opt/res -I$(HOME)/abc/src/opt/lpk -I$(HOME)/abc/src/opt/nwk -I$(HOME)/abc/src/opt/rwt \
        -I$(HOME)/abc/src/opt/cgt -I$(HOME)/abc/src/opt/csw -I$(HOME)/abc/src/opt/dar -I$(HOME)/abc/src/opt/dau -I$(HOME)/abc/src/opt/dsc -I$(HOME)/abc/src/opt/sfm -I$(HOME)/abc/src/opt/sbd \
        -I$(HOME)/abc/src/sat/bsat -I$(HOME)/abc/src/sat/xsat -I$(HOME)/abc/src/sat/satoko -I$(HOME)/abc/src/sat/csat -I$(HOME)/abc/src/sat/msat -I$(HOME)/abc/src/sat/psat -I$(HOME)/abc/src/sat/cnf -I$(HOME)/abc/src/sat/bmc \
        -I$(HOME)/abc/src/bool/bdc -I$(HOME)/abc/src/bool/deco -I$(HOME)/abc/src/bool/dec -I$(HOME)/abc/src/bool/kit -I$(HOME)/abc/src/bool/lucky \
        -I$(HOME)/abc/src/bool/rsb -I$(HOME)/abc/src/bool/rpo \
        -I$(HOME)/abc/src/proof/pdr -I$(HOME)/abc/src/proof/abs -I$(HOME)/abc/src/proof/live -I$(HOME)/abc/src/proof/ssc -I$(HOME)/abc/src/proof/int \
        -I$(HOME)/abc/src/proof/cec -I$(HOME)/abc/src/proof/acec -I$(HOME)/abc/src/proof/dch -I$(HOME)/abc/src/proof/fraig -I$(HOME)/abc/src/proof/fra -I$(HOME)/abc/src/proof/ssw \
        -I$(HOME)/abc/src/aig/aig -I$(HOME)/abc/src/aig/saig -I$(HOME)/abc/src/aig/gia -I$(HOME)/abc/src/aig/ioa -I$(HOME)/abc/src/aig/ivy -I$(HOME)/abc/src/aig/hop \
        -I$(HOME)/abc/src/aig/miniaig
all:
	echo $(INC)
	g++ $(INC) main.cpp -L libabc.a -o main 
