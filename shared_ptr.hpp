// Copyright Davydov Nikolay 2021

#ifndef INCLUDE_SHARED_PTR_HPP_
#define INCLUDE_SHARED_PTR_HPP_
#include <atomic>
#include <iostream>
#include <utility>

template <typename T>
class SharedPtr {
private:
    std::atomic_uint* num_ref_ = nullptr;
    T* data_ = nullptr;

public:
    SharedPtr() noexcept = default;

    explicit SharedPtr(T* ptr) noexcept : num_ref_(nullptr), data_(ptr) {
        if (data_ != nullptr) {
            num_ref_ = new std::atomic_uint(1);
        }
    }
    SharedPtr(const SharedPtr& copy) noexcept
            : num_ref_(copy.num_ref_), data_(copy.data_) {
        if (copy.data_ != nullptr) {
            ++(*num_ref_);
        }
    }
    SharedPtr(SharedPtr&& moved) noexcept
            : num_ref_(moved.num_ref_), data_(moved.data_) {
        moved.num_ref_ = nullptr;
        moved.data_ = nullptr;
    }
    ~SharedPtr() {
        if (data_ != nullptr) {
            --(*num_ref_);
            if (*num_ref_ == 0) {
                delete num_ref_;
                delete data_;
            }
        }
    }

    auto operator=(const SharedPtr& copy) -> SharedPtr& {
        if (this == &copy) {
            return *this;
        }

        if (data_ != nullptr) {
            --(*num_ref_);
            if (*num_ref_ == 0) {
                delete num_ref_;
                delete data_;
            }
        }
        num_ref_ = copy.num_ref_;
        data_ = copy.data_;
        if (data_ != nullptr) {
            ++(*num_ref_);
        }
        return *this;
    }
    auto operator=(SharedPtr&& moved)  noexcept -> SharedPtr& {
        if (this == &moved) {
            return *this;
        }

        if (data_) {
            --(*num_ref_);
            if (*num_ref_ == 0) {
                delete num_ref_;
                delete data_;
            }
        }

        num_ref_ = moved.num_ref_;
        data_ = moved.data_;
        moved.num_ref_ = nullptr;
        moved.data_ = nullptr;
        return *this;
    }

    explicit operator bool() const { return data_ != nullptr; }
    auto operator*() const -> T& { return *data_; }
    auto operator->() const -> T* { return data_; }

    auto get() const -> T* { return data_; }
    void reset() {
        if (data_ != nullptr) {
            --(*num_ref_);
            if (*num_ref_ == 0) {
                delete num_ref_;
                delete data_;
            }
        }
        data_ = nullptr;
        num_ref_ = nullptr;
    }
    void reset(T* ptr) {
        if (data_ != nullptr) {
            --(*num_ref_);
            if (*num_ref_ == 0) {
                delete num_ref_;
                delete data_;
            }
        }
        data_ = ptr;
        num_ref_ = new std::atomic_uint(1);
    }
    void swap(SharedPtr& other) { std::swap(*this, other); }
    [[nodiscard]] auto use_count() const -> std::size_t {
        return data_ == nullptr ? 0 : static_cast<std::size_t>(*(num_ref_));
    }

    template <typename N>
    friend bool operator==(const SharedPtr<N>& lhs, const SharedPtr<N>& rhs);
    template <typename N>
    friend bool operator==(const SharedPtr<N>& lhs, std::nullptr_t);
};

template <typename N>
bool operator==(const SharedPtr<N>& lhs, const SharedPtr<N>& rhs) {
    return lhs.data_ == rhs.data_;
}

template <typename N>
bool operator==(const SharedPtr<N>& lhs, std::nullptr_t) {
    return lhs.data_ == nullptr;
}

template <typename N>
bool operator!=(const SharedPtr<N>& lhs, const SharedPtr<N>& rhs) {
    return !(lhs == rhs);
}

template <typename N>
bool operator!=(const SharedPtr<N>& lhs, std::nullptr_t) {
    return !(lhs.data_ == nullptr);
}

#endif  // INCLUDE_SHARED_PTR_HPP_