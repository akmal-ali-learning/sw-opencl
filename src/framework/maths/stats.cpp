#include <vector>
#include <algorithm>
#include <assert.h>
#include <gtest/gtest.h>
template<typename T> 
T mean(const std::vector<T> & input )
{
    assert(!input.empty() );
    
    T total = 0;
    for(const auto& element : input)
    {
        total += element;
    }
    return total/input.size();
}

TEST(StatisticsTest, Mean)
{
    std::vector<double> numbers = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    ASSERT_EQ( 5.5, mean(numbers) );    
}