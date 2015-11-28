#ifndef SCENE_MANAGER_CM_PLAY_H
#define SCENE_MANAGER_CM_PLAY_H

#include "SceneManagerGameplay.h"
#include "SceneNode.h"
#include "AI Folder\Tank.h"
#include "AI Folder\Healer.h"
#include "AI Folder\Mage.h"
#include "AI Folder\Boss.h"

class SceneManagerCMPlay : public SceneManagerGameplay
{
private:
	vector<GameObject3D*> objectList;
	SceneNode* sceneGraph;
public:
	SceneManagerCMPlay();
	~SceneManagerCMPlay();

	void Init(const int width, const int height, ResourcePool* RP, InputManager* controls);
	void Config();
	void Update(double dt);
	void Render();
	void Exit();

	void BindShaders();
	void InitShader();

	void InitSceneGraph();
	void FSMApplication();

	void RenderLight();
	void RenderBG();
	void RenderStaticObject();
	void RenderMobileObject();

	CTank*		Tank;
	CHealer*	Healer;
	CMage*		Mage;
	CBoss*		Boss;
};

#endif