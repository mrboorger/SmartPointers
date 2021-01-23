#ifndef WEAK_PTR_H_
#define WEAK_PTR_H_

#include "shared_ptr.h"

namespace pointers {

template<class T>
class WeakPtr {
  friend class Tester;
 public:
  WeakPtr() = default;

  explicit WeakPtr(const SharedPtr<T>& shared_ptr)
      : value_(shared_ptr.value_), use_count_(shared_ptr.use_count_),
        weak_count_(shared_ptr.weak_count_) {
    IncWeakCount();
  }

  WeakPtr<T>(WeakPtr<T>&& weak_ptr) noexcept:
      value_(weak_ptr.value_),
      use_count_(weak_ptr.use_count_), weak_count_(weak_ptr.weak_count_) {
    weak_ptr.RemovePointers();
  }

  WeakPtr<T>(const WeakPtr<T>& weak_ptr) :
      value_(weak_ptr.value_),
      use_count_(weak_ptr.use_count_), weak_count_(weak_ptr.weak_count_) {
    IncWeakCount();
  }

  ~WeakPtr() {
    Reset();
  }

  WeakPtr<T>& operator=(WeakPtr<T>&& weak_ptr) noexcept {
    if (this != &weak_ptr) {
      Reset();
      CopyPointers(weak_ptr);
      weak_ptr.RemovePointers();
    }
    return *this;
  }

  WeakPtr<T>& operator=(const WeakPtr<T>& weak_ptr) {
    if (this != &weak_ptr) {
      Reset();
      CopyPointers(weak_ptr);
      IncWeakCount();
    }
    return *this;
  }

  T* Get() {
    return (Expired() ? nullptr : value_);
  }

  const T* Get() const {
    return (Expired() ? nullptr : value_);
  }

  void Reset() {
    if (value_ != nullptr) {
      --(*weak_count_);
      if (*weak_count_ == 0 && *use_count_ == 0) {
        delete use_count_;
        delete weak_count_;
      }
      RemovePointers();
    }
  }

  bool Expired() const {
    return (value_ == nullptr || *use_count_ == 0);
  }

  SharedPtr<T> Lock() const {
    SharedPtr<T> result;
    if (!Expired()) {
      ++(*use_count_);
      result.value_ = value_;
      result.use_count_ = use_count_;
      result.weak_count_ = weak_count_;
    }
    return result;
  }

 private:
  void RemovePointers() {
    value_ = nullptr;
    use_count_ = nullptr;
    weak_count_ = nullptr;
  }

  void IncWeakCount() {
    if (weak_count_ != nullptr) {
      ++(*weak_count_);
    }
  }

  void CopyPointers(const WeakPtr& ptr) {
    value_ = ptr.value_;
    use_count_ = ptr.use_count_;
    weak_count_ = ptr.weak_count_;
  }

  T* value_ = nullptr;
  int* use_count_ = nullptr;
  int* weak_count_ = nullptr;
};

}  // namespace pointers

#endif  // WEAK_PTR_H_
