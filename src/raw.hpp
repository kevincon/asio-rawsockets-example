//
// raw.hpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2012 Kevin D. Conley (kcon at stanford dot edu)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RAW_HPP
#define RAW_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/asio/detail/socket_types.hpp>
#include <boost/asio/basic_raw_socket.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/basic_resolver.hpp>
#include <boost/asio/ip/basic_resolver_iterator.hpp>
#include <boost/asio/ip/basic_resolver_query.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace asio {
namespace ip {

class raw
{
public:
  /// The type of a RAW endpoint.
  typedef boost::asio::ip::basic_endpoint<raw> endpoint;

  /// Construct to represent the IPv4 RAW protocol.
  static raw v4()
  {
    return raw(IPPROTO_UDP, PF_INET);
  }

  /// Construct to represent the IPv6 RAW protocol.
  static raw v6()
  {
    return raw(IPPROTO_UDP, PF_INET6);
  }

  // Default construction, compiler complains otherwise
  explicit raw()
    : protocol_(IPPROTO_UDP),
      family_(PF_INET)
  {
  }

  /// Obtain an identifier for the type of the protocol.
  int type() const
  {
    return SOCK_RAW;
  }

  /// Obtain an identifier for the protocol.
  int protocol() const
  {
    return protocol_;
  }

  /// Obtain an identifier for the protocol family.
  int family() const
  {
    return family_;
  }

  /// The RAW socket type.
  typedef boost::asio::basic_raw_socket<raw> socket;

  /// The RAW resolver type.
  typedef boost::asio::ip::basic_resolver<raw> resolver;

  /// Compare two protocols for equality.
  friend bool operator==(const raw& p1, const raw& p2)
  {
    return p1.protocol_ == p2.protocol_ && p1.family_ == p2.family_;
  }

  /// Compare two protocols for inequality.
  friend bool operator!=(const raw& p1, const raw& p2)
  {
    return p1.protocol_ != p2.protocol_ || p1.family_ != p2.family_;
  }

private:
  explicit raw(int protocol_id, int protocol_family)
    : protocol_(protocol_id),
      family_(protocol_family)
  {
  }

  int protocol_;
  int family_;
};

} // namespace ip
} // namespace asio


#endif // RAW_HPP
