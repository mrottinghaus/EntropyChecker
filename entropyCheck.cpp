//Entropy Checker
//Matt Rottinghaus
/*
Compile with:
g++ -Wall entropyChecker.cpp -o checkentropy

run with:
./checkentropy myfilename <threshold>
where threshold is a number. for a 64 byte block size, most data ranges between 2 and 4 bits of entropy.

*/
//!! Make block size smaller for more granularity. size is in bytes
//Pardon my shitty styling and indecisiveness between tabs and spaces...

#define BLOCK_SIZE 64
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int indx = 0;
int ucount = 0;
const float base = 0.693147180;
bool notCounted(char* uniquebuf, unsigned char item)
{
	bool ret = true;
	for(int i = 0; i < BLOCK_SIZE; i++)
	{
		if(item == uniquebuf[i])
		{
			ret = false;
			break;
		}
	}
	if(ret == true)
	{
		uniquebuf[indx] = item;
	}
	return ret;
}

int countInstances(char* buf, unsigned char item)
{
	int ret = 0;
	for(int i = 0; i < BLOCK_SIZE; i++)
	{
		if(item == buf[i])
		{
			ret++;
		}
	}
	return ret;
}

int main ( int argc, char *argv[] )
{
  char buffer[BLOCK_SIZE];
  unsigned char uniqueBytes = 0;
  int count = 0;
  char measureBuf[BLOCK_SIZE];
  float probs[BLOCK_SIZE];
  int blocksize = BLOCK_SIZE;
  int filesize = 0;
  float prob = 0;
  float entropy = 0;
  float entropyAv = 0;
  memset(buffer,0,1);
  memset(measureBuf,0,1);
  unsigned char ichar = 0;
  long int numread = 0;
  int numToRead;
  float entropyMin = 0;
  numToRead = blocksize;

  if ( argc < 2 ) // argc should be 2 for correct execution
    // We print argv[0] assuming it is the program name
    cout<<"usage: "<< argv[0] <<" <filename> <entropy min> <block size> \n";
  else 
  {
    // We assume argv[1] is a filename to open
    ifstream the_file ( argv[1],std::ios::binary );
    // Always check to see if file opening succeeded
    if ( !the_file.is_open() )
      cout<<"Could not open file\n";
    else 
    {
    	cout << "opened file!" << endl;
    	the_file.seekg(0, the_file.end);
      filesize = the_file.tellg();
      the_file.seekg(0, the_file.beg);
      if(filesize < numToRead)
      {
      	numToRead = filesize;
      	blocksize = filesize;
      }
      cout << "file size: " << filesize << endl;
      if(argc > 2)
      {
      	entropyMin = atof(argv[2]);
      }
      for(numread = 0; numread < filesize; numread += blocksize)
      {
      	the_file.read(buffer,numToRead);
      	//cout << "read file..." << endl;
        //Measure entropy
        //need number of different bytes. numBitsneeded = p(symbol) * log(p(sympol))
        for(int i = 0; i < blocksize; i++)
        {
        	if(notCounted(measureBuf,buffer[i]))
        	{
        		count = countInstances(buffer,measureBuf[indx]);
        		if(count > 0)
        		{
        		  probs[indx] = (float)count/blocksize;
        		  //printf("probability of %c = %f\n",buffer[i],probs[indx]);
        		  indx++;
        		}
        	}
        }
        for(int j = 0; j < indx; j++)
        {
        	//printf("probability: %f\n",probs[j]);
        	entropy += (probs[j] * (log(probs[j])/log(2)));
        }
        entropy = entropy * (-1);
        if(entropy > entropyMin)
        {
          cout << "Unique items: " << (indx+1) << endl;
      	  cout << "Entropy at " << numread << ": " << entropy << endl;
        }
        //store measurement
        //clear buffer
        entropy = 0;
        indx = 0;
        memset(probs,0,blocksize*sizeof(float));
        memset(buffer,0,blocksize);
        memset(measureBuf,0,blocksize);
      }
      // the_file.get ( x ) returns false if the end of the file
      //  is reached or an error occurs
      //while ( the_file.get ( x ) )
      // cout<< x;
    }
    // the_file is closed implicitly here
  }
  return 0;
}