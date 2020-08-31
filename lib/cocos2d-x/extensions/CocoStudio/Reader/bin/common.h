#pragma once

#include <stdint.h>

#include "ExtensionMacros.h"

#pragma pack(push, 4)

NS_CC_EXT_BEGIN

namespace ccsbin
{
	enum FILE_TYPE : uint32_t
	{
		CCS_GUI		= 1,
	};

	struct AttrElement
	{
		uint32_t		attrType;
		union
		{
			uint32_t	attrUIValue;
			int32_t		attrIValue;
			float		attrFValue;
		};
	};

	struct NodeElement
	{
		uint32_t attrArray;
	};

	struct FileDesc
	{
		uint32_t		fileType;
		char			version[16];
		uint32_t		dataSize;
		uint32_t		arrayMemAddr;
		uint32_t		stringMemAddr;
		uint32_t		nodeMemAddr;
		NodeElement*	pRootNode;

	public:
		template<typename T>
		T* getArray(uint32_t offset)
		{
			return (T*)(arrayMemAddr + offset);
		}

		char* getString(uint32_t offset)
		{
			return (char*)(stringMemAddr + offset);
		}

		NodeElement* getNodeElement(uint32_t offset)
		{
			return (NodeElement*)(nodeMemAddr + offset);
		}

	};
}

NS_CC_EXT_END

#pragma pack(pop)
