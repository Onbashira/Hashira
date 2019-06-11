#include "stdafx.h"
#include "Emitter.h"
#include "../Item/ParticleItem.h"

Hashira::Emitter::Emitter() : 
	_updater([this]() {for (auto& item : this->_items) { item->UpdateItem(); }}),
	_dataSize(sizeof(EmitterCommonItem))
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

void Hashira::Emitter::SetEmitterParam(const EmitterParam & emitterData)
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
}
