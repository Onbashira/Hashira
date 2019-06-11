#pragma once
#include "Engine/Source/Command/State/GeometryState.h"
#include "Engine/Source/Command/State/TargetState.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/Command/State/PipelineResourceTable.h"
#include "Engine/Source/Command/State/PipelineState.h"
#include "Engine/Source/Signature/RootSignature.h"

namespace Hashira {

	enum INTERNAL_COMMAND {
		INTERNAL_COMMAND_COPY,
		INTERNAL_COMMAND_UPDATE_BUFFER,
		INTERNAL_COMMAND_DRAW_INSTANCED,
		INTERNAL_COMMAND_DRAW_INDEXED_INSTANCED,
		INTERNAL_COMMAND_DISPATCH,
		INTERNAL_COMMAND_ASYNC_COMPONENT,
		INTERNAL_COMMAND_INDIRECT_DRAW,
		INTERNAL_COMMAND_MAX
	};

	struct InternalCommandHeader
	{
		//�R�}���h����
		struct Attribute {
			//�ǂ����������߂�
			unsigned char typeID;

			//�R�}���h�̃T�C�Y
			unsigned int  commandSize : 24;

		}Attribute;
		//�R�}���h�D��x

		struct CommandPriority {

			//�\��
			unsigned char reserve;

			//���C���[
			unsigned int  layer : 10;

			//�Z�O�����g
			unsigned int  segment : 6;

			//�D��x
			unsigned int  priority;

			//�X���b�h��ID
			unsigned char threadID;
		}CommandPriorty;

	};

	struct InternalCommandArgment
	{
		std::shared_ptr< GeometryState> geometry;
		std::shared_ptr< TargetState> target;
		std::shared_ptr< PipelineResourceTable> pipelineResource;
		std::shared_ptr< PipelineState> piepline;
	};

	struct InternalCommand {

		InternalCommandHeader header;

		InternalCommandArgment argment;

		

	};

}