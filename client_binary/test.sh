#/bin/bash

#latency test
TIMES=1000
MESSAGE="TEst"

#thorough put test
SIZE_MB=1024   #1GB

title[0]=singapore;   host[0]=xx.xx.xx.xx.ap-southeast-1.compute.amazonaws.com
title[1]=tokyo;       host[1]=xx.xx.xx.xx.ap-northeast-1.compute.amazonaws.com
title[2]=osaka;       host[2]=xx.xx.xx.xx.ap-northeast-3.compute.amazonaws.com
title[3]=oregon;      host[3]=xx.xx.xx.xx.us-west-2.compute.amazonaws.com
title[4]=california;  host[4]=xx.xx.xx.xx.us-west-1.compute.amazonaws.com
title[5]=virginia;    host[5]=xx.xx.xx.xx.compute-1.amazonaws.com
title[6]=frankfurt;   host[6]=xx.xx.xx.xx.eu-central-1.compute.amazonaws.com
title[7]=seoul;       host[7]=xx.xx.xx.xx.ap-northeast-2.compute.amazonaws.com
LAST_NUM=7

if [ "$(uname)" == 'Darwin' ];then
    OS='_mac'
elif [ "$(expr substr $(uname -s) 1 5)" == 'Linux' ]; then
    OS='_linux'
fi    

#laytency
echo 'laytency--------'
for i in `seq 0 ${LAST_NUM}`
do
    TIME=`date "+%y%m%d%H%M"`
    echo ${i} region:${title[$i]} 
    ./echo_client${OS} ${TIMES} ${host[$i]} ${MESSAGE} > laytency_${title[$i]}_${TIME}.log
    head -n 5 laytency_${title[$i]}_${TIME}.log
done

#through put
echo 'through put--------'
for i in `seq 0 ${LAST_NUM}`
do
    TIME=`date "+%y%m%d%H%M"`
    echo ${i} region:${title[$i]} 
    ./throughput_client${OS} ${SIZE_MB} ${host[$i]} > throughput_${title[$i]}_${TIME}.log
    head -n 5 throughput_${title[$i]}_${TIME}.log
done
