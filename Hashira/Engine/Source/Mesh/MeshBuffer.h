#pragma once

#include "Engine/Source/Resource/VertexBuffer.h"
#include "Engine/Source/Resource/IndexBuffer.h"
#include "Engine/Source/Command/State/GeometryState.h"

namespace Hashira {

	class MeshBuffer
	{


	private:

		std::vector<std::unique_ptr < VertexBuffer>> _vertexBuffers;

		std::unique_ptr < IndexBuffer > _indexBuffer;

		std::unique_ptr<GeometryState> _geometryState;

	public:

		MeshBuffer();

		~MeshBuffer();

		void CreateIndexBuffer(size_t elementSize, size_t elementNum, void * pSrc);

		void AddVertexBuffer(size_t elementSize, size_t elementNum, void * pSrc);

		const std::unique_ptr<Hashira::GeometryState>& GetGeometryState();

		const std::unique_ptr < Hashira::IndexBuffer > & GetIndexBuffer();

		void Discard();

	};

}