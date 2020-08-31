#ifndef _PATCHER_SCENE_H
#define _PATCHER_SCENE_H

#include "cocos2d.h"
#include "ResManager/Patcher.h"
#include "SimulatorConfig.h"

class PatcherScene : public cocos2d::CCScene ,public ResManager::XPatcherDelegateProtocol
{
public:
	PatcherScene();
	~PatcherScene();
	//override ccscene
	virtual bool init();

	//override patcherdelegate
	virtual void OnMsg(ResManager::MsgCode ret);
	virtual void OnTotalProgress(int percent);
	virtual void OnFileProgress(int percent);

	//event
	void StartPatcher(cocos2d::CCObject *pSender);
	void PlayGame(cocos2d::CCObject* pSender) { this->SwitchToLoginScene(); }
	void StopPatcher(cocos2d::CCObject* pSender) 
	{
		ResManager::XPatcher::SharedPatcher()->Stop();
	}

	void SetProjectConfig(ProjectConfig* pCfg) { _pProjCfg = pCfg; }
	CREATE_FUNC(PatcherScene);
private:
	void SwitchToLoginScene();
private:
	cocos2d::CCMenuItemFont *_pItemUpdate;
	cocos2d::CCMenuItemFont *_pItemPlay;
	cocos2d::CCMenuItemFont *_pItemCancel;
	cocos2d::CCLabelTTF *_pCurVersionLabel;
	cocos2d::CCLabelTTF *_pLatestVersionLabel;
	cocos2d::CCLabelTTF *_pFileProgressLabel;
	cocos2d::CCLabelTTF *_pTotalProgressLabel;
	cocos2d::CCLabelTTF *_pStateLabel;
	ProjectConfig* _pProjCfg;
};
class StartupCall : public CCObject
{
public:
	static StartupCall *create(ProjectConfig *pCfg);
	void startup();

private:
	ProjectConfig *_pCfg;
};
#endif