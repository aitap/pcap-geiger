pcap-geiger
===========

What is it?
-----------

This is a little program that sounds a click (like a [Geiger counter]) for each
network packet matching a [pcap filter]. Each click is proportional in volume
to the size of the packet.

How to use it?
--------------

Compile using a C++>=17 compiler, link with [libpcap] and [PortAudio].
The C preprocessor defines used are:

Symbol         | Meaning
---------------|---------------------------------------------------------------
`RING_SIZE`    | How many packets to queue before dropping
`TICK_SAMPLES` | How many sound samples per tick (assuming 44100Hz sample rate)
`MAX_EVENT`    | Maximum packet size (bigger packets are clipped)

You will probably need to launch this as administrator. The first command line
parameter should be the interface name (on Linux you may use `any` to mean all
network interfaces at the same time); the second command line parameter should
contain the filter. Use an empty string to capture all packets.


[Geiger counter]: https://en.wikipedia.org/wiki/Geiger_counter
[pcap filter]: http://www.tcpdump.org/manpages/pcap-filter.7.html
[libpcap]: https://en.wikipedia.org/wiki/Libpcap
[PortAudio]: http://www.portaudio.com/
