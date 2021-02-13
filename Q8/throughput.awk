BEGIN {
	recvdSize = 0
	txsize=0
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
	}# Update total received packets size and store packets arrival time
	if (level == "rtProtoLS" && event == "r" ) 
	{
		if (time > stopTime) 
		{
			stopTime = time
		}
		recvdSize++
	}
	
}
END {
printf("\n\t\tLink State Routing\n\n")
printf("\t\tAverage Throughput[kbps] = %.2f\n\t\tSent packets=%.2f\n\t\tReceived packets=%.2f\n\t\tStart Time=%.2f\n\t\tStop Time=5.00\n\n",(recvdSize/(stopTime-startTime)),txsize,recvdSize,startTime)
}
