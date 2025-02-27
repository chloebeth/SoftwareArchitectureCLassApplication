//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2013.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CONTAINER_DESTROYERS_HPP
#define BOOST_CONTAINER_DESTROYERS_HPP

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif

#if defined(BOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <boost/container/detail/config_begin.hpp>
#include <boost/container/detail/workaround.hpp>

#include <boost/container/allocator_traits.hpp>
#include <boost/move/detail/to_raw_pointer.hpp>
#include <boost/container/detail/version_type.hpp>

namespace boost {
namespace container {
namespace dtl {

//!A deleter for scoped_ptr that deallocates the memory
//!allocated for an object using a STL allocator.
template <class Allocator>
struct scoped_deallocator
{
   typedef allocator_traits<Allocator> allocator_traits_type;
   typedef typename allocator_traits_type::pointer pointer;
   typedef dtl::integral_constant<unsigned,
      boost::container::dtl::
         version<Allocator>::value>                   alloc_version;

   private:
   void priv_deallocate(version_1)
   {  m_alloc.deallocate(m_ptr, 1); }

   void priv_deallocate(version_2)
   {  m_alloc.deallocate_one(m_ptr); }

   BOOST_MOVABLE_BUT_NOT_COPYABLE(scoped_deallocator)

   public:

   pointer     m_ptr;
   Allocator&  m_alloc;

   scoped_deallocator(pointer p, Allocator& a)
      : m_ptr(p), m_alloc(a)
   {}

   ~scoped_deallocator()
   {  if (m_ptr)priv_deallocate(alloc_version());  }

   scoped_deallocator(BOOST_RV_REF(scoped_deallocator) o)
      :  m_ptr(o.m_ptr), m_alloc(o.m_alloc)
   {  o.release();  }

   pointer get() const
   {  return m_ptr;  }

   void set(const pointer &p)
   {  m_ptr = p;  }

   void release()
   {  m_ptr = 0; }
};

template <class Allocator>
struct null_scoped_deallocator
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   typedef typename AllocTraits::pointer    pointer;

   null_scoped_deallocator(pointer, Allocator&, std::size_t)
   {}

   void release()
   {}

   pointer get() const
   {  return pointer();  }

   void set(const pointer &)
   {}
};

//!A deleter for scoped_ptr that deallocates the memory
//!allocated for an array of objects using a STL allocator.
template <class Allocator>
struct scoped_array_deallocator
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   typedef typename AllocTraits::pointer    pointer;
   typedef typename AllocTraits::size_type  size_type;

   scoped_array_deallocator(pointer p, Allocator& a, std::size_t length)
      : m_ptr(p), m_alloc(a), m_length(length) {}

   ~scoped_array_deallocator()
   {  if (m_ptr) m_alloc.deallocate(m_ptr, size_type(m_length));  }

   void release()
   {  m_ptr = 0; }

   private:
   pointer     m_ptr;
   Allocator&  m_alloc;
   std::size_t m_length;
};

template <class Allocator>
struct null_scoped_array_deallocator
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   typedef typename AllocTraits::pointer    pointer;

   null_scoped_array_deallocator(pointer, Allocator&, std::size_t)
   {}

   void release()
   {}
};

template <class Allocator>
struct scoped_destroy_deallocator
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   typedef typename AllocTraits::pointer    pointer;
   typedef dtl::integral_constant<unsigned,
      boost::container::dtl::
         version<Allocator>::value>                          alloc_version;

   scoped_destroy_deallocator(pointer p, Allocator& a)
      : m_ptr(p), m_alloc(a) {}

   ~scoped_destroy_deallocator()
   {
      if(m_ptr){
         AllocTraits::destroy(m_alloc, boost::movelib::to_raw_pointer(m_ptr));
         priv_deallocate(m_ptr, alloc_version());
      }
   }

   void release()
   {  m_ptr = 0; }

   private:

   void priv_deallocate(const pointer &p, version_1)
   {  AllocTraits::deallocate(m_alloc, p, 1); }

   void priv_deallocate(const pointer &p, version_2)
   {  m_alloc.deallocate_one(p); }

   pointer     m_ptr;
   Allocator&  m_alloc;
};


//!A deleter for scoped_ptr that destroys
//!an object using a STL allocator.
template <class Allocator>
struct scoped_destructor_n
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   typedef typename AllocTraits::pointer    pointer;
   typedef typename AllocTraits::value_type value_type;

   BOOST_CONTAINER_FORCEINLINE scoped_destructor_n(pointer p, Allocator& a, std::size_t n)
      : m_p(p), m_a(a), m_n(n)
   {}

   BOOST_CONTAINER_FORCEINLINE void release()
   {  m_p = 0; m_n = 0; }

   BOOST_CONTAINER_FORCEINLINE void increment_size(std::size_t inc)
   {  m_n += inc;   }

   BOOST_CONTAINER_FORCEINLINE void increment_size_backwards(std::size_t inc)
   {  m_n += inc;   m_p -= std::ptrdiff_t(inc);  }

   BOOST_CONTAINER_FORCEINLINE void shrink_forward(std::size_t inc)
   {  m_n -= inc;   m_p += std::ptrdiff_t(inc);  }

   ~scoped_destructor_n()
   {
      if(m_n){
         value_type *raw_ptr = boost::movelib::to_raw_pointer(m_p);
         do {
            --m_n;
            AllocTraits::destroy(m_a, raw_ptr);
            ++raw_ptr;
         } while(m_n);
      }
   }

   private:
   pointer     m_p;
   Allocator & m_a;
   std::size_t m_n;
};

//!A deleter for scoped_ptr that destroys
//!an object using a STL allocator.
template <class Allocator>
struct null_scoped_destructor_n
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   typedef typename AllocTraits::pointer pointer;

   BOOST_CONTAINER_FORCEINLINE null_scoped_destructor_n(pointer, Allocator&, std::size_t)
   {}

   BOOST_CONTAINER_FORCEINLINE void increment_size(std::size_t)
   {}

   BOOST_CONTAINER_FORCEINLINE void increment_size_backwards(std::size_t)
   {}

   BOOST_CONTAINER_FORCEINLINE void shrink_forward(std::size_t)
   {}

   BOOST_CONTAINER_FORCEINLINE void release()
   {}
};


//!A deleter for scoped_ptr that destroys
//!an object using a STL allocator.
template <class Allocator>
struct scoped_destructor_range
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   typedef typename AllocTraits::pointer    pointer;
   typedef typename AllocTraits::value_type value_type;
   
   BOOST_CONTAINER_FORCEINLINE scoped_destructor_range(pointer p, pointer e, Allocator& a)
      : m_p(p), m_e(e), m_a(a)
   {}

   BOOST_CONTAINER_FORCEINLINE void release()
   {  m_p = pointer(); m_e = pointer(); }

   BOOST_CONTAINER_FORCEINLINE void set_end(pointer e)
   {  m_e = e;   }

   BOOST_CONTAINER_FORCEINLINE void set_begin(pointer b)
   {  m_p = b;  }

   BOOST_CONTAINER_FORCEINLINE void set_range(pointer b, pointer e)
   {  m_p = b; m_e = e;   }

   ~scoped_destructor_range()
   {
      while(m_p != m_e){
         value_type *raw_ptr = boost::movelib::to_raw_pointer(m_p);
         AllocTraits::destroy(m_a, raw_ptr);
         ++m_p;
      }
   }

   private:
   pointer     m_p;
   pointer     m_e;
   Allocator & m_a;
};

//!A deleter for scoped_ptr that destroys
//!an object using a STL allocator.
template <class Allocator>
struct null_scoped_destructor_range
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   typedef typename AllocTraits::pointer pointer;

   BOOST_CONTAINER_FORCEINLINE null_scoped_destructor_range(pointer, pointer, Allocator&)
   {}

   BOOST_CONTAINER_FORCEINLINE void release()
   {}

   BOOST_CONTAINER_FORCEINLINE void set_end(pointer)
   {}

   BOOST_CONTAINER_FORCEINLINE void set_begin(pointer)
   {}

   BOOST_CONTAINER_FORCEINLINE void set_range(pointer, pointer)
   {}
};


template<class Allocator>
class scoped_destructor
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   public:
   typedef typename Allocator::value_type value_type;
   BOOST_CONTAINER_FORCEINLINE scoped_destructor(Allocator &a, value_type *pv)
      : pv_(pv), a_(a)
   {}

   BOOST_CONTAINER_FORCEINLINE ~scoped_destructor()
   {
      if(pv_){
         AllocTraits::destroy(a_, pv_);
      }
   }

   BOOST_CONTAINER_FORCEINLINE void release()
   {  pv_ = 0; }


   BOOST_CONTAINER_FORCEINLINE void set(value_type *ptr) { pv_ = ptr; }

   BOOST_CONTAINER_FORCEINLINE value_type *get() const { return pv_; }

   private:
   value_type *pv_;
   Allocator &a_;
};


template<class Allocator, class Value = typename Allocator::value_type>
class value_destructor
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   public:
   typedef Value value_type;
   BOOST_CONTAINER_FORCEINLINE value_destructor(Allocator &a, value_type &rv)
      : rv_(rv), a_(a)
   {}

   BOOST_CONTAINER_FORCEINLINE ~value_destructor()
   {
      AllocTraits::destroy(a_, &rv_);
   }

   private:
   value_type &rv_;
   Allocator &a_;
};

template <class Allocator>
class allocator_destroyer
{
   typedef boost::container::allocator_traits<Allocator> AllocTraits;
   typedef typename AllocTraits::value_type value_type;
   typedef typename AllocTraits::pointer    pointer;
   typedef dtl::integral_constant<unsigned,
      boost::container::dtl::
         version<Allocator>::value>                           alloc_version;

   private:
   Allocator & a_;

   private:
   BOOST_CONTAINER_FORCEINLINE void priv_deallocate(const pointer &p, version_1)
   {  AllocTraits::deallocate(a_,p, 1); }

   BOOST_CONTAINER_FORCEINLINE void priv_deallocate(const pointer &p, version_2)
   {  a_.deallocate_one(p); }

   public:
   BOOST_CONTAINER_FORCEINLINE explicit allocator_destroyer(Allocator &a)
      : a_(a)
   {}

   BOOST_CONTAINER_FORCEINLINE void operator()(const pointer &p)
   {
      AllocTraits::destroy(a_, boost::movelib::to_raw_pointer(p));
      this->priv_deallocate(p, alloc_version());
   }
};

template <class Allocator>
class allocator_destroyer_and_chain_builder
{
   typedef allocator_traits<Allocator> allocator_traits_type;
   typedef typename allocator_traits_type::value_type value_type;
   typedef typename Allocator::multiallocation_chain    multiallocation_chain;

   Allocator & a_;
   multiallocation_chain &c_;

   public:
   BOOST_CONTAINER_FORCEINLINE allocator_destroyer_and_chain_builder(Allocator &a, multiallocation_chain &c)
      :  a_(a), c_(c)
   {}

   BOOST_CONTAINER_FORCEINLINE void operator()(const typename Allocator::pointer &p)
   {
      allocator_traits<Allocator>::destroy(a_, boost::movelib::to_raw_pointer(p));
      c_.push_back(p);
   }
};

template <class Allocator>
class allocator_multialloc_chain_node_deallocator
{
   typedef allocator_traits<Allocator> allocator_traits_type;
   typedef typename allocator_traits_type::value_type value_type;
   typedef typename Allocator::multiallocation_chain    multiallocation_chain;
   typedef allocator_destroyer_and_chain_builder<Allocator> chain_builder;

   Allocator & a_;
   multiallocation_chain c_;

   public:
   BOOST_CONTAINER_FORCEINLINE allocator_multialloc_chain_node_deallocator(Allocator &a)
      :  a_(a), c_()
   {}

   BOOST_CONTAINER_FORCEINLINE chain_builder get_chain_builder()
   {  return chain_builder(a_, c_);  }

   BOOST_CONTAINER_FORCEINLINE ~allocator_multialloc_chain_node_deallocator()
   {
      a_.deallocate_individual(c_);
   }
};

}  //namespace dtl {
}  //namespace container {
}  //namespace boost {

#include <boost/container/detail/config_end.hpp>

#endif   //#ifndef BOOST_CONTAINER_DESTROYERS_HPP
