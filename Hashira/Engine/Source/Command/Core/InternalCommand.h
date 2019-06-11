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
		//コマンド属性
		struct Attribute {
			//どういった命令か
			unsigned char typeID;

			//コマンドのサイズ
			unsigned int  commandSize : 24;

		}Attribute;
		//コマンド優先度

		struct CommandPriority {

			//予約
			unsigned char reserve;

			//レイヤー
			unsigned int  layer : 10;

			//セグメント
			unsigned int  segment : 6;

			//優先度
			unsigned int  priority;

			//スレッドのID
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