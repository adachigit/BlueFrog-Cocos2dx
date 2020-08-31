#include "BinReader.h"

#include <fstream>
#include <string>

using namespace std;

NS_CC_EXT_BEGIN

namespace ccsbin
{
BinReader::BinReader() :
m_fileType(0),
m_pBuff(NULL),
m_dataSize(0),
m_pDataBaseAddr(NULL),
m_NodeBaseAddr(0),
m_ArrayBaseAddr(0),
m_StringBaseAddr(0),
m_pRootNode(NULL)
{

}

BinReader::~BinReader()
{
	CC_SAFE_DELETE_ARRAY(m_pBuff);
	m_pRootNode = NULL;
	m_pDataBaseAddr = NULL;
}

void BinReader::parseFromBinFile(const char* filename)
{
	ifstream ifs;
	ifs.open(filename, std::ios_base::in | std::ios_base::binary);
	ifs.seekg(0, ios_base::end);
	uint32_t size = ifs.tellg();
	ifs.seekg(0, ios_base::beg);

	char* buff = new char[size];
	ifs.read((char*)buff, size);

	ifs.close();

	parseFromBinBuffer(buff, size);
}

void BinReader::parseFromBinBuffer(char* buff, uint32_t size)
{
	m_fileType = *((uint32_t*)buff);
	m_dataSize = *((uint32_t*)(buff + 4));
	m_NodeBaseAddr = *((uint32_t*)(buff + 8));
	m_ArrayBaseAddr = *((uint32_t*)(buff + 12));
	m_StringBaseAddr = *((uint32_t*)(buff + 16));

	CC_SAFE_DELETE_ARRAY(m_pBuff);

	m_pBuff = buff;// new char[m_dataSize];
	m_pDataBaseAddr = buff + sizeof(uint32_t) * 5;

	m_pRootNode = (NodeElement*)(m_pDataBaseAddr + m_NodeBaseAddr);
}

NodeElement* BinReader::getNode(uint32_t offset)
{
	return (NodeElement*)(m_pDataBaseAddr + m_NodeBaseAddr + offset);
}

void* BinReader::getArray(uint32_t offset, int32_t* pSize)
{
	const char* pAddr = m_pDataBaseAddr + m_ArrayBaseAddr + offset;
	int32_t size = *((int32_t*)(pAddr));

	if (pSize)
	{
		(*pSize) = size;
	}

	return (size > 0) ? (void*)(pAddr + sizeof(int32_t)) : NULL;
}

const char* BinReader::getString(uint32_t offset)
{
	return m_pDataBaseAddr + m_StringBaseAddr + offset;
}

void BinReader::putExpendParam(int key, const char* param)
{
	m_expendParamMap.insert(std::map<int, std::string>::value_type(key, param));
}

void BinReader::clearExpendParam()
{
	m_expendParamMap.clear();
}

const char* BinReader::getExpendParam(int key)
{
	std::map<int, std::string>::iterator iter = m_expendParamMap.find(key);

	if (iter != m_expendParamMap.end())
	{
		return iter->second.c_str();
	}

	return NULL;
}
}
NS_CC_EXT_END