#ifndef LF_STACK_HPP_
#define LF_STACK_HPP_
#include <atomic>
#include <optional>
template<typename T>
class LockFree {
public:
    LockFree();
    ~LockFree();
    void push(const T& value) noexcept ;
    std::optional<T> top() noexcept;
    [[nodiscard]] bool is_empty() const noexcept;
    [[nodiscard]] bool is_lock_free() const noexcept;
protected:
    struct node {
        T value;
        node *next;
        explicit node(const T &value) : value(std::move(value)), next(nullptr) {}
        explicit node(const T &value, node *next) : value(std::move(value)), next(next) {}
    };
    std::atomic<node*> top_;
};

template<typename T>
class LfStack : public LockFree<T> {
public:
    LfStack();
    ~LfStack();
	std::optional<T> pop() noexcept;
};

#include "lockfree/LockFree.inl"
#endif