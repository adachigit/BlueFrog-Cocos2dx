#include "BinWriter.h"

#include <string>
#include <map>
#include <vector>
#include <assert.h>
#include <iostream>

using namespace std;

NS_CC_EXT_BEGIN

namespace ccsbin
{
const char BinWriter::endStr = 0x00;

BinWriter::BinWriter()
{
	m_Version = "1.0.0";
	m_dataSize = 0;
	m_nodeDataSize = 0;
	m_arrayDataSize = 0;
	m_stringDataSize = 0;
}
BinWriter::~BinWriter()
{
	clear();
}

uint32_t BinWriter::addString(const char* _str)
{
	string str = _str;

	StringIndexMapType::iterator findIter = m_StringToIndexMap.find(str);
	if (findIter != m_StringToIndexMap.end())
	{
		StringDesc* pDesc = m_StringDescVector[(*findIter).second];
		return pDesc->offset;
	}

	StringDesc* pDesc = new StringDesc();
	StringDesc* pLastDesc = (m_StringDescVector.empty()) ? NULL : m_StringDescVector.back();

	pDesc->str = str;
	pDesc->length = str.length();
	if (!pLastDesc)
	{
		pDesc->offset = 0;
	}
	else
	{
		pDesc->offset = pLastDesc->offset + pLastDesc->length + 1;	// Add 1 of string ending '\0'
	}

	m_StringDescVector.push_back(pDesc);
	m_StringToIndexMap.insert(StringIndexMapType::value_type(str, m_StringDescVector.size() - 1));

	return pDesc->offset;
}

uint32_t BinWriter::newNode()
{
	NodeDesc* pNode = new NodeDesc();

	m_NodeDescVector.push_back(pNode);

	return sizeof(NodeElement) * (m_NodeDescVector.size() - 1);
}

NodeDesc* BinWriter::getNode(uint32_t offset)
{
	int index = (offset / sizeof(NodeElement));

	if (index >= 0)
	{
		return m_NodeDescVector[index];
	}
	else
	{
		return NULL;
	}
}

void BinWriter::addNodeAttribute(uint32_t nodeOffset, AttrElement* pElement)
{
	NodeDesc* pNode = getNode(nodeOffset);
	assert(pNode);

	pNode->attrVector.push_back(pElement);
}

void BinWriter::clear()
{
	for (uint32_t i = 0; i < m_AttrVector.size(); ++i)
	{
		AttrElement* pElement = m_AttrVector[i];
		delete pElement;
	}
	m_AttrVector.clear();

	for (uint32_t i = 0; i < m_NodeDescVector.size(); ++i)
	{
		NodeDesc* pNode = m_NodeDescVector[i];
		for (uint32_t i = 0; i < pNode->attrVector.size(); ++i)
		{
			delete pNode->attrVector[i];
		}
		delete pNode;
	}
	m_NodeDescVector.clear();

	for (uint32_t i = 0; i < m_StringDescVector.size(); ++i)
	{
		StringDesc* pDesc = m_StringDescVector[i];
		delete pDesc;
	}
	m_StringDescVector.clear();

	for (uint32_t i = 0; i < m_ArrayDescVector.size(); ++i)
	{
		ArrayDesc* pDesc = m_ArrayDescVector[i];
		delete[] pDesc->array;
		delete pDesc;
	}
	m_ArrayDescVector.clear();

	m_StringToIndexMap.clear();

	m_dataSize = 0;
	m_nodeDataSize = 0;
	m_arrayDataSize = 0;
	m_stringDataSize = 0;
}

void BinWriter::dumpHeader(ofstream& ofs, uint32_t fileType)
{
	// Write file type
	write32(fileType, ofs);
	// Write version
//	char versionBuf[16];
//	memset(versionBuf, 0x00, 16);
//	strcpy_s(versionBuf, 16, m_Version.c_str());
//	ofs.write(versionBuf, 16);
	// Write dataSize
	write32(m_dataSize, ofs);
	write32(m_nodeDataSize, ofs);
	write32(m_arrayDataSize, ofs);
	write32(m_stringDataSize, ofs);

	return;
}

void BinWriter::dumpStrings(ofstream& ofs)
{
	for (vector<StringDesc*>::iterator ite = m_StringDescVector.begin(); ite != m_StringDescVector.end(); ++ite)
	{
		ofs.write((*ite)->str.c_str(), (*ite)->length);
		ofs.write(&endStr, 1);
		m_dataSize += sizeof(char) * ((*ite)->length + 1);
		m_stringDataSize += sizeof(char) * ((*ite)->length + 1);
	}
}

void BinWriter::dumpArrays(ofstream& ofs)
{
	for (int32_t i = 0; i < m_ArrayDescVector.size(); ++i)
	{
		ArrayDesc* pDesc = m_ArrayDescVector[i];
		write32(pDesc->count, ofs);
		ofs.write(pDesc->array, pDesc->size);
		m_dataSize += pDesc->size + sizeof(pDesc->size);
		m_arrayDataSize += pDesc->size + sizeof(pDesc->size);
	}
}
/*
void Writer::dumpAttributes(ofstream& ofs)
{
	for (vector<AttrElement*>::iterator ite = m_AttrVector.begin(); ite != m_AttrVector.end(); ++ite)
	{
		AttrElement* pElement = (AttrElement*)(*ite);
		write32(pElement->attrUIValue, ofs);
		write32(pElement->attrParam, ofs);
		m_dataSize += sizeof(AttrElement);
	}
}
*/
void BinWriter::dumpNodes(ofstream& ofs)
{
	for (vector<NodeDesc*>::iterator ite = m_NodeDescVector.begin(); ite != m_NodeDescVector.end(); ++ite)
	{
		NodeDesc* pNode = (*ite);
		AttrElement** elements = new AttrElement*[pNode->attrVector.size()];
		for (uint32_t i = 0; i < pNode->attrVector.size(); ++i)
		{
			elements[i] = pNode->attrVector[i];
		}

		NodeElement nodeElement;
		nodeElement.attrArray = newArray(elements, pNode->attrVector.size());
		write32(nodeElement.attrArray, ofs);

		m_dataSize += sizeof(NodeElement);
		m_nodeDataSize += sizeof(NodeElement);
	}
}

void BinWriter::dumpToFile(const char* filename, uint32_t fileType)
{
	ofstream ofs;
	ofs.open(filename, ios_base::out | ios_base::binary);

	if (!ofs.is_open())
	{
		cout << "Can not open file " << filename << endl;
		return;
	}

	m_dataSize = 0;

	// Write file header
	dumpHeader(ofs, fileType);
	// Write nodes
	dumpNodes(ofs);
	// Write arrays
	dumpArrays(ofs);
	// Write strings
	dumpStrings(ofs);
	
	ofs.seekp(sizeof(uint32_t), ios_base::beg);
	write32(m_dataSize, ofs);
	write32(0, ofs);		// node address offset
	write32(m_nodeDataSize, ofs);	// array address offset
	write32(m_arrayDataSize + m_nodeDataSize, ofs);		// string address offset
	
	ofs.flush();
	ofs.close();
}

}

NS_CC_EXT_END