#! /bin/bash
# makefil
for i in {1..20}
do 
    ./parser testcases/testcase_$i

    cat answers/testcase_$i
    echo "---"
done
