#include "stdafx.h"
#include "Emitter.h"
#include "../Item/ParticleItem.h"

Hashira::Emitter::Emitter() :
	_dataSize(sizeof(EmitterCommonItem)),
	_updater([this]() {for (auto& item : this->_items) { item->UpdateItem(); }}),
	_emitterData(),
	_emitterCommonItem(),
	_items(),
	_childEmitters(),
	_emtName("UNNAMED_EMITTER")
{

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

void Hashira::Emitter::SetEmitterData(const EmitterData & emitterData)
{
	_emitterData = emitterData;
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
}
