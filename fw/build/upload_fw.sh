#!/bin/bash

echo "Upload firware into LABDOS with internal USB and bootloader"
echo "Usage  ./upload_fw.sh <LABDOS_PORT> <HEX_FILE>"

if [ "$#" -ne 2 ]
then
  echo "Wrong arguments.. "
  exit 1
fi

PORT=$1
FILE=$2

echo "Port ${PORT}"
echo "File ${FILE}"

avrdude -v -patmega1284p -carduino -P${PORT} -b57600 -D -Uflash:w:${FILE}:i
