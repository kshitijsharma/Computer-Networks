set ns [new Simulator]
$ns color 0 blue
$ns color 1 red
set nf [open dv.nam w]
$ns namtrace-all $nf
set tr [open dv.tr w]
$ns trace-all $tr
proc finish {} {
 global nf ns tr
 $ns flush-trace
 close $tr
 exec nam dv.nam &
 exit 0
 }
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
$ns duplex-link $n1 $n3 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 1Mb 10ms DropTail
$ns duplex-link $n3 $n4 0.3Mb 10ms DropTail
$ns duplex-link $n4 $n5 1Mb 10ms DropTail
$ns duplex-link $n5 $n6 1Mb 10ms DropTail
$ns duplex-link $n5 $n7 1Mb 10ms DropTail

$ns duplex-link-op $n3 $n1 orient left-up
$ns duplex-link-op $n3 $n2 orient left-down
$ns duplex-link-op $n3 $n4 orient right
$ns duplex-link-op $n4 $n5 orient right
$ns duplex-link-op $n5 $n6 orient right-up
$ns duplex-link-op $n5 $n7 orient right-down

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
$ns at 0.0 "$cbr start"
$ns at 5.0 "finish"
$ns run