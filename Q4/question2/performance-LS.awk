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
# Store start time
if (level == "rtProtoLS" && (event == "+" || event == "s") )
{
if (time < startTime)
{
startTime = time
}
txsize++;
}
# Update total received packetsâ€™ size and store packets
arrival time
if (level == "rtProtoLS" && event == "r" )
{
if (time > stopTime)
{
stopTime = time
}
recvdSize++
}
if (level == "rtProtoLS" && event == "d" )
{
drpSize++
}
}
END {
printf("\n\tLink State routing\nAverage Throughput[kbps] = %.2f\t\t\ns=%.2f\t\nd=%.2f\t\nr=%.2f\nStartTime=%.2f\t\nStopTime=%.2f\n",(recvdSize/(stopTime-startTime)),txsize,drpSize,recvdSize,startTime,stopTime)
}
