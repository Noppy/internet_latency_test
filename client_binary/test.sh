#/bin/sh

#latency test
TIMES=1000
MESSAGE="TEst"

#thorough put test
SIZE_MB=1024

title[0]=singapore;   host[0]=ec2-xxx-xxx-xxx-xxx.ap-southeast-1.compute.amazonaws.com
title[1]=tokyo;       host[1]=ec2-xxx-xxx-xxx-xxx.ap-northeast-1.compute.amazonaws.com
title[2]=osaka;       host[2]=ec2-xxx-xxx-xxx-xxx.ap-northeast-3.compute.amazonaws.com
title[3]=oregon;      host[3]=ec2-xxx-xxx-xxx-xxx.us-west-2.compute.amazonaws.com
title[4]=california;  host[4]=ec2-xxx-xxx-xxx-xxx.us-west-1.compute.amazonaws.com
title[5]=virginia;    host[5]=ec2-xxx-xxx-xxx-xxx.compute-1.amazonaws.com
title[6]=frankfurt;   host[6]=ec2-xxx-xxx-xxx-xxx.eu-central-1.compute.amazonaws.com
title[7]=seoul;       host[7]=ec2-xxx-xxx-xxx-xxx.ap-northeast-2.compute.amazonaws.com
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
    echo ${i} region:${title[$i]} 
    ./echo_client${OS} ${TIMES} ${host[$i]} ${MESSAGE} > laytency_${title[$i]}.log
    head -n 5 laytency_${title[$i]}.log
done

#through put
echo 'through put--------'
for i in `seq 0 ${LAST_NUM}`
do
    echo ${i} region:${title[$i]} 
    ./throughput_client${OS} ${SIZE_MB} ${host[$i]} > throughput_${title[$i]}.log
    head -n 5 throughput_${title[$i]}.log
done
