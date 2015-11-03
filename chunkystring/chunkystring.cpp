/*
 * \file chunkystring.cpp
 * \authors Ricky Pan, Iris Liu
 * \brief Implementation of ChunkyString and its private classes
 */

#include "chunkystring.hpp"

#include <cstddef>
#include <cassert>

using namespace std;

ChunkyString::ChunkyString(): size_(0)
{
	chunks_ = std::list<Chunk>;
}

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
		if (this.back().length_ == 0)
		{
			delete this.back()
		}
		else
		{
			// decrease the length of the last chunk by one, leaving it behind
			// decrement size_
			--this.back().length_;
			--size_;
		}

	}
}

ChunkyString::iterator ChunkyString::begin() const
{
	// if empty, return nullptr
	if (size_ == 0)
	{

		return Iterator(nullptr);

	}
	else
	{
		// return pointer to first element of first Chunk
		return Iterator(this.front()[0]);
	}
	
}

ChunkyString::iterator ChunkyString::end() const
{
	// if empty, return nullptr
	if (size_ == 0)
	{
		return Iterator(nullptr);
	}
	else
	{
		// increment from the last char
		return Iterator(back().length_[-1]);
	}
	
}

// --------------------------------------------
// Implementation of ChunkyString::Iterator
// --------------------------------------------
//
ChunkyString::Iterator::Iterator(char* current)
	: curr_{current_}
{
	// begin, end, 2 chunks, 1 chunk
	
}





