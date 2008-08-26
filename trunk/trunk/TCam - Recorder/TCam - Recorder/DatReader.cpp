#include "DatReader.h"
#include <string>

/*************************************************************************************************
Constructor
*************************************************************************************************/
CDatReader::CDatReader(string fileName)
{

	byteOffset = 0; // Initialize variables
	fileSize = 0;
	minId = 100;
	maxId = 0;

	TibiaDat = fopen(fileName.c_str(),"rb"); // Set file handle
	fseek(TibiaDat,0,SEEK_END);	// Set pointer to end of file
	fileSize = ftell(TibiaDat); // Read the byte offset at to get file length in bytes
	fseek(TibiaDat,0,SEEK_SET); // Go back to beginning of file

	data = (char*) malloc (sizeof(char)*fileSize); // Allocate size to store the file in

	fread(data,1,fileSize,TibiaDat); // Read contents of DAT file to buffer
	fclose(TibiaDat); // close file

	Advance(4); // Advance 4 bytes to get maxId
	GetShort(maxId); // Get the number of items + 100 (since first item is ID:100)
	Advance(6);

	while(maxId > minId) // Loop through all items
	{
		Object *newObject = new Object; // Create new object
		newObject->id = minId; // Object ID (TileID)
		// Reset all variables
		newObject->GroundTile = false;
		newObject->Container = false;
		newObject->Stackable = false;
		newObject->Ladder = false;
		newObject->Useable = false;
		newObject->Rune = false;
		newObject->Fluid = false; // Can contain fluids
		newObject->Splash = false;
		newObject->Blocking = false; 
		newObject->Movable = true;
		newObject->ProjBlock = false; // Blocks projectiles such as Magicwalls or Walls
		newObject->CBlock = false; // blocks creatures from moving (parcels, ect...)
		newObject->Equipable = false; // Can pick it up
		newObject->Light = false; // Gives off light

		int flag; // flag
		do
		{
			flag = 0;
			GetByte(flag);

			switch(flag)
			{
			case 0x00: //is groundtile
				newObject->GroundTile = true;
				Advance(2);
				break;
			case 0x01: //Walkable
				break;
			case 0x02: //Walkable
				break;
			case 0x03: //Walkable
				break;
			case 0x04: //Container
				newObject->Container = true;
				break;
			case 0x05: //Stackable
				newObject->Stackable = true;
				break;
			case 0x06: //Ladder
				newObject->Ladder = true;
				break;
			case 0x07: //Useable
				newObject->Useable = true;
				break;
			case 0x08: //Rune
				newObject->Rune = true;
				break;
			case 0x09: //Writeable
				Advance(2); // 2 bytes for max length
				break;
			case 0x0A: //Writeable no edit
				Advance(2);
				break;
			case 0x0B: //Fluid
				newObject->Fluid = true;
				break;
			case 0x0C: //Fluid with state
				newObject->Splash = true;
				break;
			case 0x0D: //Blocking
				newObject->Blocking = true;
				break;
			case 0x0E: //Not Moveable
				newObject->Movable = false;
				break;
			case 0x0F: //Block Missiles
				newObject->ProjBlock = true;
				break;
			case 0x10: //Block Monsters
				newObject->CBlock = true;
				break;
			case 0x11: //Equipable
				newObject->Equipable = true;
				break;
			case 0x12: //Wall item
				break;
			case 0x13: // No idea...
				break;
			case 0x14: // No idea...
				break;
			case 0x15: //Rotateable
				break;
			case 0x16: //Light
				newObject->Light = true;
				Advance(4); // 2 bytes for color 2 for intensity
				break;
			case 0x17: // No object has this...
				break;
			case 0x18: //Floor Change
				break;
			case 0x19: //Draw Offset
				Advance(4);
				break;
			case 0x1A: // No idea...
				Advance(2);
				break;
			case 0x1B:// No idea...
				break;
			case 0x1C: // No idea...
				break;
			case 0x1D:  // Minimap color
				Advance(2); // Color
				break;
			case 0x1E: // Floor change?
				Advance(2);
				break;
			case 0x1F:
				break;
			case 0xFF:
				break;
			default: 
				break;			 
			}
			//Advance(1);
		}while(flag != 0xFF);


		int width = 0,height = 0,numFrames = 0,xDiv = 0,yDiv = 0,zDiv = 0,animLen = 0,numSprites = 0;
		GetByte(width);  // Needed to calculate numSprites
		GetByte(height);
		if((width > 1) || (height > 1))
		{
			Advance(1); // No idea...
		}

		GetByte(numFrames); // Number of frames 
		GetByte(xDiv);
		GetByte(yDiv);
		GetByte(zDiv);
		GetByte(animLen); // Length of animation

		numSprites = width * height * numFrames * xDiv * yDiv * zDiv * animLen; // Get advance number

		Advance(numSprites * 2); // Advance (*2 because they're all shorts instead of bytes)

		objectList.push_back(*newObject); // Append new object ot list
		delete newObject; // Delete object to avoid memory leaks

		minId++; // Next...

	}
}

/*************************************************************************************************
Destructor
*************************************************************************************************/
CDatReader::~CDatReader()
{
} 

/*************************************************************************************************
Function name: Advance
Purpose: Advances the offset count
Comments:
Return: Void
*************************************************************************************************/
void CDatReader::Advance(int numBytes)
{
	if(byteOffset + numBytes > fileSize)
	{
		byteOffset = fileSize;
		return;
	}
	byteOffset += numBytes; // Increment bufferOffset
}

/*************************************************************************************************
Function name: GetByte
Purpose: Reads a byte from the buffer
Comments:
Return: Void
*************************************************************************************************/
void CDatReader::GetByte(int &myByte)
{
	//Error check!!!
	memcpy(&myByte,&data[byteOffset],1);
	byteOffset++;
}

/*************************************************************************************************
Function name: GetShort
Purpose: Reads a short from the buffer
Comments:
Return: Void
*************************************************************************************************/
void CDatReader::GetShort(int &myByte)
{
	//Error check!!!
	memcpy(&myByte,&data[byteOffset],2);
	byteOffset+=2;
}

/*************************************************************************************************
Function name: GetObject
Purpose: Gets an Objects flag
Comments:
Return: True if flag is there, false otherwise
*************************************************************************************************/
bool CDatReader::GetObject(int objectId, int flag)
{
	list<Object>::iterator i = objectList.begin(); // Beginning of list

	while (i != objectList.end()) // Loop
	{
		if (i->id == objectId) // If correct Id
		{
			
			switch(flag)
			{
			case 0:
				if(i->GroundTile == true)
					return true;
				break;
			case 1:
				if(i->Container == true)
					return true;
				break;
			case 2:
				if(i->Stackable == true)
					return true;
				break;
			case 3:
				if(i->Ladder == true)
					return true;
				break;
			case 4:
				if(i->Useable == true)
					return true;
				break;
			case 5:
				if(i->Rune == true)
					return true;
				break;
			case 6:
				if(i->Fluid == true)
					return true;
				break;
			case 7:
				if(i->Splash == true)
					return true;
				break;
			case 8:
				if(i->Blocking == true)
					return true;
				break;
			case 9:
				if(i->Movable == true)
					return true;
				break;
			case 10:
				if(i->ProjBlock == true)
					return true;
				break;
			case 11:
				if(i->CBlock == true)
					return true;
				break;
			case 12:
				if(i->Equipable == true)
					return true;
				break;
			case 13:
				if(i->Light == true)
					return true;
				break;
			default:
				return false;
				break;
			}
			return false;
		}
		++i;
	}
	return false;
}