/**
 * @file exceptions.cpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief File that contents the exceptions methods implementations.
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
 * @param[in] description Exception description
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