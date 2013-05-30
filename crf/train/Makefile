#######################################################################
#
#  The MIT License (MIT)
# 
#  
#  Copyright (C) 2013 Yu Jing (yujing5b5d@gmail.com)
#
#  Permission is hereby granted, free of charge, to any person 
#  obtaining a copy of this software and associated documentation files 
#  (the "Software"), to deal in the Software without restriction,  
#  including without limitation the rights to use, copy, modify, merge, 
#  publish, distribute,sublicense, and/or sell copies of the Software, 
#  and to permit persons to whom the Software is furnished to do so,  
#  subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be 
#  included in all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
#  EXPRESS OR IMPLIED,INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
#  MERCHANTABILITY,FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
#  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
#  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
#  TORT OR OTHERWISE,ARISING FROM,OUT OF OR IN CONNECTION WITH THE 
#  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
########################################################################

########################################################################
#
# Makefile
#
# Edited By Yu Jing @ May. 8 , 2013
#
########################################################################

########################################################################
# CRF++: Yet Another CRF Tool Kit
# Copyright (C) 2005-2013 Taku Kudo, All rights reserved.
#
# Usage: crf_learn [options] files
#  -f, --freq=INT              use features that occuer no less than 
#				INT(default 1)
#  -m, --maxiter=INT           set INT for max iterations in LBFGS 
#				routine(default 10k)
#  -c, --cost=FLOAT            set FLOAT for cost parameter(default 1.0)
#  -e, --eta=FLOAT             set FLOAT for termination criterion
#				(default 0.0001)
#  -C, --convert               convert text model to binary model
#  -t, --textmodel             build also text model file for debugging
#  -a, --algorithm=(CRF|MIRA)  select training algorithm
#  -p, --thread=INT            number of threads (default auto-detect)
#  -H, --shrinking-size=INT    set INT for number of iterations variable 
#				needs to  be optimal before considered for 
#				shrinking. (default 20)
#  -v, --version               show the version and exit
#  -h, --help                  show this help and exit
########################################################################

#############################SYSTEM#####################################
LEARN			:= crf_learn
TEST			:= crf_test
CHECK			:= conlleval -d "\t" -r

SAY			:= @echo
RM			:= rm -rf


GADD			:= @git add 
GCMT			:= @git commit -m  
#############################FILES######################################

FLAGS			:= -c 1  #, -a MIRA  ... 



TEMPALTE		:=template
TRAINDATA		:=train.data
TESTDATA		:=test.data
MODEL			:=model
OUTPUT			:=output.txt
#############################MAIN#######################################

#${SAY} "Usage:make trainA|trainB|testA|testB|checkA|checkB"
all			:gitupdate train test check

nall			:gitupdate ntrain ntest check


train			:
	${LEARN} ${FLAGS} ${TEMPALTE} ${TRAINDATA} ${MODEL}

ntrain			:
	${LEARN} ${FLAGS} ${TEMPALTE} ${TESTDATA} ${MODEL}

test			:
	${TEST} -m ${MODEL} ${TESTDATA} > ${OUTPUT}

ntest			:
	${TEST} -m ${MODEL} ${TRAINDATA} > ${OUTPUT}	

check			:
	${CHECK} < ${OUTPUT}


gitupdate		:
	-${GADD} .
	-${GCMT} `date "+auto:%Y%m%d%H%M%S"`

clean			:
	${RM} ${MODEL}
	${RM} ${OUTPUT}
	${RM} *~  *~




















