#include "shared_ptr.h"

#include <gmock/gmock.h>

namespace pointers {

class Tester {
 public:
  class TrickyClassOne {
   public:
    TrickyClassOne() = default;

    int Get() {
      return value_;
    }
   private:
    int value_ = 1;
  };
  static void SConstructorsTests() {
    {
      const SharedPtr<std::string> ptr;
      EXPECT_EQ(ptr.value_, nullptr);
      EXPECT_EQ(ptr.use_count_, nullptr);
      EXPECT_EQ(ptr.weak_count_, nullptr);
    }
    {
      int* value1 = new int(3);
      const SharedPtr ptr1(value1);
      EXPECT_EQ(ptr1.value_, value1);
      EXPECT_EQ(*(ptr1.use_count_), 1);
      EXPECT_EQ(*(ptr1.weak_count_), 0);

      SharedPtr ptr2(ptr1);
      EXPECT_EQ(ptr2.value_, value1);
      EXPECT_EQ(*(ptr2.use_count_), 2);
      EXPECT_EQ(*(ptr2.weak_count_), 0);
      {
        SharedPtr ptr3(std::move(ptr2));
        EXPECT_EQ(ptr2.value_, nullptr);
        EXPECT_EQ(ptr2.use_count_, nullptr);
        EXPECT_EQ(ptr2.weak_count_, nullptr);
        EXPECT_EQ(ptr3.value_, value1);
        EXPECT_EQ(*(ptr3.use_count_), 2);
        EXPECT_EQ(*(ptr3.weak_count_), 0);
      }
      EXPECT_EQ(ptr1.value_, value1);
      EXPECT_EQ(*(ptr1.use_count_), 1);
      EXPECT_EQ(*(ptr1.weak_count_), 0);
    }
  }

  static void SGetResetTests() {
    {
      int* value = new int(3);
      SharedPtr ptr(value);
      ptr.Reset();
      EXPECT_EQ(ptr.value_, nullptr);
      EXPECT_EQ(ptr.use_count_, nullptr);
      EXPECT_EQ(ptr.weak_count_, nullptr);
    }
    {
      int* value1 = new int(3);
      SharedPtr ptr1(value1);
      const SharedPtr ptr2(ptr1);
      EXPECT_EQ(ptr1.Get(), value1);
      EXPECT_EQ(ptr2.Get(), value1);
      EXPECT_EQ(*ptr1, *value1);
      EXPECT_EQ(*ptr2, *value1);
      ptr1.Reset();
      EXPECT_EQ(ptr1.Get(), nullptr);
      ASSERT_DEATH(*ptr1, ".*");
    }
    {
      auto* value = new TrickyClassOne;
      SharedPtr ptr(value);
      EXPECT_EQ(ptr->Get(), 1);
      ptr.Reset();
      ASSERT_DEATH(ptr->Get(), ".*");
    }
  }

  static void SAssignmentOperatorsTests() {
    {
      SharedPtr ptr1(new int(3));
      SharedPtr<int> ptr2;
      ptr1 = ptr2;
      EXPECT_EQ(ptr2.value_, nullptr);
      EXPECT_EQ(ptr2.use_count_, nullptr);
      EXPECT_EQ(ptr2.weak_count_, nullptr);
    }
    {
      int* value1 = new int(3);
      int* value2 = new int(4);
      SharedPtr ptr1(value1);
      SharedPtr ptr2(ptr1);
      SharedPtr ptr3(value2);
      ptr2 = ptr3;
      EXPECT_EQ(ptr1.value_, value1);
      EXPECT_EQ(*(ptr1.use_count_), 1);
      EXPECT_EQ(*(ptr1.weak_count_), 0);
      EXPECT_EQ(ptr2.value_, value2);
      EXPECT_EQ(*(ptr2.use_count_), 2);
      EXPECT_EQ(*(ptr2.weak_count_), 0);

      ptr1 = std::move(ptr2);
      EXPECT_EQ(ptr2.value_, nullptr);
      EXPECT_EQ(ptr2.use_count_, nullptr);
      EXPECT_EQ(ptr2.weak_count_, nullptr);
      EXPECT_EQ(ptr1.value_, value2);
      EXPECT_EQ(*(ptr1.use_count_), 2);
      EXPECT_EQ(*(ptr1.weak_count_), 0);
      EXPECT_EQ(ptr3.value_, value2);
      EXPECT_EQ(*(ptr3.use_count_), 2);
      EXPECT_EQ(*(ptr3.weak_count_), 0);
    }
    {
      int* value1 = new int(3);
      int* value2 = new int(4);
      SharedPtr ptr1(value1);
      SharedPtr ptr2(ptr1);
      SharedPtr ptr3(value2);
      ptr2 = std::move(ptr3);
      EXPECT_EQ(ptr1.value_, value1);
      EXPECT_EQ(*(ptr1.use_count_), 1);
      EXPECT_EQ(*(ptr1.weak_count_), 0);
      EXPECT_EQ(ptr2.value_, value2);
      EXPECT_EQ(*(ptr2.use_count_), 1);
      EXPECT_EQ(*(ptr2.weak_count_), 0);
      EXPECT_EQ(ptr3.value_, nullptr);
      EXPECT_EQ(ptr3.use_count_, nullptr);
      EXPECT_EQ(ptr3.weak_count_, nullptr);

      ptr1 = ptr2;
      EXPECT_EQ(ptr1.value_, value2);
      EXPECT_EQ(*(ptr1.use_count_), 2);
      EXPECT_EQ(*(ptr1.weak_count_), 0);
    }
  }

  static void EqualityOperatorsTests() {
    {
      int* value1 = new int(3);
      int* value2 = new int(3);
      SharedPtr<int> ptr0;
      const SharedPtr ptr1(value1);
      SharedPtr ptr2(value2);
      SharedPtr ptr3(ptr1);
      EXPECT_TRUE(ptr1 != ptr2);
      EXPECT_TRUE(ptr2 != ptr0);
      EXPECT_FALSE(ptr1 == ptr2);
      EXPECT_FALSE(ptr2 == ptr0);

      EXPECT_TRUE(ptr1 == value1);
      EXPECT_TRUE(ptr2 == value2);
      EXPECT_TRUE(ptr0 == nullptr);
      EXPECT_TRUE(value1 == ptr1);
      EXPECT_TRUE(value2 == ptr2);
      EXPECT_TRUE(nullptr == ptr0);

      EXPECT_TRUE(ptr3 == ptr2);
      EXPECT_FALSE(ptr3 != ptr2);
    }
  }
};

TEST(SharedPtr, ConstructorsTests) {
  Tester::SConstructorsTests();
}

TEST(SharedPtr, SGetTests) {
  Tester::SGetResetTests();
}

TEST(SharedPtr, AssignmentOperatorsTests) {
  Tester::SAssignmentOperatorsTests();
}

TEST(SharedPtr, EqualityOperatorsTests) {
  Tester::SAssignmentOperatorsTests();
}

}
