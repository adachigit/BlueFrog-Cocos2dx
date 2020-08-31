#pragma once

#include "BaseClasses/UIWidget.h"

#include <map>
#include <string>

#include "common.h"

NS_CC_EXT_BEGIN

namespace ccsbin
{
	class BinReader
	{
	public:
		BinReader();
		~BinReader();

		void parseFromBinFile(const char* filename);
		void parseFromBinBuffer(char* buff, uint32_t size);

		int getFileType();
		NodeElement* getRootNode() { return m_pRootNode; }
		NodeElement* getNode(uint32_t offset);
		void* getArray(uint32_t offset, int32_t* pSize);
		const char* getString(uint32_t offset);

		void putExpendParam(int key, const char* param);
		const char* getExpendParam(int key);
		void clearExpendParam();
	private:
		int m_fileType;
		char* m_pBuff;
		char* m_pDataBaseAddr;
		uint32_t m_dataSize;
		uint32_t m_NodeBaseAddr;
		uint32_t m_ArrayBaseAddr;
		uint32_t m_StringBaseAddr;
		NodeElement* m_pRootNode;

		std::map<int, std::string> m_expendParamMap;
	};
}

NS_CC_EXT_END
