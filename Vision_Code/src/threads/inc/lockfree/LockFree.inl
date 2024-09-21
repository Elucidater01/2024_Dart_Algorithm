#include "lockfree/LockFree.hpp"

template<typename T>
LockFree<T>::LockFree()
    : top_(nullptr) {}

template<typename T>
LockFree<T>::~LockFree() {
    node *top = this->top_.load();
    while (top != nullptr) {
        node* next = top->next;
        delete top;
        top = next;
    }
}

template<typename T>
void LockFree<T>::push(const T&value) noexcept {
    LockFree::node *new_node = new LockFree::node(value, this->top_.load());
    while (!this->top_.compare_exchange_weak(new_node->next, new_node));
}

template<typename T>
std::optional<T> LockFree<T>::top() noexcept {
    auto top = this->top_.load(std::memory_order_acquire);

    if (top == nullptr) {
        return std::optional<T>();
    }

    T value = top->value;
    delete top;
    return std::optional<T>(value);
}

template<typename T>
bool LockFree<T>::is_empty() const noexcept {
    return this->top_.load() == nullptr;
}

template<typename T>
bool LockFree<T>::is_lock_free() const noexcept {
    return this->top_.is_lock_free();
}

template<typename T>
LfStack<T>::LfStack()
    : LockFree<T>() {}


template<typename T>
LfStack<T>::~LfStack() = default;


template<typename T>
std::optional<T> LfStack<T>::pop() noexcept {
    auto top = this->top_.load();

    if (top == nullptr) {
        return std::optional<T>();
    }

    auto new_front = top->next;

    while (!this->top_.compare_exchange_weak(top, new_front)) {

        if (top == nullptr) {
            return std::optional<T>();
        }

        new_front = top->next;
    }

    T value = std::move(top->value);
    delete top;

    return std::optional<T>(value);
}
