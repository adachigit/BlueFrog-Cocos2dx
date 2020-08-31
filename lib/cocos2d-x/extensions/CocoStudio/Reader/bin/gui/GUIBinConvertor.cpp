#include "GUIBinConvertor.h"

#include <string>
#include <fstream>

#include "DictionaryHelper.h"
#include "../BinWriter.h"
#include "../BinReader.h"
#include "RootNodeConvertor.h"

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	GUIBinConvertor* GUIBinConvertor::m_instance = NULL;

	GUIBinConvertor::GUIBinConvertor()
	{

	}

	GUIBinConvertor* GUIBinConvertor::sharedWriter()
	{
		if (!m_instance)
		{
			m_instance = new GUIBinConvertor();
		}

		return m_instance;
	}

	void GUIBinConvertor::convertJsonToBinByFilename(const char* jsonFilename, const char* binFilename)
	{
		unsigned char *pBytes = NULL;
		std::string jsonpath;
		rapidjson::Document jsonDict;
		jsonpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(jsonFilename);
		unsigned long size = 0;
		pBytes = CCFileUtils::sharedFileUtils()->getFileData(jsonpath.c_str(), "r", &size);
		if (NULL == pBytes || strcmp((const char*)pBytes, "") == 0)
		{
			printf("read json file[%s] error!\n", jsonFilename);
			return;
		}
		CCData *data = new CCData(pBytes, size);
		std::string load_str = std::string((const char *)data->getBytes(), data->getSize());
		CC_SAFE_DELETE(data);
		jsonDict.Parse<0>(load_str.c_str());
		if (jsonDict.HasParseError())
		{
			printf("GetParseError %s\n", jsonDict.GetParseError());
			return;
		}

		BinWriter writer;
		uint32_t rootNode = writer.newNode();
		RootNodeConvertor::writeProperties(jsonDict, rootNode, writer);

		writer.dumpToFile(binFilename, FILE_TYPE::CCS_GUI);
	}

	UIWidget* GUIBinConvertor::widgetFromBinFile(const char* filename)
	{
		std::string fullpath;
		fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
		int pos = fullpath.find_last_of('/');
		string filePath = fullpath.substr(0, pos + 1);
		unsigned long size = 0;
		unsigned char* pBytes = CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "rb", &size);
		if (NULL == pBytes || strcmp((const char*)pBytes, "") == 0)
		{
			printf("read bin file[%s] error!\n", filename);
			return NULL;
		}

		BinReader reader;
		reader.parseFromBinBuffer((char*)pBytes, size);
		reader.putExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileFullPath, filePath.c_str());
		reader.putExpendParam(GUIBinConvertor::READER_PARAM_KEY::BinFileName, filename);
		NodeElement* pRootNode = reader.getRootNode();

		int versionInteger = GUIReader::shareReader()->getVersionInteger(RootNodeConvertor::getVersion(pRootNode, reader));
		if (versionInteger < 250)
		{
			assert(0 && "version lower then 250 is not support");
		}
		else
		{
			return RootNodeConvertor::readProperties(pRootNode, reader);
		}

		return NULL;
	}
}
}

NS_CC_EXT_END