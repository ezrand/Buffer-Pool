#pragma once
#include "BufferBlockADT.h"
#include <iostream>
#include <string>



using namespace std;

class BufferBlock : public BufferblockADT {
private:
    //Instance variables:
    int blockID;
    char* block;
    int size;

public:

    //sz is the size of the character array data
    //points to.
    BufferBlock() { size = 4096; } //single argument constructor to set the amount of bytes
    BufferBlock(char* data, int id, int sz = 4096) //three argument constructor to set the id and actual information, also with the amount of bytes
    {
        block = data;
        blockID = id; 
        size = sz;
    }
    ~BufferBlock() {} //destructor

    //read the block from pos to pos + sz-1 (or to the end of the block)
    void getData(int pos, int sz, char* data)
    {
        //retrieves the data of the position read into this function
        //writes to block into data
        //in the for loop, for i less than size, or i + position is greater than blocksize, the loop will run
        for (int i = 0; (i < (sz)) || ((i + pos) > getBlocksize()); i++)
        {
            data[i] = block[i + pos];
        }
    }

    //setID
    void setID(int id)
    {
        blockID = id;
    }

    //getID
    int getID() const
    {
        return blockID;
    }

    //getBlocksize
    int getBlocksize() const
    {
        return size; 
    }

    //return the block
    char* getBlock() const
    {
        return block; 
    }

    //set the block
    void setBlock(char* blk)
    {
        //create the block
        block = new char[size];
        //for i less than size(4096) bytes, i will increment by one, setting the information into the new block created
        for (int i = 0; i < size; i++)
        {
            block[i] = blk[i];
        }
    }
};
