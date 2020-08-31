#pragma once

#include "ExtensionMacros.h"
#include "BaseClasses/UIWidget.h"

NS_CC_EXT_BEGIN

namespace ccsbin
{
namespace gui
{
	class GUIBinConvertor
	{
	public:
		enum READER_PARAM_KEY
		{
			BinFileName,
			BinFileFullPath,
		};

		static GUIBinConvertor* sharedWriter();

		void convertJsonToBinByFilename(const char* jsonPath, const char* binPath);
		UIWidget* widgetFromBinFile(const char* binFilename);
	private:
		GUIBinConvertor();

		static GUIBinConvertor* m_instance;

	};
}
}

NS_CC_EXT_END