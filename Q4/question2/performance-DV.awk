BEGIN {
recvdSize = 0
txsize=0
drpSize=0
startTime = 0
stopTime = 0
thru=0
}
{
event = $1
time = $2
node_id = $3
pkt_size = $6
level = $5

if (level == "rtProtoDV" && (event == "+" || event == "s") )
{
if (time < startTime)
{
startTime = time
}
txsize++;
}

arrival time
if (level == "rtProtoDV" && event == "r" )
{
if (time > stopTime)
{
stopTime = time
}
recvdSize++
}
if (level == "rtProtoDV" && event == "d" )
{
drpSize++
}
}
END {
printf("\tDistance vector\nAverage Throughput[kbps] = %.2f\t\ts=%.2f\td=%.2f\tr=%.2f\nStartTime=%.2f\t\nStopTime=%.2f\n",(recvdSize/(stopTime-startTime)),txsize,drpSize,recvdSize,startTime,stopTime)
}
