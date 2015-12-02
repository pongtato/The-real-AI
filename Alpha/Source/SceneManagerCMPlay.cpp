#include "SceneManagerCMPlay.h"

SceneManagerCMPlay::SceneManagerCMPlay() 
: sceneGraph(NULL)
, Tank(NULL)
, Healer(NULL)
, Mage(NULL)
, Boss(NULL)
{
	ViewChoice = 0;
	KeyDelay = 0.f;
}

SceneManagerCMPlay::~SceneManagerCMPlay()
{
	if (sceneGraph)
	{
		delete sceneGraph;
		sceneGraph = NULL;
	}

	if (Tank)
	{
		delete Tank;
		Tank = NULL;
	}

	if (Healer)
	{
		delete Healer;
		Healer = NULL;
	}

	if (Mage)
	{
		delete Mage;
		Mage = NULL;
	}

	if (Boss)
	{
		delete Boss;
		Boss = NULL;
	}


}

void SceneManagerCMPlay::Init(const int width, const int height, ResourcePool *RM, InputManager* controls)
{
	SceneManagerGameplay::Init(width, height, RM, controls);
	this->Tank = new CTank;
	this->Mage = new CMage;
	this->Healer = new CHealer;
	this->Boss = new CBoss;

	this->InitShader();

	tpCamera.Init(Vector3(0, 0, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	fpCamera.Init(Vector3(0, 0, -10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	Tank->SetDangerZone(Boss->GetAttackRange() * 1.5f);
	Mage->SetDangerZone(Boss->GetAttackRange() * 1.5f);
	Healer->SetDangerZone(Boss->GetAttackRange() * 1.5f);

	int TANK_COUNT = 1;
	int MAGE_COUNT = 1;
	int	HEALER_COUNT = 1;
	int BOSS_COUNT = 1;
	int TOTAL_COUNT = 0;

	string newName = "WARRIOR_";
	newName += to_string(TANK_COUNT);
	Tank->SetID(newName, WARRIOR, TOTAL_COUNT);
	Tank->RandomSpawn(0, 100);

	TOTAL_COUNT++;

	newName = "MAGE_";
	newName += to_string(MAGE_COUNT);
	Mage->SetID(newName, MAGE, TOTAL_COUNT);
	Mage->RandomSpawn(0, 100);

	TOTAL_COUNT++;

	newName = "HEALER_";
	newName += to_string(HEALER_COUNT);
	Healer->SetID(newName, HEALER, TOTAL_COUNT);
	Healer->RandomSpawn(0, 100);

	TOTAL_COUNT++;

	newName = "BOSS_";
	newName += to_string(BOSS_COUNT);
	Boss->SetID(newName, BOSS, TOTAL_COUNT);
	Boss->RandomSpawn(0, 100);

	TOTAL_COUNT++;

	ListOfCharacters.push_back(Tank);
	ListOfCharacters.push_back(Mage);
	ListOfCharacters.push_back(Healer);
	ListOfCharacters.push_back(Boss);

	InitSceneGraph();
	lightEnabled = true;
}

void SceneManagerCMPlay::Config()
{

}

void SceneManagerCMPlay::Update(double dt)
{
	SceneManagerGameplay::Update(dt);
	//Uncomment the following line to play sound
	//resourceManager.retrieveSound("MenuFeedback");

	if (inputManager->getKey("ToggleWireFrame"))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	else if (inputManager->getKey("ToggleFill"))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	KeyDelay = Math::Clamp(KeyDelay += (float)dt, 0.f, 1.f);

	if (inputManager->getKey("Up") && KeyDelay >= 1.f)
	{
		ViewChoice = Math::Wrap(ViewChoice + 1, 0, ((int)ListOfCharacters.size() - 1));
		KeyDelay = 0.0f;
	}

	if (inputManager->getKey("Down") && KeyDelay >= 1.f)
	{
		ViewChoice = Math::Wrap(ViewChoice - 1, 0, ((int)ListOfCharacters.size() - 1));
		KeyDelay = 0.0f;
	}

	Vector3 m_CameraFollow = ListOfCharacters[ViewChoice]->GetPosition();

	theDirection = tpCamera.getTarget() - ListOfCharacters[ViewChoice]->GetPosition();
	if (!theDirection.IsZero())
	{
		theDirection.Normalize();
	}

	tpCamera.UpdatePosition((m_CameraFollow - (theDirection * 15.f)), Vector3(0, 0, 0));
	//tpCamera.Update(dt);

	//cout << (m_CameraFollow - (theDirection * 15.f)) << endl;
	//cout << theDirection << endl;
	//cout << tpCamera.getPosition() << endl;
	


	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (i != Boss->GetCurrentTarget())
		{
			//Set all units that are not the boss current target to false;
			ListOfCharacters[i]->SetIsTarget(false);
		}
		else
		{
			//Bosses target
			ListOfCharacters[i]->SetIsTarget(true);
		}
	}

	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == WARRIOR)
		{
			ListOfCharacters[i]->RunFSM(dt, ListOfCharacters, Boss->GetPosition(), Boss->GetPosition());
		}
		else if (ListOfCharacters[i]->GetTYPE() == MAGE)
		{
			ListOfCharacters[i]->RunFSM(dt, ListOfCharacters, Boss->GetPosition(), Boss->GetPosition());
		}
		else if (ListOfCharacters[i]->GetTYPE() == HEALER)
		{
			ListOfCharacters[i]->RunFSM(dt, ListOfCharacters, Boss->GetPosition(), Boss->GetPosition());
		}
		else if (ListOfCharacters[i]->GetTYPE() == BOSS)
		{
			ListOfCharacters[i]->RunFSM(dt, ListOfCharacters, Boss->GetPosition(), Boss->GetPosition());
		}
	}


}

void SceneManagerCMPlay::Render()
{
	SceneManagerGameplay::Render();

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(tpCamera.getPosition().x, tpCamera.getPosition().y, tpCamera.getPosition().z,
		tpCamera.getTarget().x, tpCamera.getTarget().y, tpCamera.getTarget().z,
		tpCamera.getUp().x, tpCamera.getUp().y, tpCamera.getUp().z);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderLight();
	RenderBG();
	RenderStaticObject();
	RenderMobileObject();
}

void SceneManagerCMPlay::Exit()
{
	if (sceneGraph)
	{
		delete sceneGraph;
		sceneGraph = NULL;
	}

	if (Tank)
	{
		delete Tank;
		Tank = NULL;
	}

	if (Healer)
	{
		delete Healer;
		Healer = NULL;
	}

	if (Mage)
	{
		delete Mage;
		Mage = NULL;
	}

	if (Boss)
	{
		delete Boss;
		Boss = NULL;
	}

	SceneManagerGameplay::Exit();
}

void SceneManagerCMPlay::BindShaders()
{
	SceneManagerGameplay::BindShaders();
}

// Other specific init, update and render classes
void SceneManagerCMPlay::InitShader()
{
	SHADER thisShader = resourceManager.retrieveShader("Comg");
	programID = LoadShaders(thisShader.vertexShaderDirectory.c_str(), thisShader.fragmentShaderDirectory.c_str());

	this->BindShaders();

	parameters.resize(U_TOTAL);
	lights.resize(1);

	// Get a handle for our uniform
	parameters[U_MVP] = glGetUniformLocation(programID, "MVP");
	//parameters[U_MODEL] = glGetUniformLocation(programID, "M");
	//parameters[U_VIEW] = glGetUniformLocation(programID, "V");
	parameters[U_MODELVIEW] = glGetUniformLocation(programID, "MV");
	parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(programID, "MV_inverse_transpose");
	parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(programID, "material.kAmbient");
	parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(programID, "material.kDiffuse");
	parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(programID, "material.kSpecular");
	parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(programID, "material.kShininess");
	parameters[U_LIGHTENABLED] = glGetUniformLocation(programID, "lightEnabled");
	parameters[U_NUMLIGHTS] = glGetUniformLocation(programID, "numLights");
	parameters[U_LIGHT0_TYPE] = glGetUniformLocation(programID, "lights[0].type");
	parameters[U_LIGHT0_POSITION] = glGetUniformLocation(programID, "lights[0].position_cameraspace");
	parameters[U_LIGHT0_COLOR] = glGetUniformLocation(programID, "lights[0].color");
	parameters[U_LIGHT0_POWER] = glGetUniformLocation(programID, "lights[0].power");
	parameters[U_LIGHT0_KC] = glGetUniformLocation(programID, "lights[0].kC");
	parameters[U_LIGHT0_KL] = glGetUniformLocation(programID, "lights[0].kL");
	parameters[U_LIGHT0_KQ] = glGetUniformLocation(programID, "lights[0].kQ");
	parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(programID, "lights[0].spotDirection");
	parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(programID, "lights[0].cosCutoff");
	parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(programID, "lights[0].cosInner");
	parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(programID, "colorTextureEnabled");
	parameters[U_COLOR_TEXTURE] = glGetUniformLocation(programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	parameters[U_TEXT_ENABLED] = glGetUniformLocation(programID, "textEnabled");
	parameters[U_TEXT_COLOR] = glGetUniformLocation(programID, "textColor");

	// Use our shader
	glUseProgram(programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 0, 10);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 0.f, 1.f);

	glUniform1i(parameters[U_NUMLIGHTS], 1);
	glUniform1i(parameters[U_TEXT_ENABLED], 0);

	glUniform1i(parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
}

void SceneManagerCMPlay::RenderLight()
{

}

void SceneManagerCMPlay::RenderBG()
{

}

void SceneManagerCMPlay::RenderStaticObject()
{
	Mesh* drawMesh = resourceManager.retrieveMesh("SKYPLANE");
	drawMesh->textureID = resourceManager.retrieveTexture("SKYPLANE_TOP");

	modelStack.PushMatrix();
	modelStack.Translate(0, 2000, 0);
	Render3DMesh(drawMesh, false);
	modelStack.PopMatrix();

	drawMesh = resourceManager.retrieveMesh("FLOOR");
	drawMesh->textureID = resourceManager.retrieveTexture("GRASS");

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);
	Render3DMesh(drawMesh, false);
	modelStack.PopMatrix();
}

void SceneManagerCMPlay::RenderPlayerStats()
{
	float m_BillBoard;
	Mesh* drawMesh = resourceManager.retrieveMesh("FONT");
	drawMesh->textureID = resourceManager.retrieveTexture("STATUSFONT");

	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		Vector2 CopyVector;
		Vector2 CopyVector2;

		CopyVector.Set(tpCamera.getPosition().x, tpCamera.getPosition().z);
		CopyVector2.Set(ListOfCharacters[i]->GetPosition().x, ListOfCharacters[i]->GetPosition().z);

		Vector2 Direction = (CopyVector - CopyVector2).Normalized();
	
		if (!Direction.IsZero())
		{
			m_BillBoard = Math::RadianToDegree(atan2(Direction.x, Direction.y));
			
		}
		cout << "Angle: " << m_BillBoard << endl;
		/*if (m_BillBoard <= 0)
		{
			m_BillBoard += 360;
		}*/

		if (i == 0)
		{
			cout << m_BillBoard << endl;
		}

		modelStack.PushMatrix();
		modelStack.Translate(ListOfCharacters[i]->GetPosition().x, ListOfCharacters[i]->GetPosition().y + 15.f, ListOfCharacters[i]->GetPosition().z);
		modelStack.Rotate(m_BillBoard, 0, 1, 0);
		modelStack.Scale(2.f, 2.f, 2.f);
		RenderText(drawMesh, ListOfCharacters[i]->PrintState(), resourceManager.retrieveColor("White"));	
		modelStack.PopMatrix();
	}
}

void SceneManagerCMPlay::RenderMobileObject()
{
	//Mesh* drawMesh = resourceManager.retrieveMesh("CIRCLE_RED");
	//template circle

	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == "BOSS")
		{
			CBoss* temp = (CBoss*)ListOfCharacters[i];
			if (temp->GetCastingSkillBool())
			{

				modelStack.PushMatrix();
				modelStack.Translate(temp->GetPosition().x, temp->GetPosition().y, temp->GetPosition().z);
				modelStack.Rotate(-90, 1, 0, 0);
				modelStack.Scale(temp->GetSkillRadius(), temp->GetSkillRadius(), temp->GetSkillRadius());
				Render3DMesh(resourceManager.retrieveMesh("CIRCLE_RED"), true);
				modelStack.PopMatrix();
			}
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(40, 1, 0);
	modelStack.Scale(10, 10, 10);
	Render3DMesh(resourceManager.retrieveMesh("FIREBALL"), false);
	modelStack.PopMatrix();

	FSMApplication();
	RenderPlayerStats();
	sceneGraph->Draw(this);
}

void SceneManagerCMPlay::InitSceneGraph()
{
	this->sceneGraph = new SceneNode();

	//TEST CODE FOR TWO TANKS

	/*this->Tank = new CTank;
	TANK_COUNT = 2;
	string newName = "WARRIOR_";
	newName += to_string(TANK_COUNT);
	Tank->SetID(newName, WARRIOR);
	Tank->RandomSpawn(0, 100);
	ListOfCharacters.push_back(Tank);*/

	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == WARRIOR)
		{
			AddTANK(ListOfCharacters[i]->GetID());
		}
		else if (ListOfCharacters[i]->GetTYPE() == MAGE)
		{
			AddMAGE(ListOfCharacters[i]->GetID());
		}
		else if (ListOfCharacters[i]->GetTYPE() == HEALER)
		{
			AddHEALER(ListOfCharacters[i]->GetID());
		}
		else if (ListOfCharacters[i]->GetTYPE() == BOSS)
		{
			AddBOSS(ListOfCharacters[i]->GetID());
		}
	}
}

void SceneManagerCMPlay::TANK_NODE(CEntity* theTank)
{
	//**********//
	//Warrior	//
	//**********//
	sceneGraph->GetChildNode(theTank->GetID())->GetGameObject()->setPosition(theTank->GetPosition());
	sceneGraph->GetChildNode(theTank->GetID())->GetGameObject()->setRotation(theTank->GetRotation(), 0, 1, 0);

	string IDPlus = theTank->GetID();
	IDPlus += SWORD;
	IDPlus += theTank->GetID().back();

	//sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setPosition(Vector3(0, 0, -5));

	Vector3 Temp;
	Temp.Set(0, theTank->GetChildTranslation(CHILD_1), -5);

	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setPosition(Temp);
	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setRotation(theTank->GetChildRotation(CHILD_1), 0, 0, 1);

	/*IDPlus = theTank->GetID();
	IDPlus += SHIELD;
	IDPlus += theTank->GetID().back();*/

	IDPlus = theTank->GetID();
	IDPlus += SHIELD_PIVOT;
	IDPlus += theTank->GetID().back();

	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setPosition(Vector3(0, 0, 0));
	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setRotation(theTank->GetChildRotation(CHILD_2), 0, 1, 0);

	IDPlus = theTank->GetID();
	IDPlus += SHIELD;
	IDPlus += theTank->GetID().back();

	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setPosition(Vector3(0, 0, 5));
	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setRotation(90, 0, 1, 0);
}

void SceneManagerCMPlay::MAGE_NODE(CEntity* theMage)
{
	//**********//
	//Mage	//
	//**********//	
	sceneGraph->GetChildNode(theMage->GetID())->GetGameObject()->setPosition(theMage->GetPosition());
	sceneGraph->GetChildNode(theMage->GetID())->GetGameObject()->setRotation(theMage->GetRotation(), 0, 1, 0);

	string IDPlus = theMage->GetID();
	IDPlus += STAFF;
	IDPlus += theMage->GetID().back();

	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setPosition(Vector3(0, 0, -5));
	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setRotation(theMage->GetChildRotation(CHILD_1), 0, 0, 1);
}
void SceneManagerCMPlay::HEALER_NODE(CEntity* theHealer)
{
	//**********//
	//Healer	//
	//**********//
	sceneGraph->GetChildNode(theHealer->GetID())->GetGameObject()->setPosition(theHealer->GetPosition());
	sceneGraph->GetChildNode(theHealer->GetID())->GetGameObject()->setRotation(theHealer->GetRotation(), 0, 1, 0);

	string IDPlus = theHealer->GetID();
	IDPlus += ROD;
	IDPlus += theHealer->GetID().back();

	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setPosition(Vector3(0, 0, -5));
	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setRotation(theHealer->GetChildRotation(CHILD_1), 0, 0, 1);
}
void SceneManagerCMPlay::BOSS_NODE(CEntity* theBoss)
{
	//**********//
	//Boss		//
	//**********//
	sceneGraph->GetChildNode(theBoss->GetID())->GetGameObject()->setPosition(theBoss->GetPosition());
	sceneGraph->GetChildNode(theBoss->GetID())->GetGameObject()->setRotation(theBoss->GetRotation(), 0, 1, 0);

	string IDPlus = theBoss->GetID();
	IDPlus += RARM;
	IDPlus += theBoss->GetID().back();

	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setPosition(Vector3(0, 0, -5));
	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setRotation(theBoss->GetChildRotation(CHILD_1), 0, 0, 1);


	IDPlus = theBoss->GetID();
	IDPlus += LARM;
	IDPlus += theBoss->GetID().back();

	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setPosition(Vector3(0, 0, 5));
	sceneGraph->GetChildNode(IDPlus)->GetGameObject()->setRotation(theBoss->GetChildRotation(CHILD_1), 0, 0, 1);
}

void SceneManagerCMPlay::FSMApplication()
{
	for (int i = 0; i < ListOfCharacters.size(); ++i)
	{
		if (ListOfCharacters[i]->GetTYPE() == WARRIOR)
		{
			TANK_NODE(ListOfCharacters[i]);
		}
		else if (ListOfCharacters[i]->GetTYPE() == MAGE)
		{
			MAGE_NODE(ListOfCharacters[i]);
		}
		else if (ListOfCharacters[i]->GetTYPE() == HEALER)
		{
			HEALER_NODE(ListOfCharacters[i]);
		}
		else if (ListOfCharacters[i]->GetTYPE() == BOSS)
		{
			BOSS_NODE(ListOfCharacters[i]);
		}
	}
}

void SceneManagerCMPlay::AddTANK(string ID)
{
	//**********//
	//Warrior	//
	//**********//
	GameObject3D* newModel = new GameObject3D;
	SceneNode* newNode = new SceneNode;
	Mesh* drawMesh = resourceManager.retrieveMesh("WARRIOR_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("WARRIOR");
	newModel->setMesh(drawMesh);
	newModel->setName(ID);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildNode(newNode);

	drawMesh = resourceManager.retrieveMesh("WARRIOR_SWORD_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("WEAPONS");
	newModel = new GameObject3D;
	newNode = new SceneNode;

	string IDPlus = ID;
	IDPlus += SWORD;
	IDPlus += ID.back();

	newModel->setName(IDPlus);
	newModel->setMesh(drawMesh);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildToChildNode(ID, newNode);

	//drawMesh = resourceManager.retrieveMesh("WARRIOR_SHIELD_OBJ");
	//drawMesh->textureID = resourceManager.retrieveTexture("WEAPONS");
	newModel = new GameObject3D;
	newNode = new SceneNode;

	IDPlus = ID;
	IDPlus += SHIELD_PIVOT;
	IDPlus += ID.back();

	newModel->setName(IDPlus);
	//newModel->setMesh(drawMesh);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildToChildNode(ID, newNode);

	drawMesh = resourceManager.retrieveMesh("WARRIOR_SHIELD_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("WEAPONS");
	newModel = new GameObject3D;
	newNode = new SceneNode;

	string theCopy = IDPlus;

	IDPlus = ID;
	IDPlus += SHIELD;
	IDPlus += ID.back();

	newModel->setName(IDPlus);
	newModel->setMesh(drawMesh);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildToChildNode(theCopy, newNode);

	TANK_COUNT++;
}

void SceneManagerCMPlay::AddMAGE(string ID)
{
	//**********//
	//Mage		//
	//**********//
	GameObject3D* newModel = new GameObject3D;
	SceneNode* newNode = new SceneNode;
	Mesh* drawMesh = resourceManager.retrieveMesh("MAGE_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("MAGE");
	newModel = new GameObject3D;
	newNode = new SceneNode;
	newModel->setMesh(drawMesh);
	newModel->setName(ID);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildNode(newNode);

	drawMesh = resourceManager.retrieveMesh("MAGE_STAFF_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("WEAPONS");
	newModel = new GameObject3D;
	newNode = new SceneNode;

	string IDPlus = ID;
	IDPlus += STAFF;
	IDPlus += ID.back();

	newModel->setName(IDPlus);
	newModel->setMesh(drawMesh);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildToChildNode(ID, newNode);

	MAGE_COUNT++;
}

void SceneManagerCMPlay::AddHEALER(string ID)
{
	//**********//
	//Healer	//
	//**********//
	GameObject3D* newModel = new GameObject3D;
	SceneNode* newNode = new SceneNode;
	Mesh* drawMesh = resourceManager.retrieveMesh("HEALER_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("HEALER");
	newModel = new GameObject3D;
	newNode = new SceneNode;
	newModel->setMesh(drawMesh);
	newModel->setName(ID);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildNode(newNode);

	drawMesh = resourceManager.retrieveMesh("HEALER_ROD_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("WEAPONS");
	newModel = new GameObject3D;
	newNode = new SceneNode;

	string IDPlus = ID;
	IDPlus += ROD;
	IDPlus += ID.back();

	newModel->setName(IDPlus);
	newModel->setMesh(drawMesh);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildToChildNode(ID, newNode);

	HEALER_COUNT++;
}

void SceneManagerCMPlay::AddBOSS(string ID)
{
	//**********//
	//Boss		//
	//**********//
	GameObject3D* newModel = new GameObject3D;
	SceneNode* newNode = new SceneNode;
	Mesh* drawMesh = resourceManager.retrieveMesh("BOSS_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("BOSS");
	newModel = new GameObject3D;
	newNode = new SceneNode;
	newModel->setMesh(drawMesh);
	newModel->setName(ID);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildNode(newNode);

	drawMesh = resourceManager.retrieveMesh("BOSS_ARM_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("BOSS");
	newModel = new GameObject3D;
	newNode = new SceneNode;

	string IDPlus = ID;
	IDPlus += RARM;
	IDPlus += ID.back();

	newModel->setName(IDPlus);
	newModel->setMesh(drawMesh);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildToChildNode(ID, newNode);

	drawMesh = resourceManager.retrieveMesh("BOSS_ARM_OBJ");
	drawMesh->textureID = resourceManager.retrieveTexture("BOSS");
	newModel = new GameObject3D;
	newNode = new SceneNode;

	IDPlus = ID;
	IDPlus += LARM;
	IDPlus += ID.back();

	newModel->setName(IDPlus);
	newModel->setMesh(drawMesh);
	newNode->SetGameObject(newModel);
	sceneGraph->AddChildToChildNode(ID, newNode);

	BOSS_COUNT++;
}