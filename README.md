Asio/Boost.Asio Raw Sockets Example
=======================

This is a simple demonstration of using Asio's basic_raw_socket template class to transmit packets with a custom transport layer header. In this example I have created a udp_header class for holding the contents of a UDP header, and I use the basic_raw_socket class to send UDP packets with the custom header.

This example should also work for Boost.Asio, but you'll have to change the namespaces from "asio" to "boost::asio" and the include files from "asio/FILE.hpp" to "boost/asio/FILE.hpp".

Clarification
-------------

Before you read any further, I think it's important to clarify what you can do with the basic_raw_socket class and what you can't do.

You __CAN__:
* Transmit packets with a custom transport layer (TCP, UDP, ICMP, etc.) header.
* Specify the constants you want to use for protocol type and family. 
* Send packets to a specified endpoint (IP address) even if the socket is not connected.

You __CANNOT__:
* Inject packets with a custom Ethernet or IP header into the network.

__UPDATE__: It turns out that as of Asio 1.10.0 / Boost 1.54, you can now transmit packets with a custom Ethernet header using the new generic::raw_protocol class. (thanks, Tomas!)

When I first tried using the basic_raw_socket class, I was attempting to inject packets into the network because for my work at NASA we needed to be able to send [AFDX packets](http://en.wikipedia.org/wiki/Avionics_Full-Duplex_Switched_Ethernet) over an Ethernet interface. 

I chose the Asio library for this because it's cross-platform, and unlike Boost.Asio it does not require linking with any libraries. Unfortunately, I discovered you cannot inject network packets using Asio or Boost.Asio. I haven't tried it yet, but another library that might allow you to inject network packets while remaining cross-platform is [libpnet6](http://pnet6.sourceforge.net/).

Warning
-------

I'm still very much a beginner to network programming, and this is really the first time I've programmed in C++, let alone the Boost/Asio library, so please excuse any bad programming styles I've used or any mistakes I've made. If you find a mistake or have a suggestion for improving this example, please submit an issue or pull request and I'll be glad to use your feedback to make it better. 

Dependencies
------------

Before this example will compile, you must first download the following libraries and extract them to the same folder as this repository:

* [Boost 1.5.0](http://www.boost.org/)
* [Asio 1.4.8](http://think-async.com/)

So right before you compile, your repository folder should look like this:

* boost_1_50_0/
* asio-1.4.8/
* example.cpp
* udp_header.hpp
* raw.hpp
* Makefile
* README.md
* LICENSE_1_0.txt

Compilation
-----------

To compile the example, just type:

	make

And to run the example, type:

	sudo ./example

The reason we have to use sudo (and therefore enter our user password) is because the SOCK_RAW protocol type requires root privileges.

Walkthrough
-----------

We start by initializing our udp_header object:

	udp_header udph;

We then set its fields using methods:
	
	udph.source_port(23);
	udph.destination_port(23);
	udph.length(11);
	udph.checksum(0x00); //optional for IPv4 

We can print out the contents of the UDP header for debugging using the to_string() method:
	
	udph.to_string();

We then create a small data payload for our packet, a simple string "---":

	std::string body("---"); //create a small data payload

We create an asio io_service object:

	asio::io_service io_service;

We then create our basic_raw_socket object, passing in the "raw" class defined in raw.hpp:

	asio::basic_raw_socket<asio::ip::raw> socket_(io_service);

The raw class defines the protocol type and family for the basic_raw_socket to use. It's set to use SOCK_RAW for the protocol type, IPPPROTO_UDP for the protocol, and PF_INET/PF_INET6 depending on whether you use IPv4 or IPv6. By setting the protocol to be IPPPROTO_UDP, the appropriate Byte in the IP header will be set, so our custom UDP header will be recognized by the network.

We then create an asio::streambuf object:

	asio::streambuf request_buffer;

And we use it to create an output stream:

	std::ostream os(&request_buffer);

Now we can put our custom header and the data payload into the output stream:

	os << udph << body;

Then we define the destination endpoint for our packet as an asio::ip::address_v4::bytes_type object:

	asio::ip::address_v4::bytes_type b = {{127, 0, 0, 1}};

I use 127.0.0.1 (localhost) for testing.

Then we create the endpoint object using the bytes_type object:
	
	asio::ip::raw::endpoint ep(asio::ip::address_v4(b), 23);

The 23 here is the destination port (telnet).

Finally, we open the socket:

	socket_.open();

And we send our packet to the endpoint:

	socket_.send_to(request_buffer.data(), ep);

I have these last two lines in a try/catch block so I can see any exceptions that might occur. For example, you might get "Operation not permitted" if you forget to run the program with sudo.

Wireshark
---------

[Wireshark](http://www.wireshark.org/download.html) is a terrific tool for looking at packets as they get sent across the network. You can use it to verify that this example program is actually sending packets.

When I run this example on my Macbook running Mac OSX 10.7.4, I see two items pop up in Wireshark:

* A UDP packet addressed to 127.0.0.1:23.
* An ICMP packet describing that the destination address and port are unreachable.

Wireshark says that the IP header checksum, which is 0x0000 in the packet, is incorrect. I believe this is caused by the fact that Wireshark is seeing the packet before the network card gets a chance to set the checksum (checksum offloading).

Also I believe it's fine that the ICMP packet says the destionation is unreachable, since we don't have anything listening for packets on port 23 running.

Acknowledgements
----------------

The Asio documentation, especially the [ICMP ping example](http://think-async.com/Asio/asio-1.4.8/doc/asio/examples.html) was very helpful for understanding how to use most of the Asio functions.

[This PDF document](http://boost.cowic.de/rc/pdf/asio_doc.pdf) written by the creator of Asio has even more information about how Asio/Boost.Asio works. There's an especially great table on page 19 that compares the BSD socket API to Asio.

Also thanks to Paul Gurniak for helping me understand how C++ template classes work.

License
-------
Copyright (c) 2012 Kevin D. Conley (kcon at stanford dot edu)

Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
