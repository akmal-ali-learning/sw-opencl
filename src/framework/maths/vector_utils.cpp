#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>

template<typename T>
std::vector<T>
reverse_vector(const std::vector<T>& input)
{
    std::vector<T> copy = input;
    std::reverse(copy.begin(), copy.end() );
    return copy;
}

GTEST_TEST(VectorUtilsTest, Reverse) 
{
    std::cout<< "Running VectorUtilsTest"<<std::endl;
    
    std::vector<int> a = {0,1,2,3,4,5,6,7,8,9};
    std::vector<int> b = {9,8,7,6,5,4,3,2,1,0};
    ASSERT_EQ(a.size(), b.size());
    auto reversed_a = reverse_vector(a); 
    ASSERT_EQ(a.size(), reversed_a.size());
    for(size_t i =0 ; i < reversed_a.size(); ++i)
    {
        ASSERT_EQ(b[i], reversed_a[i]);
    }
}


