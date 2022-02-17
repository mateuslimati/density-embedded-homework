/**
 * @file counter.hpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief Counter class headers.
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __COUNTER_HPP__
#define __COUNTER_HPP__

class Counter
{
public:
    explicit Counter(unsigned int value = 0);
    virtual ~Counter();

    unsigned int get_value();
    Counter &operator+=(const int num);
    Counter &operator-=(const int num);

private:
    unsigned int value;

};

#endif//__COUNTER_HPP__