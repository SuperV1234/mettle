#ifndef INC_METTLE_DRIVER_DETAIL_OPTIONAL_HPP
#define INC_METTLE_DRIVER_DETAIL_OPTIONAL_HPP

// Try to use N4480's optional class, or fall back to Boost's.
#if !defined(METTLE_NO_STDLIB_EXTS) && defined(__has_include)
#  if __has_include(<experimental/optional>)
#    include <experimental/optional>
#    define METTLE_OPTIONAL_NS std::experimental
#  else
#    include <boost/optional.hpp>
#    include <boost/optional/optional_io.hpp>
#    define METTLE_OPTIONAL_NS boost
#  endif
#else
#  include <boost/optional.hpp>
#  include <boost/optional/optional_io.hpp>
#  define METTLE_OPTIONAL_NS boost
#endif

#endif
