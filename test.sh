#!/bin/sh

testCaseNum=12

testDir="testbenchs"
timeLimit="30m"

for (( i = 1 ; i <= ${testCaseNum} ; i++ )); do

  testSubDir="${testDir}/unit${i}"
  timeout ${timeLimit} ./rpgen ${testSubDir}/F.v ${testSubDir}/G.v ${testSubDir}/weight.txt ${testSubDir}/patch.v ${testSubDir}/out.v > ${testSubDir}/log.txt 2>&1 &

done
