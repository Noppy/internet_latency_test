#/bin/bash

#latency test
TIMES=1
MESSAGE="TEst"

#thorough put test
SIZE_MB=1

title[0]=singapore;   host[0]=ec2-54-169-233-147.ap-southeast-1.compute.amazonaws.com
title[1]=tokyo;       host[1]=ec2-13-230-60-207.ap-northeast-1.compute.amazonaws.com
title[2]=osaka;       host[2]=ec2-3-208-140-176.ap-northeast-3.compute.amazonaws.com
title[3]=oregon;      host[3]=ec2-34-214-208-17.us-west-2.compute.amazonaws.com
title[4]=california;  host[4]=ec2-52-53-203-162.us-west-1.compute.amazonaws.com
title[5]=virginia;    host[5]=ec2-54-152-142-225.compute-1.amazonaws.com
title[6]=frankfurt;   host[6]=ec2-18-184-145-89.eu-central-1.compute.amazonaws.com
title[7]=seoul;       host[7]=ec2-13-125-215-168.ap-northeast-2.compute.amazonaws.com
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
