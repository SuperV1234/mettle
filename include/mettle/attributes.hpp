#ifndef INC_METTLE_ATTRIBUTES_HPP
#define INC_METTLE_ATTRIBUTES_HPP

#include <set>
#include <stdexcept>
#include <string>

namespace mettle {

enum class attr_action {
  run,
  skip,
  hide
};

class attr_base {
protected:
  constexpr attr_base(const char *name, attr_action action = attr_action::run)
    : name_(name), action_(action) {
    if(action == attr_action::hide)
      throw std::invalid_argument("attr's action can't be \"hide\"");
  }
  ~attr_base() = default;
public:
  std::string name() const {
    return name_;
  }

  attr_action action() const {
    return action_;
  }
private:
  const char *name_;
  attr_action action_;
};

class attr_instance {
public:
  using value_type = std::set<std::string>;
  explicit attr_instance(const attr_base &attr, const value_type &value)
    : attr_(attr), value_(value) {}

  std::string name() const {
    return attr_.name();
  }

  attr_action action() const {
    return attr_.action();
  }

  bool empty() const {
    return value_.empty();
  }

  const value_type & value() const {
    return value_;
  }
private:
  const attr_base &attr_;
  value_type value_;
};

namespace detail {
  struct attr_less {
    using is_transparent = void;

    bool operator ()(const attr_instance &lhs, const attr_instance &rhs) const {
      return lhs.name() < rhs.name();
    }

    bool operator ()(const attr_instance &lhs, const std::string &rhs) const {
      return lhs.name() < rhs;
    }

    bool operator ()(const std::string &lhs, const attr_instance &rhs) const {
      return lhs < rhs.name();
    }
  };
}

class bool_attr : public attr_base {
public:
  constexpr bool_attr(const char *name, attr_action action = attr_action::run)
    : attr_base(name, action) {}

  operator const attr_instance() const {
    return attr_instance(*this, {});
  }

  template<typename T>
  const attr_instance operator ()(T &&comment) const {
    return attr_instance(*this, {std::forward<T>(comment)});
  }
};

class string_attr : public attr_base {
public:
  constexpr string_attr(const char *name)
    : attr_base(name) {}

  template<typename T>
  const attr_instance operator ()(T &&value) const {
    return attr_instance(*this, {std::forward<T>(value)});
  }
};

class list_attr : public attr_base {
public:
  constexpr list_attr(const char *name)
    : attr_base(name) {}

  template<typename ...T>
  const attr_instance operator ()(T &&...args) const {
    return attr_instance(*this, {std::forward<T>(args)...});
  }
};

using attr_list = std::set<attr_instance, detail::attr_less>;

inline attr_list unite(const attr_list &lhs, const attr_list &rhs) {
  // XXX: Make list_attrs merge instead of overriding.
  attr_list all_attrs;
  std::set_union(
    lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
    std::inserter(all_attrs, all_attrs.begin()), detail::attr_less()
  );
  return all_attrs;
}

using filter_result = std::pair<attr_action, const attr_instance*>;

struct default_attr_filter {
  filter_result operator ()(const attr_list &attrs) const {
    for(const auto &attr : attrs) {
      if(attr.action() == attr_action::skip)
        return {attr_action::skip, &attr};
    }
    return {attr_action::run, nullptr};
  }
};

constexpr bool_attr skip("skip", attr_action::skip);

} // namespace mettle

#endif