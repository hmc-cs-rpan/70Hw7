/*
 * \file chunkystring.cpp
 * \authors Ricky Pan, Iris Liu
 * \brief Implementation of ChunkyString and its private classes
 */

#include "chunkystring.hpp"

#include <cassert>

ChunkyString::ChunkyString()
    : size_{0}
{
    std::list<Chunk> chunks;
    chunks_ = chunks;
}

ChunkyString::ChunkyString(const ChunkyString& orig)
{
    // initialize default values for a ChunkyString
    size_ = 0;
    std::list<Chunk> chunks;
    chunks_ = chunks;

    // pushes all of the elements in orig into our ChunkyString
    for(const_iterator i = orig.begin(); i != orig.end(); ++i)
    {
        push_back(*i);
    }
}

ChunkyString::iterator ChunkyString::begin() 
{
    return Iterator<false>(chunks_.begin(), 0);
}

ChunkyString::iterator ChunkyString::end() 
{
    return Iterator<false>(chunks_.end(), 0);
}

ChunkyString::const_iterator ChunkyString::begin() const
{
    return Iterator<true>(chunks_.begin(), 0);
}

ChunkyString::const_iterator ChunkyString::end() const
{
    return Iterator<true>(chunks_.end(), 0);
}

ChunkyString& ChunkyString::operator+=(const ChunkyString& rhs)
{
    ChunkyString deepCopy = ChunkyString(rhs);

    // push_back each char from the deepCopy of rhs
    for (iterator i = deepCopy.begin(); i != deepCopy.end(); ++i)
    {
        this->push_back(*i);
    }
    return *this;
}


void ChunkyString::push_back(char c)
{
    // adds a char c to the end of our ChunkyString
    if (size_ == 0 || chunks_.back().length_ == CHUNKSIZE)
    {
        // create a newChunk
        Chunk newChunk = Chunk(0, CHUNKSIZE);
        // push_back newChunk to our chunks_ list
        chunks_.push_back(newChunk);
        chunks_.back().chars_[0] = c;
        chunks_.back().length_ += 1;
    }
    else
    {
        // place in next available array index
        size_t nextInd = chunks_.back().length_;
        chunks_.back().chars_[nextInd] = c;
        chunks_.back().length_ += 1;
    }
    ++size_;    
}

size_t ChunkyString::size() const
{
    return size_;
}

bool ChunkyString::operator==(const ChunkyString& rhs) const
{
    if(size_ != rhs.size_)
    {
        return false;
    }

    // Initializes 1 iterator to loop through each ChunkyString
    const_iterator a = this->begin();
    const_iterator b = rhs.begin();

    for(size_t i = 0; i < size_; ++i)
    {
        if(*a != *b)
        {
            return false;
        }
        else
        {
            // increments iterators
            ++a;
            ++b;
        }
    }
    return true;
}

bool ChunkyString::operator!=(const ChunkyString& rhs) const
{
    // Idiomatic code: leverage == to implement !=
    return !(*this == rhs);
}

bool ChunkyString::operator<(const ChunkyString& rhs) const
{
    return std::lexicographical_compare(this->begin(), this->end(),
                                         rhs.begin(), rhs.end());
}

std::ostream& operator<<(std::ostream& out, 
    const ChunkyString& text)
{
    std::string toPrint = "";
    for(ChunkyString::const_iterator i = text.begin(); i != text.end(); ++i)
    {
        toPrint += *i;
    }
    out << toPrint;
    return out;
}

double ChunkyString::utilization() const
{
    return double(size_)/(chunks_.size()*CHUNKSIZE);
}

// ---------------------------------------------
// Implementation of ChunkyString::Chunk
// ---------------------------------------------
//
ChunkyString::Chunk::Chunk(size_t length, size_t CHUNKSIZE)
    : length_{0}
{
    char chars_[CHUNKSIZE]; 
}
