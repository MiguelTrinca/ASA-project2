#!/bin/bash
TESTNAME=$1

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Pipes
# mkfifo piper;

FILES=./tests/*.in

test(){
  for f in $FILES
  do
    #echo "Processing $f file..."
    # take action on each file. $f store current file name

    # croping the prefix ("./") and the sufix (".in") to extract the main name
    tmp=${f#./*}
    f=${tmp%.in}

    echo ">>> Test $f"

    cat $f.in | ./a.out > $f.outhyp

    # diff if successful
    if [ $? -eq 0 ]; then
      colordiff $f.outhyp $f.out
      if [ $? -eq 0 ]; then
        echo -e "Test ${GREEN}PASSED ${NC}"
      else
        echo -e "Test ${RED}FAILED ${NC}(different)"
        read -n1 -r -p "Press any key to continue..." key

      fi
    else
      echo -e "Test ${RED}FAILED ${NC}(aborted)"
      read -n1 -r -p "Press any key to continue..." key
    fi

    rm $f.outhyp

    echo "" # new line

  done
}

min_test(){
  for f in $FILES
  do
    #echo "Processing $f file..."
    # take action on each file. $f store current file name

    # croping the prefix ("./") and the sufix (".in") to extract the main name
    tmp=${f#./*}
    f=${tmp%.in}

    echo"$f Test :"

    java -Dimport=$f.import -Din=$f.in -Dout=$f.outhyp mmt.app.App &

    # diff if successful
    if [ $? -eq 0 ]; then
      colordiff $f.outhyp $f.out > /dev/null
      if [ $? -eq 0 ]; then
        echo -e "${GREEN}PASSED ${NC}"
      else
        echo -e "${RED}FAILED ${NC}(different)"
      fi
    else
      echo -e "${RED}FAILED ${NC}(aborted)"
    fi

    rm $f.outhyp
  done
}


# Main Program
echo -e "======================================"
echo -e "  Wellcome to the OOP testing script  "
echo -e "                V. 0.1                "
echo -e "======================================\n"
FILES=./tests/*.in
test
FILES=./bigIO/*.in
test
