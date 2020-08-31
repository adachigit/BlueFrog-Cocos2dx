#include "PatcherScene.h"

USING_NS_CC;
using namespace ResManager;

PatcherScene::PatcherScene() :_pItemUpdate(NULL), _pCurVersionLabel(NULL),
_pLatestVersionLabel(NULL), _pFileProgressLabel(NULL), _pTotalProgressLabel(NULL),
_pStateLabel(NULL), _pItemPlay(NULL), _pItemCancel(NULL), _pProjCfg(NULL)
{}

PatcherScene::~PatcherScene()
{
	XPatcher::SharedPatcher()->PurgePatcher();
}
//////////////////////////////////////////////////////////////////////////
//override
bool PatcherScene::init()
{
	if (!CCScene::init())
		return false;

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	_pItemUpdate = CCMenuItemFont::create("update", this, menu_selector(PatcherScene::StartPatcher));
	if (NULL != _pItemUpdate)
	{
		_pItemUpdate->setPosition(ccp(origin.x + visibleSize.width - 50,
			origin.y + visibleSize.height - 20));
	}
	_pItemPlay = CCMenuItemFont::create("play", this, menu_selector(PatcherScene::PlayGame));
	if (NULL != _pItemPlay)
	{
		_pItemPlay->setPosition(ccp(origin.x + visibleSize.width - 50,
			origin.y + visibleSize.height - 50));
	}
	_pItemCancel = CCMenuItemFont::create("cancel", this, menu_selector(PatcherScene::StopPatcher));
	if (NULL != _pItemCancel)
	{
		_pItemCancel->setPosition(ccp(origin.x + visibleSize.width - 50,
			origin.y + visibleSize.height - 80));
	}
	CCMenu* pMenu = CCMenu::create(_pItemUpdate,_pItemPlay,_pItemCancel, NULL);
	if (NULL != pMenu)
	{
		pMenu->setPosition(origin);
		addChild(pMenu);
	}

	_pCurVersionLabel = CCLabelTTF::create("", "Arial", 20);
	if (NULL != _pCurVersionLabel)
	{
		_pCurVersionLabel->setPosition(ccp(origin.x + 50, origin.y + visibleSize.height - 20));
		addChild(_pCurVersionLabel);
	}
	
	_pLatestVersionLabel = CCLabelTTF::create("", "Arial", 20);
	if (NULL != _pLatestVersionLabel)
	{
		_pLatestVersionLabel->setPosition(ccp(origin.x + 50,
			origin.y + 60));
		addChild(_pLatestVersionLabel);
	}
	
	_pFileProgressLabel = CCLabelTTF::create("", "Arial", 20);
	if (NULL != _pFileProgressLabel)
	{
		_pFileProgressLabel->setPosition(ccp(origin.x + visibleSize.width - 50,
			origin.y + 60));
		addChild(_pFileProgressLabel);
	}

	_pTotalProgressLabel = CCLabelTTF::create("", "Arial", 20);
	if (NULL != _pTotalProgressLabel)
	{
		_pTotalProgressLabel->setPosition(ccp(origin.x + visibleSize.width / 2,
			origin.y + 40));
		addChild(_pTotalProgressLabel);
	}
	
	_pStateLabel = CCLabelTTF::create("", "Arial", 20);
	if (NULL != _pStateLabel)
	{
		_pStateLabel->setPosition(ccp(origin.x + visibleSize.width / 2,
			origin.y + 20));
		addChild(_pStateLabel);
	}
	return true;
}

void PatcherScene::OnMsg(MsgCode msg)
{
	XPatcher* pPatcher = XPatcher::SharedPatcher();
	switch (msg)
	{
	case ResManager::CHECK_VERSION:
		if (NULL!=_pStateLabel)	_pStateLabel->setString("check version");
		break;
	case ResManager::OBTAIN_VERSION_FAIL:
		if(NULL!=_pStateLabel) _pStateLabel->setString("check version fail");
		break;
	case ResManager::UPDATE_NEWEST_VERSION:
		if(NULL!=_pLatestVersionLabel) _pLatestVersionLabel->setString(pPatcher->GetLatestVersionStr().c_str());
		break;
	case ResManager::ALREADY_NEWEST:
		if(NULL!=_pStateLabel) _pStateLabel->setString("no new version");
		break;
	case ResManager::START_DOWNLOAD:
		if (NULL != _pStateLabel) _pStateLabel->setString("start download");
		break;
	case ResManager::DOWNLOAD_FAIL:
		if (NULL != _pStateLabel) _pStateLabel->setString("download fail");
		break;
	case ResManager::START_UNZIP:
		if (NULL != _pStateLabel) _pStateLabel->setString("start unzip");
		break;
	case ResManager::UNZIP_FAIL:
		if (NULL != _pStateLabel) _pStateLabel->setString("unzip fail");
		break;
	case ResManager::UPDATE_CUR_VERSION:
		if (NULL != _pCurVersionLabel) _pCurVersionLabel->setString(pPatcher->GetCurVersionStr().c_str());
		break;
	case ResManager::PATCH_SUCCESS:
		if (NULL != _pStateLabel) _pStateLabel->setString("patch success");
		this->SwitchToLoginScene();
		break;
	case ResManager::PATCH_FAIL:
		this->SwitchToLoginScene();
		break;
	case ResManager::UPDATE_CANCEL:
		if (NULL != _pStateLabel) _pStateLabel->setString("patch cancel");
		this->SwitchToLoginScene();
		break;
	}
}

void PatcherScene::OnTotalProgress(int percent)
{
	char progress[20];
	snprintf(progress, 20, "downloading %d%%", percent);
	_pTotalProgressLabel->setString(progress);
}

void PatcherScene::OnFileProgress(int percent)
{
	char progress[20];
	snprintf(progress, 20, "file %d%%", percent);
	_pFileProgressLabel->setString(progress);
}

//////////////////////////////////////////////////////////////////////////
//
void PatcherScene::StartPatcher(cocos2d::CCObject *pSender)
{
	XPatcher* pPatcher = XPatcher::SharedPatcher();
	pPatcher->SetResUrl("http://121.52.229.206/download/wang/");
	pPatcher->SetVersionFileName("version");
	pPatcher->SetCurVersion("0.0.0");
	if (NULL != _pCurVersionLabel)
		_pCurVersionLabel->setString(pPatcher->GetCurVersionStr().c_str());
	pPatcher->SetConnectionTimeout(3);
	std::string storagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "cache";
	pPatcher->SetStoragePath(storagePath.c_str());
	pPatcher->SetDelegate(this);
	pPatcher->StartPatcher();
}

//////////////////////////////////////////////////////////////////////////
//
void PatcherScene::SwitchToLoginScene()
{
	StartupCall *call = StartupCall::create(_pProjCfg);
	CCDirector *pDirector = CCDirector::sharedDirector();
#ifdef _WIN32
	if (_pProjCfg->getDebuggerType() != kCCLuaDebuggerNone)
	{
		CCScene *scene = CCScene::create();
		CCLabelTTF *label = CCLabelTTF::create("WAITING FOR CONNECT TO DEBUGGER...", "Arial", 32);
		const CCSize winSize = pDirector->getWinSize();
		label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		scene->addChild(label);
		pDirector->replaceScene(scene);
		scene->runAction(CCCallFunc::create(call, callfunc_selector(StartupCall::startup)));
	}
	else
#endif
	{
		call->startup();
	}
}

//////////////////////////////////////////////////////////////////////////
//
// ----------------------------------------

StartupCall *StartupCall::create(ProjectConfig *pCfg)
{
	StartupCall *call = new StartupCall();
	call->_pCfg = pCfg;
	call->autorelease();
	return call;
}

void StartupCall::startup()
{
	CCLuaEngine *pEngine = CCLuaEngine::defaultEngine();
	CCLuaStack *pStack = pEngine->getLuaStack();

	// set search path
	string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(_pCfg->getScriptFileRealPath().c_str());
	size_t pos;
	while ((pos = path.find_first_of("\\")) != std::string::npos)
	{
		path.replace(pos, 1, "/");
	}
	size_t p = path.find_last_of("/");
	string workdir;
	if (p != path.npos)
	{
		workdir = path.substr(0, p);
		pStack->addSearchPath(workdir.c_str());
	}
#ifdef _WIN32
	// connect debugger
	if (_pCfg->getDebuggerType() != kCCLuaDebuggerNone)
	{
		pStack->connectDebugger(_pCfg->getDebuggerType(), NULL, 0, NULL, workdir.c_str());
	}
#endif

	// load framework
	/*    if (projectConfig.isLoadPrecompiledFramework())
	{
	const string precompiledFrameworkPath = SimulatorConfig::sharedDefaults()->getPrecompiledFrameworkPath();
	pStack->loadChunksFromZIP(precompiledFrameworkPath.c_str());
	}
	*/
	// set default scene
	CCScene *scene = CCScene::create();
	if (CCDirector::sharedDirector()->getRunningScene())
	{
		CCDirector::sharedDirector()->replaceScene(scene);
	}
	else
	{
		CCDirector::sharedDirector()->runWithScene(scene);
	}

	// load script
	string env = "__LUA_STARTUP_FILE__=\"";
	env.append(path);
	env.append("\"");
	pEngine->executeString(env.c_str());

	CCLOG("------------------------------------------------");
	CCLOG("LOAD LUA FILE: %s", path.c_str());
	CCLOG("------------------------------------------------");
	pEngine->executeScriptFile(path.c_str());
}
