/**
 * @file counter.cpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief Counter class methods.
 * @version 0.1
 * @date 2022-02-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <counter.hpp>
#include <limits>

/**
 * @brief Construct a new Counter:: Counter object
 *
 * @param[in] value
 */
Counter::Counter(unsigned int value) : value(value)
{
}

/**
 * @brief Destroy the Counter:: Counter object
 *
 */
Counter::~Counter()
{
}

/**
 * @brief Get value method
 *
 * @return unsigned int
 */
unsigned int Counter::get_value()
{
    return this->value;
}

/**
 * @brief Overload for iterator +=
 * 
 * @param[i] num that whill be add to value attribute
 * @return Counter& this object
 */
Counter &Counter::operator+=(const int num)
{
    Counter result = *this;
    if ((this->value + num) < __UINT32_MAX__)
    {
        this->value += num;
    }
    else
    {
        this->value = __UINT32_MAX__;
    }
    return *this;
}

/**
 * @brief[i] Overload for iterator -=
 * 
 * @param num that whill be subtraid to value attribute
 * @return Counter& 
 */
Counter &Counter::operator-=(const int num)
{
    Counter result = *this;
    if ((this->value - num) > 0)
    {
        this->value -= num;
    }
    else
    {
        this->value = 0;
    }
    return *this;
}