#pragma once

#include <stdint.h>

#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "common.h"

using namespace std;

NS_CC_EXT_BEGIN

namespace ccsbin
{
	struct StringDesc
	{
		string	 str;
		uint32_t offset;
		uint32_t length;
	};

	struct ArrayDesc
	{
		char*		array;
		int32_t		size;
		uint32_t	count;
		uint32_t	offset;
	};

	struct NodeDesc
	{
		vector<AttrElement*> attrVector;
	};

	class BinWriter
	{
	public:
		BinWriter();
		~BinWriter();

		template<typename T> AttrElement* newAttribute(uint32_t attrType, T value)
		{
			static_assert(sizeof(T) == sizeof(uint32_t), "Size of typename T is not equals sizeof(uint32_t)");

			AttrElement* pElement = new AttrElement();
			pElement->attrType = attrType;
			pElement->attrIValue = *(int32_t*)(T*)&value; //(value, (char*)&pElement->attrUIValue);

			return pElement;
		}

		template<const char*> AttrElement* newAttribute(uint32_t attrType, const char* value)
		{
			AttrElement* pElement = new AttrElement();
			pElement->attrType = attrType;
			pElement->attrUIValue = addString(value);

			return pElement;
		}

		template<char*> AttrElement* newAttribute(uint32_t attrType, char* value)
		{
			AttrElement* pElement = new AttrElement();
			pElement->attrType = attrType;
			pElement->attrUIValue = addString(value);

			return pElement;
		}

		template<typename T> uint32_t newArray(T* array, uint32_t size)
		{
			static_assert(sizeof(T) == sizeof(uint32_t), "Size of typename T is not equals sizeof(uint32_t)");

			ArrayDesc* pDesc = new ArrayDesc();
			pDesc->array = new char[sizeof(T) * size];
			uint32_t pos = 0;
			for (uint32_t i = 0; i < size; ++i)
			{
				memWrite32(array[i], pDesc->array + pos);
				pos += sizeof(T);
			}
			pDesc->size = sizeof(T) * size;
			pDesc->count = size;

			ArrayDesc* pLastDesc = (m_ArrayDescVector.empty()) ? NULL : m_ArrayDescVector.back();
			if (pLastDesc == NULL)
			{
				pDesc->offset = 0;
			}
			else
			{
				pDesc->offset = pLastDesc->offset + pLastDesc->size + sizeof(uint32_t);
			}

			m_ArrayDescVector.push_back(pDesc);

			return pDesc->offset;
		}

		template<char*> uint32_t newArray(char** array, uint32_t size)
		{
			uint32_t* arr = new uint32_t[size];

			for (uint32_t i = 0; i < size; ++i)
			{
				arr[i] = addString(array[i]);
			}

			uint32_t offset = newArray(arr, size);
			delete[] arr;

			return offset;
		}

		template<AttrElement*> uint32_t newArray(AttrElement** array, uint32_t size)
		{
			ArrayDesc* pDesc = new ArrayDesc();
			pDesc->array = new char[sizeof(AttrElement) * size];
			pDesc->size = sizeof(AttrElement) * size;
			pDesc->count = size;

			uint32_t pos = 0;
			for (uint32_t i = 0; i < size; ++i)
			{
				AttrElement* pElement = array[i];
				memWrite32(pElement->attrType, pDesc->array + pos);
				pos += sizeof(pElement->attrType);
				memWrite32(pElement->attrUIValue, pDesc->array + pos);
				pos += sizeof(pElement->attrUIValue);
			}

			ArrayDesc* pLastDesc = (m_ArrayDescVector.empty()) ? NULL : m_ArrayDescVector.back();
			if (pLastDesc == NULL)
			{
				pDesc->offset = 0;
			}
			else
			{
				pDesc->offset = pLastDesc->offset + pLastDesc->size + sizeof(uint32_t);
			}

			m_ArrayDescVector.push_back(pDesc);

			return pDesc->offset;
		}

		uint32_t newNode();
		void addNodeAttribute(uint32_t nodeOffset, AttrElement* pElement);
		uint32_t addString(const char* _str);

		void clear();
		void dumpToFile(const char* filename, uint32_t fileType);
		void dumpHeader(ofstream& ofs, uint32_t fileType);
		void dumpStrings(ofstream& ofs);
		void dumpArrays(ofstream& ofs);
		void dumpAttributes(ofstream& ofs);
		void dumpNodes(ofstream& ofs);
	private:
		NodeDesc* getNode(uint32_t offset);

		template<typename T> void write32(T value, ofstream& ofs)
		{
			static_assert(sizeof(T) == sizeof(uint32_t), "Size of typename T is not equals sizeof(uint32_t)");
		#if defined(BIGENDIAN)
			char* src = (char*)&value;
			char dest[4];
			dest[0] = src[3];
			dest[1] = src[2];
			dest[2] = src[1];
			dest[3] = src[0];
			ofs.write(dest, 4);
		#else
			ofs.write((char*)&value, 4);
		#endif
		}

		template<typename T> void memWrite32(T value, char* buf)
		{
			static_assert(sizeof(T) == sizeof(uint32_t), "Size of typename T is not equals sizeof(uint32_t)");
		#if defined(BIGENDIAN)
			char* src = (char*)&value;
			buf[0] = src[3];
			buf[1] = src[2];
			buf[2] = src[1];
			buf[3] = src[0];
		#else
			memcpy((void*)buf, &value, sizeof(T));
		#endif
		}

		static const char endStr;

		string m_Version;
		uint32_t m_dataSize;
		uint32_t m_nodeDataSize;
		uint32_t m_arrayDataSize;
		uint32_t m_stringDataSize;

		typedef map<string, uint32_t> StringIndexMapType;

		vector<AttrElement*>	m_AttrVector;
		vector<NodeDesc*>		m_NodeDescVector;
		vector<StringDesc*>		m_StringDescVector;
		vector<ArrayDesc*>		m_ArrayDescVector;
		StringIndexMapType		m_StringToIndexMap;
	};
}

NS_CC_EXT_END