#include <gtest/gtest.h>

#include <ATen/cpu/vec256/vec256.h>
#include <ATen/ATen.h>

#include <c10/core/CPUCachingAllocator.h>

TEST(CPUCachingAllocatorTest, check_alloc_free) {
  c10::WithCPUCachingAllocatorGuard cachine_allocator_guard;
  at::Tensor a = at::rand({23, 23});
  float* data_ptr = a.data_ptr<float>();
  a.reset();
  a = at::rand({23, 23});
  ASSERT_TRUE(data_ptr == a.data_ptr<float>());
}

// This should just free the pointer correctly.
TEST(CPUCachingAllocatorTest, check_alloc_outside_free_inside) {
  at::Tensor a = at::rand({23, 23});
  {
    c10::WithCPUCachingAllocatorGuard cachine_allocator_guard;
    float* data_ptr = a.data_ptr<float>();
    a.reset();
    a = at::rand({23, 23});
  }
}

TEST(CPUCachingAllocatorTest, check_alloc_inside_free_outside) {
  at::Tensor a;
  {
    c10::WithCPUCachingAllocatorGuard cachine_allocator_guard;
    a = at::rand({23, 23});
  }
  a.reset();
  a = at::rand({23, 23});
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  at::manual_seed(42);
  return RUN_ALL_TESTS();
}