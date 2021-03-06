#!/bin/bash

#TARGET=/home/server10g/huunghia/mmt-security
#USER=server10g
#IP=192.168.0.7
#PORT=22

#USER=montimage
#IP=192.168.0.194

#TARGET=/home/server10ga/huunghia/mmt-security
#USER=root
#IP=192.168.0.36

TARGET=hn/mmt-security
USER=mmt
IP=localhost
PORT=2222

#PORT=22
#IP=192.168.0.80
#USER=mmt
#TARGET=hn/mmt-security

#TARGET=mmt/mmt-security
#USER=epc
#IP=192.168.0.163
#PORT=2222
#
#TARGET=mmt/mmt-security
#USER=montimage
#IP=192.168.0.235
#PORT=2003


#centos
#TARGET=/home/montimage/mmt-security
#USER=montimage
#PORT=2233

#TARGET=/home/server10g/huunghia/mmt-security
#USER=server10g
#IP=192.168.0.7
#PORT=22

rsync -e "ssh -i /Users/nhnghia/.ssh/id_rsa -p $PORT" -rca ./* .git $USER@$IP:$TARGET

DEBUG="DEBUG=1 UPDATE_RULES=1"
#DEBUG="DEBUG=1 VALGRIND=1"


#RUN="make install $DEBUG"

#RUN="cd /home/server10g/huunghia/ && rm \"callgrind.out.*\" ; valgrind --tool=callgrind ./mmt_sec_standalone -t 10_800_116kfps.pcap -c 1 -x '101-1000' -v"
#RUN="make standalone $DEBUG && ./copy mmt_sec_standalone && ssh root@192.168.0.7 \"$RUN\" && ./rcopy \"callgrind*\" "

#TEST="message_t"
#RUN="make test.$TEST DEBUG=1 && ./test.$TEST"

#RUN="make sample_rules"
#RUN="make $DEBUG"

#RUN="make check $DEBUG"
RUN="make clean; make $DEBUG"
#RUN="make clean ; make test.spsc $DEBUG"
#RUN="make perf.hash && ./perf.hash"

#RUN=""
#RUN="$RUN rules/78.sendate_http_video_ddos.so"
#RUN="$RUN rules/79.sendate_http_video_overloading.so"
#RUN="rm rules/*.so; make $RUN"

#RUN="make $DEBUG && valgrind --leak-check=full  ./mmt_sec_standalone -t /media/sf_share_vbox/pcap/smallFlows.pcap"
#RUN="rm rules/10.*.so ; make $DEBUG ; ./compile_rule rules/10.http.port.so rules/10.http.port.xml; ./mmt_sec_standalone -t /media/sf_share_vbox/pcap/smallFlows.pcap -v"

#RUN="make clean; make $DEBUG; ./compile_rule ../mmt-probe/rules/forward.so test/forward/forward-packets.xml"
RUN="$RUN; sudo make install"

#RUN=""

echo "Compiling `pwd` ... on $USER@$IP:$TARGET"
echo "Run: $RUN"

ssh -p $PORT $USER@$IP "cd $TARGET && $RUN"
