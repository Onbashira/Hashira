#include "ModelObject.h"
#include "Engine/Source/Mesh/MeshHeap.h"
#include "Engine/Source/Mesh/MeshBuffer.h"

K3D::ModelObject::ModelObject()
{

}


K3D::ModelObject::~ModelObject()
{
	Discard();
}

void K3D::ModelObject::Discard()
{
	this->_meshData->meshBuffer->Discard();
	this->_meshData->meshHeap->Discard();
}