#!/bin/sh
crf_learn template train.txt model
crf_test  -m model test.txt

#crf_learn -a MIRA template train.data model
#crf_learn -c 10.0 template train.txt model
#crf_test  -m model test.data
#rm -f model
