#include "weak_ptr.h"

#include <gmock/gmock.h>

namespace pointers {

class Tester {
 public:
  static void WConstructorsTests() {
    {
      int* value = new int(3);
      SharedPtr<int> ptr_s(value);
      const WeakPtr<int> ptr_w(ptr_s);
      EXPECT_EQ(ptr_w.value_, value);
      EXPECT_EQ(*(ptr_w.use_count_), 1);
      EXPECT_EQ(*(ptr_w.weak_count_), 1);
      EXPECT_EQ(ptr_w.value_, ptr_s.value_);
      EXPECT_EQ(ptr_w.use_count_, ptr_s.use_count_);
      EXPECT_EQ(ptr_w.weak_count_, ptr_s.weak_count_);

      WeakPtr<int> ptr_w_2(ptr_w);
      EXPECT_EQ(*(ptr_w_2.use_count_), 1);
      EXPECT_EQ(*(ptr_w_2.weak_count_), 2);
      EXPECT_EQ(ptr_w.value_, ptr_w_2.value_);
      EXPECT_EQ(ptr_w.use_count_, ptr_w_2.use_count_);
      EXPECT_EQ(ptr_w.weak_count_, ptr_w_2.weak_count_);

      WeakPtr<int> ptr_w_3(std::move(ptr_w_2));
      EXPECT_EQ(*(ptr_w_3.use_count_), 1);
      EXPECT_EQ(*(ptr_w_3.weak_count_), 2);
      EXPECT_EQ(ptr_w.value_, ptr_w_3.value_);
      EXPECT_EQ(ptr_w.use_count_, ptr_w_3.use_count_);
      EXPECT_EQ(ptr_w.weak_count_, ptr_w_3.weak_count_);

      ptr_s.Reset();
      EXPECT_EQ(*(ptr_w.use_count_), 0);
      EXPECT_EQ(*(ptr_w.weak_count_), 2);
      EXPECT_TRUE(ptr_w.Get() == nullptr);
      EXPECT_EQ(ptr_w_2.Get(), nullptr);
    }
  }

  static void WResetGetTests() {
    {
      int* value = new int(3);
      SharedPtr<int> ptr_s(value);
      WeakPtr<int> ptr_w(ptr_s);
      ptr_w.Reset();
      EXPECT_EQ(ptr_w.Expired(), true);
      EXPECT_EQ(ptr_w.Get(), nullptr);
    }
    {
      int* value = new int(3);
      SharedPtr<int> ptr_s(value);
      WeakPtr<int> ptr_w(ptr_s);
      ptr_s.Reset();
      EXPECT_EQ(*ptr_w.weak_count_, 1);
      EXPECT_EQ(*ptr_w.use_count_, 0);
      EXPECT_EQ(ptr_s.weak_count_, nullptr);
      EXPECT_EQ(ptr_w.Expired(), true);
      EXPECT_EQ(ptr_w.Get(), nullptr);
    }
    {
      int* value = new int(3);
      SharedPtr<int> ptr_s(value);
      WeakPtr<int> ptr_w(ptr_s);
      WeakPtr<int> ptr_w_2(ptr_w);
      EXPECT_EQ(ptr_w.Expired(), false);
      EXPECT_EQ(ptr_w.Get(), value);
      EXPECT_EQ(ptr_w_2.Expired(), false);
      EXPECT_EQ(ptr_w_2.Get(), value);
      EXPECT_EQ(*ptr_s.weak_count_, 2);
      ptr_w.Reset();
      EXPECT_EQ(ptr_w.Expired(), true);
      EXPECT_EQ(ptr_w.Get(), nullptr);
      EXPECT_EQ(ptr_w_2.Expired(), false);
      EXPECT_EQ(ptr_w_2.Get(), value);
      EXPECT_EQ(*ptr_s.weak_count_, 1);
      auto ptr_s_2 = ptr_w_2.Lock();
      EXPECT_EQ(ptr_s.value_, ptr_s_2.value_);
      EXPECT_EQ(ptr_s.use_count_, ptr_s_2.use_count_);
      EXPECT_EQ(ptr_s.weak_count_, ptr_s_2.weak_count_);
      EXPECT_EQ(*ptr_s_2.use_count_, 2);
      ptr_s_2.Reset();

      ptr_w_2.Reset();
      EXPECT_EQ(ptr_w_2.Expired(), true);
      EXPECT_EQ(ptr_w_2.Get(), nullptr);
      EXPECT_EQ(*ptr_s.weak_count_, 0);
      ptr_s_2 = ptr_w_2.Lock();
      EXPECT_EQ(ptr_s_2.value_, nullptr);
      EXPECT_EQ(ptr_s_2.use_count_, nullptr);
      EXPECT_EQ(ptr_s_2.weak_count_, nullptr);
    }
  }

  static void WAssignmentOperators() {
    {
      int* value = new int(3);
      SharedPtr<int> ptr_s(value);
      WeakPtr ptr_w(ptr_s);
      WeakPtr ptr_w_2(ptr_s);
      WeakPtr<int> ptr_w_3;

      EXPECT_EQ((ptr_w_3 = ptr_w_2).value_, value);
      EXPECT_EQ(ptr_w_3.use_count_, ptr_s.use_count_);
      EXPECT_EQ(*(ptr_w_3.use_count_), 1);
      EXPECT_EQ(ptr_w_3.weak_count_, ptr_s.weak_count_);
      EXPECT_EQ(*(ptr_w_3.weak_count_), 3);

      ptr_w_3 = std::move(ptr_w_2);
      EXPECT_EQ(ptr_w_3.value_, value);
      EXPECT_EQ(*(ptr_w_3.weak_count_), 2);
      EXPECT_EQ(ptr_w_2.value_, nullptr);
      EXPECT_EQ(ptr_w_2.use_count_, nullptr);
      EXPECT_EQ(ptr_w_2.weak_count_, nullptr);

    }
  }
};

TEST(WeakPtr, ConstructorsTests) {
  Tester::WConstructorsTests();
}

TEST(WeakPtr, ResetTests) {
  Tester::WResetGetTests();
}

TEST(WeakPtr, AssignmentOperators) {
  Tester::WAssignmentOperators();
}


}
