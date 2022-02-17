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
 * @param value
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
 * @brief Overload for iterator ++
 *
 * @return Counter
 */
Counter Counter::operator++(int)
{
    Counter result = *this;
    if (this->value < __UINT32_MAX__)
    {
        this->value++;
    }
    return *this;
}

/**
 * @brief Overload for iterator --
 *
 * @return Counter
 */
Counter Counter::operator--(int)
{
    Counter result = *this;
    if (this->value > 0)
    {
        this->value--;
    }
    return *this;
}