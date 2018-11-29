#ifndef WANDA_KEYED_HPP
#define WANDA_KEYED_HPP

namespace wanda
{

template <typename Derived>
struct keyed
{
  protected:
    struct key
    {
    };

    explicit keyed(key) {}
};

} // namespace wanda

#endif