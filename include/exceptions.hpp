/**
 * @file exceptions.hpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief File that contents the exception class declaration.
 * @version 0.1
 * @date 2022-02-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __EXCEPTIONS_HPP__
#define __EXCEPTIONS_HPP__

#include <exception>

#define FILE_LINE_STR__(line) #line
#define FILE_LINE_STR(line) FILE_LINE_STR__(line)

#define EXCEPTION_MSG(string_param)  (__FILE__ ":"  FILE_LINE_STR(__LINE__) " - " string_param)

/** Basic type of Exception */
class Exception : public std::exception
{
private:
    /** Exception Description */
    const char *m_description;
public:
    explicit Exception(const char *description = "Not especified");
    virtual ~Exception();

    virtual const char* what() const throw()
    {
        return m_description;
    }

};

#endif //__EXCEPTIONS_HPP__

/** @} */
