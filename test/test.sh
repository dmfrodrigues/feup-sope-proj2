#!/bin/bash

set -e

GREEN_INTENSIVE='\033[1;32m'
RED_INTENSIVE='\033[1;31m'
NC_INTENSIVE='\033[1m' # No Color
NC='\033[0m' # No Color

test/test1.sh
test/test2-0.sh
test/test2.sh
test/test3.sh
