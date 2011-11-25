/*
Minetest-c55
Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef NODEMETADATA_HEADER
#define NODEMETADATA_HEADER

#include "irrlichttypes.h"
#include <string>
#include <iostream>

/*
	NodeMetadata stores arbitary amounts of data for special blocks.
	Used for furnaces, chests and signs.

	There are two interaction methods: inventory menu and text input.
	Only one can be used for a single metadata, thus only inventory OR
	text input should exist in a metadata.
*/

class Inventory;
class IGameDef;

class NodeMetadata
{
public:
	typedef NodeMetadata* (*Factory)(std::istream&, IGameDef *gamedef);
	typedef NodeMetadata* (*Factory2)(IGameDef *gamedef);

	NodeMetadata(IGameDef *gamedef);
	virtual ~NodeMetadata();
	
	static NodeMetadata* create(const std::string &name, IGameDef *gamedef);
	static NodeMetadata* deSerialize(std::istream &is, IGameDef *gamedef);
	void serialize(std::ostream &os);
	
	virtual u16 typeId() const = 0;
	virtual const char* typeName() const = 0;
	virtual NodeMetadata* clone(IGameDef *gamedef) = 0;
	virtual void serializeBody(std::ostream &os) = 0;
	virtual std::string infoText() {return "";}
	virtual Inventory* getInventory() {return NULL;}
	// This is called always after the inventory is modified, before
	// the changes are copied elsewhere
	virtual void inventoryModified(){}
	// A step in time. Returns true if metadata changed.
	virtual bool step(float dtime) {return false;}
	virtual bool nodeRemovalDisabled(){return false;}
	// Used to make custom inventory menus.
	// See format in guiInventoryMenu.cpp.
	virtual std::string getInventoryDrawSpecString(){return "";}
	// primarily used for locking chests, but others can play too
	virtual std::string getOwner(){ return std::string(""); }
	virtual void setOwner(std::string t){}
	virtual bool allowsTextInput(){ return false; }
	virtual std::string getText(){ return ""; }
	virtual void setText(const std::string &t){}
protected:
	static void registerType(u16 id, const std::string &name, Factory f,
			Factory2 f2);
	IGameDef *m_gamedef;
private:
	static core::map<u16, Factory> m_types;
	static core::map<std::string, Factory2> m_names;
};

/*
	List of metadata of all the nodes of a block
*/

class NodeMetadataList
{
public:
	~NodeMetadataList();

	void serialize(std::ostream &os);
	void deSerialize(std::istream &is, IGameDef *gamedef);
	
	// Get pointer to data
	NodeMetadata* get(v3s16 p);
	// Deletes data
	void remove(v3s16 p);
	// Deletes old data and sets a new one
	void set(v3s16 p, NodeMetadata *d);
	
	// A step in time. Returns true if something changed.
	bool step(float dtime);

private:
	core::map<v3s16, NodeMetadata*> m_data;
};

#endif

