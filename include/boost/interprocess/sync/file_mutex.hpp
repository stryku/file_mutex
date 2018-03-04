//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_FILE_MUTEX_HPP
#define BOOST_INTERPROCESS_FILE_MUTEX_HPP

#ifndef BOOST_CONFIG_HPP
#include <boost/config.hpp>
#endif
#
#if defined(BOOST_HAS_PRAGMA_ONCE)
#pragma once
#endif

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/interprocess/sync/file_lock.hpp>

//!\file
//! Describes a class that wraps file access synchronization capabilities.

namespace boost {
namespace interprocess {

//! A file mutex is a mutual exclusion utility to synchronize access to file.
//! A file mutex has sharable and exclusive locking capabilities and
//! can be used with scoped_lock and sharable_lock classes.
//! A file mutex can't guarantee synchronization between threads of the same
//! process so just use file mutexes to synchronize threads from different
//! processes.
class file_mutex {
#if !defined(BOOST_INTERPROCESS_DOXYGEN_INVOKED)
  // Non-copyable
  BOOST_MOVABLE_BUT_NOT_COPYABLE(file_mutex)
#endif //#ifndef BOOST_INTERPROCESS_DOXYGEN_INVOKED

public:
  //! Constructs an empty file mutex.
  //! Does not throw
  file_mutex() {}

  //! Creates a file mutex. Creates a file for file lock in the same directory
  //! where original file is. File lock name consists of original file name +
  //! suffix. Throws interprocess_exception if there are no operating system
  //! resources.
  file_mutex(const filesystem::path &path,
             const filesystem::path &suffix = ".lock")
      : m_file_lock(create_file_lock(build_file_lock_path(path, suffix))) {}

  //! Moves the ovnership of "moved"'s file lock to *this.
  //! Does not throw.
  file_mutex(BOOST_RV_REF(file_mutex) moved) { this->swap(moved); }

  //! Moves the ownership of "moved"'s file lock to *this.
  //! Does not throw
  file_mutex &operator=(BOOST_RV_REF(file_mutex) moved) {
    file_mutex tmp(boost::move(moved));
    this->swap(tmp);
    return *this;
  }

  //! Swaps two file mutexes.
  //! Does not throw.
  void swap(file_mutex &other) { m_file_lock.swap(other.m_file_lock); }

  //! Effects: The calling thread tries to obtain exclusive ownership of the
  //! mutex,
  //!   and if another thread has exclusive, or sharable ownership of
  //!   the mutex, it waits until it can obtain the ownership.
  //! Throws: interprocess_exception on error.
  void lock() { m_file_lock.lock(); }

  //! Effects: The calling thread tries to acquire exclusive ownership of the
  //! mutex
  //!   without waiting. If no other thread has exclusive, or sharable
  //!   ownership of the mutex this succeeds.
  //! Returns: If it can acquire exclusive ownership immediately returns true.
  //!   If it has to wait, returns false.
  //! Throws: interprocess_exception on error.
  bool try_lock() { return m_file_lock.try_lock(); }

  //! Effects: The calling thread tries to acquire exclusive ownership of the
  //! mutex
  //!   waiting if necessary until no other thread has exclusive, or sharable
  //!   ownership of the mutex or abs_time is reached.
  //! Returns: If acquires exclusive ownership, returns true. Otherwise returns
  //! false.
  //! Throws: interprocess_exception on error.
  bool timed_lock(const boost::posix_time::ptime &abs_time) {
    return m_file_lock.timed_lock(abs_time);
  }

  //! Precondition: The thread must have exclusive ownership of the mutex.
  //! Effects: The calling thread releases the exclusive ownership of the mutex.
  //! Throws: An exception derived from interprocess_exception on error.
  void unlock() { m_file_lock.unlock(); }

  //! Effects: The calling thread tries to obtain sharable ownership of the
  //! mutex,
  //!   and if another thread has exclusive ownership of the mutex, waits until
  //!   it can obtain the ownership.
  //! Throws: interprocess_exception on error.
  void lock_sharable() { m_file_lock.lock_sharable(); }

  //! Effects: The calling thread tries to acquire sharable ownership of the
  //! mutex
  //!   without waiting. If no other thread has exclusive ownership of the
  //!   mutex this succeeds.
  //! Returns: If it can acquire sharable ownership immediately returns true. If
  //! it
  //!   has to wait, returns false.
  //! Throws: interprocess_exception on error.
  bool try_lock_sharable() { return m_file_lock.try_lock_sharable(); }

  //! Effects: The calling thread tries to acquire sharable ownership of the
  //! mutex
  //!   waiting if necessary until no other thread has exclusive ownership of
  //!   the mutex or abs_time is reached.
  //! Returns: If acquires sharable ownership, returns true. Otherwise returns
  //! false.
  //! Throws: interprocess_exception on error.
  bool timed_lock_sharable(const boost::posix_time::ptime &abs_time) {
    return m_file_lock.timed_lock_sharable(abs_time);
  }

  //! Precondition: The thread must have sharable ownership of the mutex.
  //! Effects: The calling thread releases the sharable ownership of the mutex.
  //! Throws: An exception derived from interprocess_exception on error.
  void unlock_sharable() { m_file_lock.unlock_sharable(); }

  //! Removes file used for locking.
  //! Returns: false if path did not exist in the first place, otherwise true.
  //! Throws: As specified in filesystem's error reporting
  static bool remove(const filesystem::path &path) {
    return filesystem::remove(path);
  }

private:
  filesystem::path build_file_lock_path(filesystem::path file_path,
                                        const filesystem::path &suffix) {
    return file_path += suffix;
  }

  file_lock create_file_lock(const filesystem::path &lock_path) {
    filesystem::ofstream(lock_path, filesystem::ofstream::app);
    return file_lock(lock_path.string().c_str());
  }

#if !defined(BOOST_INTERPROCESS_DOXYGEN_INVOKED)
private:
  file_lock m_file_lock;
#endif //#ifndef BOOST_INTERPROCESS_DOXYGEN_INVOKED
};

} // namespace interprocess {
} // namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif // BOOST_INTERPROCESS_FILE_MUTEX_HPP
