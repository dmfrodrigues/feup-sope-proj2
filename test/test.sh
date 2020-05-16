#!/bin/bash

set -e

GREEN_INTENSIVE='\033[1;32m'
RED_INTENSIVE='\033[1;31m'
NC_INTENSIVE='\033[1m' # No Color
NC='\033[0m' # No Color

test/test01.sh
test/test02.sh
test/test03.sh
test/test04.sh
test/test05.sh
test/test06.sh
test/test07.sh
test/test08.sh
test/test09.sh