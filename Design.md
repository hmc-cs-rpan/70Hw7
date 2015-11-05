We decided to stick with our idea of implementing ChunkyString from
the pseudocode we wrote last week. 

In particular, we chose to design ChunkyString as a std::list of 
Chunk struct so we could take advantage of the STL to iterate 
through the list of Chunks. Each Chunk kept track of its length and 
the array of characters it held in a standard char array. This 
would allow us to create an iterator that kept track of the Chunk 
by using the STL iterator, and an index to track the char in the 
array. We decided on a struct because the Chunk did not need any 
member functions. 

We designed push_back to add to the end of the ChunkyString without 
considering rebalancing for easier implementation.

Our insert function takes into account the Chunk immediately before 
and after it, and considers the utilization locally. We aim to 
insert a character if space exists, and insert a new Chunk whenever 
the Chunk is full, and reflow text only if inserting a new Chunk 
will cause the local utilization to fall under 1/2.


Erase looks only at the Chunk specified by the iterator, and 
deletes the specified character. It will erase by decrementing the 
length_ member and shifting characters if necessary. It will then 
invoke a reflow function that checks if utilization > 1/4.

Our reflow function combines adjacent Chunks if there is space. It 
iterates through the ChunkyString while the utilization < 1/4. 
After combining sufficient Chunks, it exits.