//
// udp_header.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2012 Kevin D. Conley (kcon at stanford dot edu)
// Based on icmp_header.hpp (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef UDP_HEADER_HPP
#define UDP_HEADER_HPP

#include <istream>
#include <ostream>
#include <algorithm>

// UDP header for both IPv4 and IPv6.
//
// The wire format of a UDP header is:
//
// 0               8               16                             31
// +-------------------------------+------------------------------+      ---
// |                               |                              |       ^
// |          source port          |       destination port       |       |
// |                               |                              |       |
// +-------------------------------+------------------------------+    8 bytes
// |                               |                              |       |
// |            length             |           checksum           |       |
// |                               |                              |       v
// +-------------------------------+------------------------------+      ---
//
// The checksum field is optional for IPv4 but is required by IPv6.

class udp_header
{
public:
  udp_header() { std::fill(rep_, rep_ + sizeof(rep_), 0); }

  unsigned char source_port() const { return decode(0, 1); }
  unsigned char destination_port() const { return decode(2, 3); }
  unsigned short length() const { return decode(4, 5); }
  unsigned short checksum() const { return decode(6, 7); }

  void source_port(unsigned short n) { encode(0, 1, n); }
  void destination_port(unsigned short n) { encode(2, 3, n); }
  void length(unsigned short n) { encode(4, 5, n); }
  void checksum(unsigned short n) { encode(6, 7, n); }

  void to_string() 
  {
    unsigned int i;
    printf("{");
    for (i = 0; i < sizeof(rep_); i++) {
      printf(i == sizeof(rep_) - 1 ? "%.2X}\n" : "%.2X, ", rep_[i]);
    }
  }

  friend std::istream& operator>>(std::istream& is, udp_header& header)
    { return is.read(reinterpret_cast<char*>(header.rep_), 8); }

  friend std::ostream& operator<<(std::ostream& os, const udp_header& header)
    { return os.write(reinterpret_cast<const char*>(header.rep_), 8); }

private:
  unsigned short decode(int a, int b) const
    { return (rep_[a] << 8) + rep_[b]; }

  void encode(int a, int b, unsigned short n)
  {
    rep_[a] = static_cast<unsigned char>(n >> 8);
    rep_[b] = static_cast<unsigned char>(n & 0xFF);
  }

  unsigned char rep_[8];
};

#endif // UDP_HEADER_HPP
