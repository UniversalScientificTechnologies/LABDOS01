echo "LABDOS01 data logger"
echo "To exit, pres ctrl-a, ctrl-x"
echo "usage example: ./log_labdos.sh /dev/ttyUSB0 [description]"

mkdir ~/labdos01/ >/dev/null 2>&1

DESC=""
if [ -n "$2" ]; then
  DESC="_$2"
fi

picocom -b 115200 -q $1 | awk '{ print strftime("%s,")$0; fflush(); }' | tee ~/labdos01/LABDOS_$(date +"%Y%m%d_%H%M%S")${DESC}.log
