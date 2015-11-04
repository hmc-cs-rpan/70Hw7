/*
 * \file chunkystring.cpp
 * \authors Ricky Pan, Iris Liu
 * \brief Implementation of ChunkyString and its private classes
 */

#include "chunkystring.hpp"

#include <cstddef>
#include <cassert>

using namespace std;

ChunkyString::ChunkyString(): size_(0), front_(nullptr), back_()
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
	// if empty, return Iterator(nullptr,-1). This will be the convention
	// to describe and empty ChunkyString's iterator
	if(this.empty())
	{
		return Iterator(nullptr, -1);
	}
	else
	{
		// return pointer to first element of first Chunk
		return Iterator(this.front(), 0);
	}
	
}

ChunkyString::iterator ChunkyString::end() const
{
	// see begin() for empty case
	if(this.empty())
	{
		return Iterator(nullptr, -1);
	}
	else
	{
		// Iterator(nullptr, 0) will be the convention for the end()
		// iterator.
		return Iterator(nullptr, 0);
	}	
}

// --------------------------------------------
// Implementation of ChunkyString::Iterator
// --------------------------------------------
//
ChunkyString::Iterator::Iterator(Chunk* chunk, int index)
	: currChunk_{chunk}, charInd_{index}
{
	// Nothing to do here
}

ChunkyString::Iterator& ChunkyString::Iterator::operator++()
{
	// sets the iterator to point to the next char in the ChunkyString

	// case for iterator points to last char in Chunk
	if(charInd_ = currChunk_->length_-1)
	{
		// set iterator to point to first char of next Chunk
		// if iterator pointed to last char, it will be equal to the
		// end iterator
		currChunk_ = currChunk_.next();
		charInd_ = 0;
	}
	else
	{
		++charInd_;
	}
}

ChunkyString::Iterator& ChunkyString::Iterator::operator--()
{
	// sets the iterator to point to the previous char in ChunkyString

	// test for empty ChunkyString
	if(currChunk_ == nullptr)
	{
		if(charInd_ == -1)
		{
			cerr << "ChunkyString is empty!" << endl;
		}
		else
		{
			currChunk_ = 
		}
	}
	else if 
	// if curr_ = chars_.begin
	//		then prev chunk
}

char& ChunkyString::Iterator::operator*() const
{
	// Return the char curr_ points to
	return currChunk_->chars_[charInd_];
}

bool ChunkyString::Iterator::operator==(const Iterator& rhs) const
{
	// Checks if two iterators hold the same Chunk address and same
	// location within the array
	return currChunk_ == rhs.currChunk_ && charInd_ == rhs.charInd_;
}

bool ChunkyString::Iterator::operator!=(const Iterator& rhs) const
{
	// leverage == to implement !=
	return !(*this == rhs);
}



