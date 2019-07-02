#include "stdafx.h"
#include "Emitter.h"
#include "../Item/ParticleItem.h"
#include "../Item/CommonItem/ParentOptionItem.h"
#include "../Item/CommonItem/SpawnItem.h"
#include "../Item/CommonItem/TransformItem.h"
Hashira::Emitter::Emitter() :
	_dataSize(0),
	_emitterData(),
	_emitterCommonItem(),
	_items(),
	_childEmitters(),
	_emtName("UNNAMED_EMITTER")
{
	_updater =
		[this]() 
	{
		
		for (auto& item : this->_items)
		{
			item->UpdateItem();
		}
	};

	AddItems<TransformItem>().AddItems<ParentOptionItem>().AddItems<SpawnItem>();
}


Hashira::Emitter::~Emitter()
{
	Discard();
}

void Hashira::Emitter::UpdateItems()
{
	_updater();
}

void Hashira::Emitter::SetCustomUpdater(std::function<void(void)> function)
{
	_updater = function;
}

void Hashira::Emitter::SetName(String & name)
{
	this->_emtName.swap(name);
}

const unsigned int & Hashira::Emitter::GetBinSize()
{
	return _dataSize;
}

Hashira::EmitterCommonItem & Hashira::Emitter::GetCommonItem()
{
	return _emitterCommonItem;
}

Hashira::EmitterData & Hashira::Emitter::GetEmitterData()
{
	return _emitterData;
}

std::vector<std::unique_ptr<Hashira::ParticleItem>>& Hashira::Emitter::GetEmitterItems()
{
	return  _items;
}

std::vector<Hashira::Emitter*>& Hashira::Emitter::GetChildren()
{
	return _childEmitters;
}

void Hashira::Emitter::Discard()
{
	_updater = []() {};
	_items.clear();
	_items.shrink_to_fit();
	_childEmitters.clear();
	_childEmitters.shrink_to_fit();
	_emitterData = EmitterData();
	_emitterCommonItem = EmitterCommonItem();
	_emtName = "DELETED_EMITTER";
	_dataSize = 0;

}
