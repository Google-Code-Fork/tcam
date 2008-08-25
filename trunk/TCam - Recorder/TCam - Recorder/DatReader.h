#ifndef DATREADER_H
#define DATREADER_H

#include <iostream>
#include <list>
#include <string>
#include <stdio.h>
#include <windows.h>
#include "Globals.h"

using namespace std;

class CDatReader;

struct Object
{
	int id;

	bool GroundTile;
	bool Container;
	bool Stackable;
	bool Ladder;
	bool Useable;
	bool Rune;
	bool Fluid; // Can contain fluids
	bool Splash;
	bool Blocking; 
	bool Movable;
	bool ProjBlock; // Blocks projectiles such as Magicwalls or Walls
	bool CBlock; // blocks creatures from moving (parcels, ect...)
	bool Equipable; // Can pick it up
	bool Light; // Gives off light
};

enum Flags
{
	GroundTile = 0,
	Container,
	Stackable,
	Ladder,
	Useable,
	Rune,
	Fluid,
	Splash,
	Blocking, 
	Movable,
	ProjBlock,
	CBlock,
	Equipable,
	Light, 
};

class CDatReader
{

public:
	CDatReader(string fileName);
	~CDatReader();
	void Advance(int numBytes);
	void GetByte(int &myByte);
	void GetShort(int &myByte);
	bool GetObject(int objectId, int flag);

private:
	FILE *TibiaDat;
	char *data;
	list<Object> objectList;
	long byteOffset;
	long fileSize;
	int minId;
	int maxId;
};

#endif