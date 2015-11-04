/*
 * \file chunkystring.cpp
 * \authors Ricky Pan, Iris Liu
 * \brief Implementation of ChunkyString and its private classes
 */

#include "chunkystring.hpp"

#include <cstddef>
#include <cassert>
#include <list>

using namespace std;

#define CHUNKSIZE 12

// ChunkyString::ChunkyString()
// 	: size_{0}
// {
// 	chunks_ = std::list<Chunk>;
// }

ChunkyString::ChunkyString(const ChunkyString& orig)
{
	// initialize default values for a ChunkyString
	size_ = 0;
	chunks_ = std::list<Chunk>;

	// pushes all of the elements in orig into our ChunkyString
	for(Iterator i = orig.begin(); i != orig.end(); ++i)
	{
		push_back(*i);
	}
}

ChunkyString::~ChunkyString()
{
	// delete empty chunks after removing each char from the chunk's char[]
	while(size_ != 0)
	{
		if (chunks_.back().length_ == 0)
		{
			delete chunks_.back()
		}
		else
		{
			// decrease the length of the last chunk by one, leaving it behind
			// decrement size_
			--chunks_.back().length_;
			--size_;
		}

	}
}

ChunkyString::iterator ChunkyString::begin() 
{
	return Iterator(chunks_.begin(), 0);
}

ChunkyString::iterator ChunkyString::end() 
{
	return Iterator(chunks_.end(), 0)	
}

ChunkyString::const_iterator ChunkyString::begin() const
{
	return Iterator(chunks_.begin(), 0);
}

ChunkyString::const_iterator ChunkyString::end() const
{
	return Iterator(chunks_.end(), 0)	
}

ChunkyString& ChunkyString::operator+=(const ChunkyString& rhs)
{
	ChunkyString deepCopy = ChunkyString(rhs);

	// push_back each char from the deepCopy of rhs
	for (Iterator i = deepCopy.begin(); i != deepCopy.end(); ++i)
	{
		this.push_back(*i);
	}

	// prevent memory leak
	delete deepCopy;

	return *this;
}


void ChunkyString::push_back(char c)
{
	// adds a char c to the end of our ChunkyString
	if (size_ == 0 || chunks_.back().length_ == CHUNKSIZE)
	{
		// create a newChunk
		Chunk newChunk = Chunk(0, chars_[CHUNKSIZE]);
		// push_back newChunk to our chunks_ list
		chunks_.(std::list::push_back(newChunk));
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

bool ChunkyString::operator==(const ChunkyString& rhs) const
{
	if(size_ != rhs.size_)
	{
		return false;
	}

	// Initializes 1 iterator to loop through each ChunkyString
	Iterator a = this.begin();
	Iterator b = rhs.begin();

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
	return std::lexicographical_compare(this.begin(), this.end(),
										 rhs.begin(), rhs.end());
}
