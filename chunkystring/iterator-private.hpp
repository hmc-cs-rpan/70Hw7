/*********************************************************************
 * ChunkyString::Iterator class.
 *********************************************************************
 *
 * Implementation for the templated ChunkyString iterator
 *
 */

#include <stdexcept>

template <bool const_it>
ChunkyString::Iterator<const_it>::Iterator()
{
    throw std::logic_error("Not yet implemented");
}

template <bool const_it>
ChunkyString::Iterator<const_it>::Iterator(const Iterator<false>& i)
{
    throw std::logic_error("Not yet implemented");
}

template <bool const_it>
ChunkyString::Iterator<const_it>& ChunkyString::Iterator<const_it>::operator++()
{
    throw std::logic_error("Not yet implemented");
}

template <bool const_it>
ChunkyString::Iterator<const_it>& ChunkyString::Iterator<const_it>::operator--()
{
    throw std::logic_error("Not yet implemented");
}

template <bool const_it>
typename ChunkyString::Iterator<const_it>::reference 
    ChunkyString::Iterator<const_it>::operator*() const
{
    throw std::logic_error("Not yet implemented");
}

template <bool const_it>
bool ChunkyString::Iterator<const_it>::operator==(const Iterator& rhs) const
{
    throw std::logic_error("Not yet implemented");    
}

template <bool const_it>
bool ChunkyString::Iterator<const_it>::operator!=(const Iterator& rhs) const
{
    throw std::logic_error("Not yet implemented");
}

