#include "BufferPoolADT.h"
#include "BufferBlockIMP.h" 
#include <string> 
#include <fstream> 
#pragma once

class LRUBufferPool : public BufferPoolADT {
private:
	BufferBlock* blck;
	fstream inFile;
	string fName;
public:
	LRUBufferPool(string filename, int poolSize = 5, int blockSize = 4096) //LRU buffer constructor
	{
		//creating a new block, of pool size(5)
		blck = new BufferBlock[POOL_SIZE]();
		fName = filename;

		//opening the file 
		inFile.open(fName.c_str(), fstream::in | fstream::binary);

		//for loop, increments backwards to set the block of data and its id
		for (int i = (POOL_SIZE - 1); i >= 0; i--)
			{
				//create the temporary block of data
				char* data = new char[BLOCKSIZE];


				//find and read in the block of data
				inFile.seekg(i* BLOCKSIZE);
				inFile.read(data, BLOCKSIZE);
				
				
				//set the data and id
				blck[i].setBlock(data);
				blck[i].setID(i);
			}
		inFile.close();
	}
	~LRUBufferPool() //destructor, delete the block, and set block to NULL to prevent a hanging pointer
	{
		delete[] blck;
		blck = NULL;
	}
	// Copy "sz" bytes from position "pos" of the buffered
	//   storage to "space".
	void getBytes(char* space, int sz, int pos)
	{
		//instance variables
		//num is the current ID of the block, it is used to find the correct position to read data from 
		int num = (pos / BLOCKSIZE);

		//exist is the bool variable to check if the block exists already
		bool exist = false;

		//current position(cPos) is set to i when a block is found, allowing it to be swapped to its new position
		int cPos;

		//check if block exists
		for (int i = 0; i < POOL_SIZE; i++)
		{
			if (blck[i].getID() == num)
			{
				//if the block exists, set exist to true and current Position to i, break loop to move to if else statement
				exist = true;
				cPos = i;
				break;
			}
		}
		//if block exists
		if (exist)
		{
			//create a temporary block to read the block into
			BufferBlock temporary(blck[cPos].getBlock(), BLOCKSIZE);
			temporary.setID(blck[cPos].getID());


			//move the blocks above the existing block down
			moveBlocks(cPos); 


			//set the tempoary block to 0 since it is the most recently accessed block
			blck[0].setBlock(temporary.getBlock());
			blck[0].setID(temporary.getID());
		}
		//else if block does not exist
		else
		{
			//create temporary char array read the information into
			char* temp = new char[BLOCKSIZE];
			//find and read into the correct spot in the file
			inFile.open(fName.c_str(), fstream::in | fstream::binary);
			inFile.seekg(num * BLOCKSIZE);
			inFile.read(temp, BLOCKSIZE);


			//setting the block to the least recently used in the pool
			blck[POOL_SIZE - 1].setBlock(temp);
			blck[POOL_SIZE - 1].setID(num);
			//temp set to NULL to prevent hanging pointer
			temp = NULL;

			//same logic from above, create a temporary block to read the block into
			BufferBlock temporary(blck[POOL_SIZE - 1].getBlock(), BLOCKSIZE);
			temporary.setID(blck[POOL_SIZE - 1].getID());


			//move the blocks above the new block down, in this case the whole list moves
			moveBlocks(POOL_SIZE - 1);


			//set the tempoary block to 0 since it is the most recently accessed block
			blck[0].setBlock(temporary.getBlock());
			blck[0].setID(temporary.getID());
			inFile.close(); 
		}
		//getData for the most recently accessed block 
		blck[0].getData((pos % BLOCKSIZE), sz, space);
}
	
	//helper function to move the blocks to the correct position
	void moveBlocks(int b)
	{
		//a increments down from position the read in
		for (int a = b; a > 0; a--)
		{
			blck[a].setBlock(blck[a - 1].getBlock());
			blck[a].setID(blck[a - 1].getID());
		}
	}

	// Print the order of the buffer blocks using the block id
	//	 numbers.
	void printBufferBlockOrder() 
	{
		//shows the current order of the list
		cout << "My buffer block order from most recently used to LRU is: " << endl; 
		//runs a for loop to get the blocks ID 
		for (int i = 0; i < POOL_SIZE; i++)
		{
			cout << blck[i].getID() << "  ";
		}
		cout << endl; 
	}

	// Get the block id number of the least recently used 
	//	 buffer block.
	int getLRUBlockID() 
	{
		//return the blocks id
		return blck[POOL_SIZE - 1].getID();
	}
};



