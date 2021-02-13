set ns [new Simulator]

$ns color 0 blue
$ns color 1 red
set nf [open out.nam w]
$ns namtrace-all $nf
set tr [open out-DV.tr w]
$ns trace-all $tr

proc finish {} {
global nf ns tr
$ns flush-trace
close $tr
exec nam out.nam 
exit 0
}

set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]

$ns duplex-link $n1 $n3 2Mb 20ms DropTail
$ns duplex-link $n2 $n3 2Mb 20ms DropTail
$ns duplex-link $n3 $n4 2Mb 20ms DropTail
$ns duplex-link $n3 $n8 2Mb 20ms DropTail
$ns duplex-link $n4 $n5 1Mb 20ms DropTail
$ns duplex-link $n5 $n6 2Mb 20ms DropTail
$ns duplex-link $n5 $n7 2Mb 20ms DropTail
$ns duplex-link $n5 $n8 1Mb 20ms DropTail
$ns duplex-link-op $n1 $n3 orient left-up
$ns duplex-link-op $n2 $n3 orient right-up
$ns duplex-link-op $n3 $n4 orient right-up
$ns duplex-link-op $n3 $n8 orient right-down
$ns duplex-link-op $n4 $n5 orient up
$ns duplex-link-op $n5 $n6 orient right-up
$ns duplex-link-op $n5 $n7 orient right-down
$ns duplex-link-op $n5 $n8 orient left-down



set udp [new Agent/UDP]
$ns attach-agent $n2 $udp

$udp set class_ 0

set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp

set null [new Agent/Null]
$ns attach-agent $n6 $null

$ns connect $udp $null

$udp set fid_ 2
$udp set window_ 8000
$udp set packetSize_ 552

$ns rtproto DV
$ns rtmodel-at 1.0 down $n3 $n8



$ns at 0.0 "$cbr start"

$ns at 4.0 "finish"
$ns run
