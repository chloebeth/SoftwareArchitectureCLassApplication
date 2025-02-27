///////////////////////////////////////////////////////////////
//  Copyright 2012 - 2021 John Maddock.
//  Copyright 2021 Matt Borland.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_MP_CPP_INT_CORE_HPP
#define BOOST_MP_CPP_INT_CORE_HPP

#include <cstdint>
#include <type_traits>
#include <limits>
#include <boost/multiprecision/detail/standalone_config.hpp>
#include <boost/multiprecision/detail/number_base.hpp>
#include <boost/multiprecision/detail/assert.hpp>

namespace boost {
namespace multiprecision {

namespace detail {

//
// These traits calculate the largest type in the list
// [unsigned] long long, long, int, which has the specified number
// of bits.  Note that int_t and uint_t find the first
// member of the above list, not the last.  We want the last in the
// list to ensure that mixed arithmetic operations are as efficient
// as possible.
//

template <unsigned Bits>
struct int_t
{
   using exact = typename std::conditional<Bits == sizeof(signed char) * CHAR_BIT, signed char,
                 typename std::conditional<Bits == sizeof(short) * CHAR_BIT, short,
                 typename std::conditional<Bits == sizeof(int) * CHAR_BIT, int,
                 typename std::conditional<Bits == sizeof(long) * CHAR_BIT, long,
                 typename std::conditional<Bits == sizeof(long long) * CHAR_BIT, long long, void
                 >::type>::type>::type>::type>::type;
   
   static_assert(!std::is_same<void, exact>::value, "Number of bits does not match any standard data type. \
      Please file an issue at https://github.com/boostorg/multiprecision/ referencing this error from cpp_int_config.hpp");
};

template <unsigned Bits>
struct uint_t
{
   using exact = typename std::conditional<Bits == sizeof(unsigned char) * CHAR_BIT, unsigned char,
                 typename std::conditional<Bits == sizeof(unsigned short) * CHAR_BIT, unsigned short,
                 typename std::conditional<Bits == sizeof(unsigned int) * CHAR_BIT, unsigned int,
                 typename std::conditional<Bits == sizeof(unsigned long) * CHAR_BIT, unsigned long,
                 typename std::conditional<Bits == sizeof(unsigned long long) * CHAR_BIT, unsigned long long, void
                 >::type>::type>::type>::type>::type;

   static_assert(!std::is_same<void, exact>::value, "Number of bits does not match any standard data type. \
      Please file an issue at https://github.com/boostorg/multiprecision/ referencing this error from cpp_int_config.hpp");
};

template <unsigned N>
struct largest_signed_type
{
   using type = typename std::conditional<
       1 + std::numeric_limits<long long>::digits == N,
       long long,
       typename std::conditional<
           1 + std::numeric_limits<long>::digits == N,
           long,
           typename std::conditional<
               1 + std::numeric_limits<int>::digits == N,
               int,
               typename int_t<N>::exact>::type>::type>::type;
};

template <unsigned N>
struct largest_unsigned_type
{
   using type = typename std::conditional<
       std::numeric_limits<unsigned long long>::digits == N,
       unsigned long long,
       typename std::conditional<
           std::numeric_limits<unsigned long>::digits == N,
           unsigned long,
           typename std::conditional<
               std::numeric_limits<unsigned int>::digits == N,
               unsigned int,
               typename uint_t<N>::exact>::type>::type>::type;
};

} // namespace detail

#if defined(BOOST_HAS_INT128)

using limb_type = detail::largest_unsigned_type<64>::type;
using signed_limb_type = detail::largest_signed_type<64>::type;
using double_limb_type = boost::multiprecision::uint128_type;
using signed_double_limb_type = boost::multiprecision::int128_type;
constexpr const limb_type                       max_block_10        = 1000000000000000000uLL;
constexpr const limb_type                       digits_per_block_10 = 18;

inline BOOST_MP_CXX14_CONSTEXPR limb_type block_multiplier(unsigned count)
{
   constexpr const limb_type values[digits_per_block_10] = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000, 100000000000000, 1000000000000000, 10000000000000000, 100000000000000000, 1000000000000000000};
   BOOST_MP_ASSERT(count < digits_per_block_10);
   return values[count];
}

// Can't do formatted IO on an __int128
#define BOOST_MP_NO_DOUBLE_LIMB_TYPE_IO

// Need to specialise integer_traits for __int128 as it's not a normal native type:
} // namespace multiprecision

template <>
class integer_traits<multiprecision::double_limb_type>
    : public std::numeric_limits<multiprecision::double_limb_type>,
      public detail::integer_traits_base<multiprecision::double_limb_type, 0, ~static_cast<multiprecision::double_limb_type>(0)>
{};
template <>
class integer_traits<multiprecision::signed_double_limb_type>
    : public std::numeric_limits<multiprecision::signed_double_limb_type>,
      public detail::integer_traits_base<multiprecision::signed_double_limb_type, static_cast<multiprecision::signed_double_limb_type>((static_cast<multiprecision::double_limb_type>(1) << 127)), static_cast<multiprecision::signed_double_limb_type>(((~static_cast<multiprecision::double_limb_type>(0)) >> 1))>
{};

namespace multiprecision {

#else

using limb_type = detail::largest_unsigned_type<32>::type;
using signed_limb_type = detail::largest_signed_type<32>::type  ;
using double_limb_type = detail::largest_unsigned_type<64>::type;
using signed_double_limb_type = detail::largest_signed_type<64>::type  ;
constexpr const limb_type                       max_block_10        = 1000000000;
constexpr const limb_type                       digits_per_block_10 = 9;

inline limb_type block_multiplier(unsigned count)
{
   constexpr const limb_type values[digits_per_block_10] = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
   BOOST_MP_ASSERT(count < digits_per_block_10);
   return values[count];
}

#endif

constexpr const unsigned bits_per_limb = sizeof(limb_type) * CHAR_BIT;

template <class T>
inline BOOST_MP_CXX14_CONSTEXPR void minmax(const T& a, const T& b, T& aa, T& bb)
{
   if (a < b)
   {
      aa = a;
      bb = b;
   }
   else
   {
      aa = b;
      bb = a;
   }
}

enum cpp_integer_type
{
   signed_magnitude   = 1,
   unsigned_magnitude = 0,
   signed_packed      = 3,
   unsigned_packed    = 2
};

enum cpp_int_check_type
{
   checked   = 1,
   unchecked = 0
};

} // namespace multiprecision
} // namespace boost

#endif // BOOST_MP_CPP_INT_CORE_HPP
