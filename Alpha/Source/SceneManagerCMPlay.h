#ifndef SCENE_MANAGER_CM_PLAY_H
#define SCENE_MANAGER_CM_PLAY_H

#include "SceneManagerGameplay.h"
#include "SceneNode.h"
#include "AI Folder\Tank.h"
#include "AI Folder\Healer.h"
#include "AI Folder\Mage.h"
#include "AI Folder\Boss.h"

#define WARRIOR "WARRIOR"
#define SWORD	"_SWORD_"
#define SHIELD	"_SHIELD_"
#define SHIELD_PIVOT "_SHIELD_PIVOT_"
#define MAGE "MAGE"
#define ROD	"_ROD_"
#define HEALER "HEALER"
#define STAFF	"_STAFF_"
#define BOSS "BOSS"
#define LARM "_LARM_"
#define RARM "_RARM_"

#define CHILD_1 1
#define CHILD_2 2

class SceneManagerCMPlay : public SceneManagerGameplay
{
private:
	vector<GameObject3D*> objectList;
	SceneNode* sceneGraph;
	vector<CEntity*> ListOfCharacters;

	int TANK_COUNT;
	int MAGE_COUNT;
	int	HEALER_COUNT;
	int BOSS_COUNT;
	int TOTAL_COUNT;

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
	void RenderPlayerStats();

	void AddTANK(string ID);
	void AddMAGE(string ID);
	void AddHEALER(string ID);
	void AddBOSS(string ID);

	void TANK_NODE(CEntity* theTank);
	void MAGE_NODE(CEntity* theMage);
	void HEALER_NODE(CEntity* theHealer);
	void BOSS_NODE(CEntity* theBoss);

	CTank*		Tank;
	CHealer*	Healer;
	CMage*		Mage;
	CBoss*		Boss;
};

#endif