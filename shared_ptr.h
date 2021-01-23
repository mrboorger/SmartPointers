#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

#include <cassert>
#include <memory>

namespace pointers {

template<class T>
class SharedPtr {
  friend class Tester;
  template<typename T2> friend
  class WeakPtr;
 public:
  SharedPtr() : value_(nullptr), use_count_(nullptr), weak_count_(nullptr) {}

  explicit SharedPtr(T* ptr)
      : value_(ptr), use_count_(nullptr), weak_count_(nullptr) {
    if (value_ != nullptr) {
      use_count_ = new int(1);
      weak_count_ = new int(0);
    }
  }

  SharedPtr(const SharedPtr<T>& shared_ptr)
      : value_(shared_ptr.value_), use_count_(shared_ptr.use_count_),
        weak_count_(shared_ptr.weak_count_) {
    IncUseCount();
  }

  SharedPtr(SharedPtr<T>&& shared_ptr) noexcept
      : value_(shared_ptr.value_), use_count_(shared_ptr.use_count_),
        weak_count_(shared_ptr.weak_count_) {
    shared_ptr.RemovePointers();
  }

  ~SharedPtr() {
    Reset();
  }

  T* Get() {
    return value_;
  }

  const T* Get() const {
    return value_;
  }

  void Reset() {
    if (value_ != nullptr) {
      --(*use_count_);
      if (*use_count_ == 0) {
        delete value_;
        if (*weak_count_ == 0) {
          delete use_count_;
          delete weak_count_;
        }
      }
      RemovePointers();
    }
  }

  SharedPtr<T>& operator=(SharedPtr<T>&& ptr) noexcept {
    if (this != &ptr) {
      Reset();
      CopyPointers(ptr);
      ptr.RemovePointers();
    }
    return *this;
  }

  SharedPtr<T>& operator=(const SharedPtr<T>& ptr) {
    if (this != &ptr) {
      Reset();
      CopyPointers(ptr);
      IncUseCount();
    }
    return *this;
  }

  T& operator*() {
    assert(value_ != nullptr);
    return *value_;
  }

  const T& operator*() const {
    assert(value_ != nullptr);
    return *value_;
  }

  T* operator->() {
    assert(value_ != nullptr);
    return value_;
  }

  const T* operator->() const {
    assert(value_ != nullptr);
    return value_;
  }

  bool operator==(const SharedPtr<T>& shared_ptr) const {
    return value_ == shared_ptr.value_;
  }

  bool operator==(const T* ptr) const {
    return value_ == ptr;
  }

  friend bool operator==(const T* lhs, const SharedPtr<T>& rhs) {
    return lhs == rhs.value_;
  }

  bool operator!=(const SharedPtr<T>& shared_ptr) const {
    return value_ != shared_ptr.value_;
  }

  bool operator!=(const T* ptr) const {
    return value_ != ptr;
  }

  friend bool operator!=(const T* lhs, const SharedPtr<T>& rhs) {
    return lhs != rhs.value_;
  }

 private:
  void RemovePointers() {
    value_ = nullptr;
    use_count_ = nullptr;
    weak_count_ = nullptr;
  }

  void CopyPointers(const SharedPtr& ptr) {
    value_ = ptr.value_;
    use_count_ = ptr.use_count_;
    weak_count_ = ptr.weak_count_;
  }

  void IncUseCount() {
    if (value_ != nullptr) {
      ++(*use_count_);
    }
  }

  T* value_;
  int* use_count_;
  int* weak_count_;
};

}  // namespace pointers

#endif  // SHARED_PTR_H_
