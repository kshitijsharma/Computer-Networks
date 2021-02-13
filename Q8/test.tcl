set ns [new Simulator]

$ns color 0 red
$ns color 1 green
$ns color 2 blue

#open trace file
set f [open out.tr w]
$ns trace-all $f
#open namtrace file
set nf [open out.nam w]
$ns namtrace-all $nf

set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]


$ns duplex-link $n1 $n3 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 2Mb 10ms DropTail
$ns duplex-link $n3 $n4 2Mb 10ms DropTail
$ns duplex-link $n4 $n5 0.3Mb 100ms DropTail
$ns duplex-link $n5 $n7 2Mb 10ms DropTail
$ns duplex-link $n5 $n6 2Mb 10ms DropTail



$ns duplex-link-op $n1 $n3 orient right-down
$ns duplex-link-op $n2 $n3 orient right-up
$ns duplex-link-op $n3 $n4 orient right
$ns duplex-link-op $n4 $n5 orient right
$ns duplex-link-op $n5 $n6 orient right-up
$ns duplex-link-op $n5 $n7 orient right-down


$ns queue-limit $n3 $n4 5


#tcp 1-6
set tcp [new Agent/TCP]
$tcp set class_ 0
set sink [new Agent/TCPSink]
$ns attach-agent $n1 $tcp
$ns attach-agent $n6 $sink
$ns connect $tcp $sink


set ftp [new Application/FTP]
$ftp attach-agent $tcp

#$ns rtmodel-at 1.0 down $n1 $n6
#$ns rtmodel-at 2.0 up $n1 $n6


$ns rtproto LS

$ns at 0.0 "$ftp start"


$ns at 5.0 "finish"

proc finish {} {
global ns f nf
$ns flush-trace
close $f
close $nf

puts "running nam..."
exec nam out.nam &
exit 0
}

$ns run
