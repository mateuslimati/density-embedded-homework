/**
 * @file exceptions.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <exceptions.hpp>

/**
 * @brief Construct a new Exception:: Exception object
 *
 * @param description
 */
Exception::Exception(const char *description) : 
    m_description{description}
{
}

/**
 * @brief Destroy the Exception:: Exception object
 *
 */
Exception::~Exception()
{
}