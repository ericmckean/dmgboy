/*
 This file is part of gbpablog.
 
 gbpablog is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 gbpablog is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with gbpablog.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CARTRIDGE_H__
#define __CARTRIDGE_H__

#include <string>
#include "Def.h"
#include "MBC.h"

#define CART_NAME		0x0134
#define CART_TYPE		0x0147
#define CART_ROM_SIZE	0x0148
#define CART_RAM_SIZE	0x0149

class Cartridge
{
private:
	unsigned long _RomSize;
	bool _isLoaded;
	BYTE * _memCartridge;

	BYTE (*ptrRead)(WORD);
	void (*ptrWrite)(WORD, BYTE);
	void CheckCartridge();
	int CheckRomSize(int numHeaderSize, int fileSize);
public:
	char nameROM[17];
	Cartridge(std::string path);
	Cartridge(BYTE * cartridgeBuffer, unsigned long size);
	~Cartridge();
	void Print(int beg, int end);
	BYTE *GetData();
	unsigned int GetSize();
	inline BYTE Read(WORD direction) { return ptrRead(direction); };
	inline void Write(WORD direction, BYTE value) { ptrWrite(direction, value); };
	bool IsLoaded();
};

#endif
