#include "pch.h"
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <fstream>
#include "Game.h"

//Basic game functions
#pragma region gameFunctions											
void Start()
{
	// initialize game resources here
	InitTextures(g_NamedTexturesArr, g_TexturesSize, g_Numbers, g_GridSize);
	InitializeRooms(g_Level);
	SetObstacles(g_CellArr, g_NrRows, g_NrCols);
	InitWeapons();
	InitPlayer(g_Player, g_CellArr, g_PlayerSprites);
	InitInteractables();
	InitEnemies(g_EnemyArr, g_EnemyArrSize, g_CellArr, g_GridSize);
}

void Draw()
{
	ClearBackground(0,0,0);



	switch (g_Game)
	{
	case GameStates::startScreen:
		DrawStartScreen();
		
		break;
	case GameStates::playing:
		DrawGridTextures(g_CellArr, g_NrRows, g_NrCols);
		DrawEnemies(g_EnemyArr, g_EnemyArrSize);
		DrawEnemyHealthBars(g_EnemyArr);
		DrawPlayer(g_Player, g_PlayerSprites);
		DrawWeaponInventory(g_Player);
		DrawPlayerHealth(g_Player);
		break;
	case GameStates::gameOverScreen:
		DrawEndScreen();
		break;
	case GameStates::restarting:
		break;
	default:
		break;
	}


	//Comment this out to disable the debug grid
	
	//DrawDebugGrid(g_CellArr, g_NrRows, g_NrCols);

}

void Update(float elapsedSec)
{
	UpdateAnimationPos(elapsedSec, g_Player);
	UpdateEnemies(elapsedSec, g_EnemyArr, g_EnemyArrSize, g_CellArr, g_GridSize);
	ProcessMovement(g_Player, g_CellArr, g_GridSize, g_PlayerSprites,elapsedSec);
	ProcessAnimState(g_Player, g_PlayerSprites);
	UpdatePlayerSprites(g_PlayerSprites, elapsedSec);
	SetGameOverScreen(g_Player);
}

void End()
{
	// free game resources here
	DeleteTextures();
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_TAB:
		CycleWeapons(g_Player);
		break;
	case SDLK_SPACE:
		Interact(g_Player, g_CellArr, g_GridSize, g_CurrentRoom);
		break;
	case SDLK_e:
		// EnterRoom(g_Player, g_CellArr, g_GridSize);
		break;
	case SDLK_k:
		PlayerDebugInfo();
		break;
	case SDLK_o: 
		{
			// opens all doors. Remove when room clearing is implemented
		OpenDoors(g_CellArr, g_GridSize);

		std::cerr << "ALL DOOR TEXTURES ARE NOW SET TO OPEN" << std::endl;
		break;
		}
	}
}

void OnKeyUpEvent(SDL_Keycode key)
{
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	ProcessFacing(g_Player, e);
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case (SDL_BUTTON_LEFT):
		ClickStart(e);
		UseWeapon(g_Player);
		break;
	}
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{

}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here
#pragma region utilFunctions
// Utils
void PlayerDebugInfo()
{
	int playerIndex{ GetPlayerGridIndex(g_Player, g_CellArr, g_GridSize) };
	std::string StandingOnTextureWithName{ FetchTextureName(g_CellArr[playerIndex].texture) };
	std::cerr << "playerindex: " << playerIndex << std::endl;
	std::cerr << "StandingOnTextureWithName: " << StandingOnTextureWithName << std::endl;

}
int GetIndex(const int rowIdx, const int colIdx, const int nrCols)
{
	return rowIdx * nrCols + colIdx;
}
int GetPlayerGridIndex(const Player& player, Cell cellArr[], const int arrSize)
{
	int index{};

	for (int i = 0; i < arrSize; i++)
	{
		if (player.dstRect.bottom == cellArr[i].dstRect.bottom && player.dstRect.left == cellArr[i].dstRect.left)
		{
			index = i;
		}
	}

	return index;
}
Point2f GetPlayerRowColumn(Player& player, Cell cellArr[], int nrRows, int nrCols)
{
	Point2f playerpos{ player.dstRect.left, player.dstRect.bottom };
	Point2f gridpos{};

	for (int i = 0; i < nrRows; i++)
	{
		for (int j = 0; j < nrCols; j++)
		{
			if (playerpos.x == cellArr[GetIndex(i,j,nrCols)].dstRect.left && 
				playerpos.y == cellArr[GetIndex(i, j, nrCols)].dstRect.bottom)
			{
				gridpos.x = float(GetIndex(i, j, nrCols) / nrCols);
				gridpos.y = float(GetIndex(i, j, nrCols) % nrCols);
			}
		}
	}
	return gridpos;
}
float CalculateAngleToMouse(Point2f playerCenter, Point2f mousePos)
{
	float distX{ mousePos.x - playerCenter.x };
	float distY{ mousePos.y - playerCenter.y };
	float angle{ atan2f(distY, distX) };

	return angle;
}
bool HasEnemy(const int cellIndex, Enemy enemyArr[], int enemyArrSize)
{
	for (int index{}; index < enemyArrSize; ++index)
	{
		if (GetEnemyGridIndex(g_EnemyArr[index], g_CellArr, g_GridSize) == cellIndex && enemyArr[index].health > 0.f)
		{
			return true;
		}
	}
	return false;
}
void SetPlayerPos(Player& player, Cell cellArr[], int dstIndex)
{
	player.dstRect = cellArr[dstIndex].dstRect;
}
void TeleportPlayer(const int index, Player& player)
{
	player.dstRect = g_CellArr[index].dstRect;
	player.animationPos = g_CellArr[index].dstRect;
}
bool IsPointInRect(const Rectf& rectangle, const Point2f& point)
{
	return (point.x >= rectangle.left &&
		point.x <= rectangle.left + rectangle.width &&
		point.y >= rectangle.bottom &&
		point.y <= rectangle.bottom + rectangle.height);

	return false;

}
#pragma endregion utilFunctions

#pragma region textureHandling
// Texture Handling
void InitTextures(NamedTexture namedTextureArr[], const int arrSize, Texture textureNumbersArr[], const int numbersArrSize)
{
	LoadTexturesFromFolder("Resources", namedTextureArr, arrSize);

	// Load numbers 0 through 95 for debug grid
	 for (int i = 0; i < numbersArrSize; i++)
	{
		bool success = TextureFromString(std::to_string(i), "resources/Font.ttf", 13, g_White, textureNumbersArr[i]);
		if (!success)
		{
			std::cout << "Loading number " + std::to_string(i) + " from Font.otf failed" << std::endl;
		}
	}
}
void LoadTexturesFromFolder(std::string folderPath, NamedTexture namedTextureArr[], const int arrSize)
{
	std::fstream file;
	file.open(folderPath + "/textures.txt", std::ios::in);
	if (!file)
	{
		std::cout << "CAN NOT FIND TEXTURES FILE\n";
	}

	std::string textureFileName{};
	std::string textureInternalName{};
	std::string boolBuffer{};
	bool isAddingToBuffer{ false };
	bool isPastFirstComma{ false };
	bool isPastSecondComma{ false };
	char currentChar;
	int index{};

	while (file)
	{
		currentChar = file.get();
		if (currentChar == '}')
		{
			if (boolBuffer == "true")
			{
				LoadTexture(folderPath + "/" + textureFileName, namedTextureArr[index], textureInternalName, true);
			}
			else
			{
				LoadTexture(folderPath + "/" + textureFileName, namedTextureArr[index], textureInternalName, false);
			}
			isAddingToBuffer = false;
			textureFileName = "";
			textureInternalName = "";
			boolBuffer = "";
			isPastFirstComma = false;
			isPastSecondComma = false;
			++index;
		}
		else if (currentChar == ',')
		{
			if (isPastFirstComma == true) isPastSecondComma = true;
			else isPastFirstComma = true;
		}
		else if (isAddingToBuffer == true)
		{
			if (isPastSecondComma)
			{
				boolBuffer += currentChar;
			}
			else if (isPastFirstComma)
			{
				textureInternalName += currentChar;
			}
			else
			{
				textureFileName += currentChar;
			}
		}
		else if (currentChar == '{')
		{
			isAddingToBuffer = true;
		}
	}

	file.close();
}
void LoadTexture(const std::string texturePath, NamedTexture& namedTexture, std::string name, bool isObstacle) {
	bool success = TextureFromFile(texturePath, namedTexture.texture);
	namedTexture.name = name;
	namedTexture.isObstacle = isObstacle;
	if (!success)
	{
		std::cout << "Loading of " << name << " texture failed" << std::endl;
	}
}

void DeleteTextures()
{
	// Delete player textures in player struct
	DeleteTexture(g_Player.sprite.texture);

	// Delete named textures array
	for (int i = 0; i < g_TexturesSize; i++)
	{
		DeleteTexture(g_NamedTexturesArr[i].texture);
	}

	// Delete enemy textures in enemy array
	for (int i = 0; i < g_EnemyArrSize; i++)
	{
		DeleteTexture(g_EnemyArr[i].texture);
	}

	// Delete textures stored in the grid array
	for (int i = 0; i < g_GridSize; i++)
	{
		DeleteTexture(g_CellArr[i].texture);
	}

	// Delete number textures array
	for (int i = 0; i < g_GridSize; i++)
	{
		DeleteTexture(g_Numbers[i]);
	}

	// Delete textures stored in the g_PlayerSprites array
	for (int i = 0; i < g_PlayerSpritesSize; i++)
	{
		DeleteTexture(g_PlayerSprites[i].texture);

	}

}
Texture FetchTexture(const std::string& textureName)
{
	for (int index{}; index < g_TexturesSize; ++index)
	{
		if (textureName == g_NamedTexturesArr[index].name)
			return g_NamedTexturesArr[index].texture;
	}
	return g_NamedTexturesArr[0].texture;
}
std::string FetchTextureName(const Texture& texture)
{
	for (int index{}; index < g_TexturesSize; ++index)
	{
		if (g_NamedTexturesArr[index].texture.id == texture.id)
			return g_NamedTexturesArr[index].name;
	}
	return g_NamedTexturesArr[0].name;
}


#pragma endregion textureHandling

#pragma region gridHandling
// Grid Handling
void InitGrid(Cell cellArr[], int nrRows, int nrCols)
{
	const float cellSize{ 64.f };

	Point2f pos{ 0.5f * (g_WindowWidth - (nrCols * cellSize)),
				 0.5f * (g_WindowHeight + ((nrRows - 2) * cellSize)) }; // ensures grid is always centered

	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			cellArr[GetIndex(row, col, nrCols)].dstRect.bottom = pos.y;
			cellArr[GetIndex(row, col, nrCols)].dstRect.left = pos.x;
			cellArr[GetIndex(row, col, nrCols)].dstRect.width = cellSize;
			cellArr[GetIndex(row, col, nrCols)].dstRect.height = cellSize;

			pos.x += cellSize;
		}
		pos.x -= nrCols * cellSize;
		pos.y -= cellSize;

	}

}
void DrawDebugGrid(Cell cellArr[], int nrRows, int nrCols)
{
	SetColor(g_White);
	for (int row = 0; row < nrRows; row++)
	{
		for (int col = 0; col < nrCols; col++)
		{
			Point2f pos{ cellArr[GetIndex(row, col, nrCols)].dstRect.left, 
						 cellArr[GetIndex(row, col, nrCols)].dstRect.bottom };

			DrawTexture(g_Numbers[GetIndex(row, col, nrCols)], pos);
			DrawRect(cellArr[GetIndex(row, col, nrCols)].dstRect);
		}
	}
}

void DrawGridTextures(Cell cellArr[], int nrRows, int nrCols)
{
	int cellAmount{ nrRows * nrCols };
	for (int index{}; index < cellAmount; ++index)
	{	
		DrawTexture(cellArr[index].texture, cellArr[index].dstRect);
	}
}
void SetObstacles(Cell cellArr[], int nrRows, int nrCols)
{
	int cellAmount{ nrRows * nrCols };
	for (int index{}; index < cellAmount; ++index)
	{
		// this if statement sets all appropriate cells to 'isObstacle = true' for the appropriate textures
		// Might need global array to store obstacles in the future to shorten this if statement
		Cell& currentCell = cellArr[index];
		for (int indexNamedTextures{}; indexNamedTextures < g_TexturesSize; ++indexNamedTextures)
		{
			if (currentCell.texture.id == g_NamedTexturesArr[indexNamedTextures].texture.id)
			{
				currentCell.isObstacle = g_NamedTexturesArr[indexNamedTextures].isObstacle;
			}
		}
	}


}
#pragma endregion gridHandling

#pragma region playerHandling
// Player Handling
void InitPlayer(Player& player, Cell cellArr[], Sprite Sprites[])
{
	int idle{ 0 };

	InitPlayerSprites(Sprites);
	player.sprite.texture = Sprites[idle].texture;
	player.animState = AnimStates::idleRight;
	player.dstRect = cellArr[97].dstRect; // 97 is starting pos in starting room
	player.animationPos = player.dstRect;
	player.health = player.maxHealth;
	player.weaponInventory[0] = FetchWeapon("basic_sword");
	player.selectedWeapon = 0;
}
void DrawPlayer(const Player& player, Sprite Sprites[])
{
	Rectf srcRect{};

	switch (player.animState)
	{
	case AnimStates::idleRight:
		srcRect.width = Sprites[int(AnimStates::idleRight)].texture.width / Sprites[int(AnimStates::idleRight)].cols;
		srcRect.height = Sprites[int(AnimStates::idleRight)].texture.height;
		srcRect.left = Sprites[int(AnimStates::idleRight)].currentFrame * srcRect.width;
		srcRect.bottom = srcRect.height;
		break;
	case AnimStates::idleLeft:
		srcRect.width = Sprites[int(AnimStates::idleLeft)].texture.width / Sprites[int(AnimStates::idleLeft)].cols;
		srcRect.height = Sprites[int(AnimStates::idleLeft)].texture.height;
		srcRect.left = Sprites[int(AnimStates::idleLeft)].currentFrame * srcRect.width;
		srcRect.bottom = srcRect.height;
		break;
	case AnimStates::idleDown:
		srcRect.width = Sprites[int(AnimStates::idleDown)].texture.width / Sprites[int(AnimStates::idleDown)].cols;
		srcRect.height = Sprites[int(AnimStates::idleDown)].texture.height;
		srcRect.left = Sprites[int(AnimStates::idleDown)].currentFrame * srcRect.width;
		srcRect.bottom = srcRect.height;
		break;
	case AnimStates::idleUp:
		srcRect.width = Sprites[int(AnimStates::idleUp)].texture.width / Sprites[int(AnimStates::idleUp)].cols;
		srcRect.height = Sprites[int(AnimStates::idleUp)].texture.height;
		srcRect.left = Sprites[int(AnimStates::idleUp)].currentFrame * srcRect.width;
		srcRect.bottom = srcRect.height;
		break;
	case AnimStates::runRight:
		srcRect.width = Sprites[int(AnimStates::runRight)].texture.width / Sprites[int(AnimStates::runRight)].cols;
		srcRect.height = Sprites[int(AnimStates::runRight)].texture.height;
		srcRect.left = Sprites[int(AnimStates::runRight)].currentFrame * srcRect.width;
		srcRect.bottom = srcRect.height;
		break;
	case AnimStates::runDown:
		srcRect.width = Sprites[int(AnimStates::runDown)].texture.width / Sprites[int(AnimStates::runDown)].cols;
		srcRect.height = Sprites[int(AnimStates::runDown)].texture.height;
		srcRect.left = Sprites[int(AnimStates::runDown)].currentFrame * srcRect.width;
		srcRect.bottom = srcRect.height;
		break;
	case AnimStates::runUp:
		srcRect.width = Sprites[int(AnimStates::runUp)].texture.width / Sprites[int(AnimStates::runUp)].cols;
		srcRect.height = Sprites[int(AnimStates::runUp)].texture.height;
		srcRect.left = Sprites[int(AnimStates::runUp)].currentFrame * srcRect.width;
		srcRect.bottom = srcRect.height;
		break;
	case AnimStates::runLeft:
		srcRect.width = Sprites[int(AnimStates::runLeft)].texture.width / Sprites[int(AnimStates::runLeft)].cols;
		srcRect.height = Sprites[int(AnimStates::runLeft)].texture.height;
		srcRect.left = Sprites[int(AnimStates::runLeft)].currentFrame * srcRect.width;
		srcRect.bottom = srcRect.height;
		break;
	case AnimStates::hit:
		srcRect.width = Sprites[int(AnimStates::hit)].texture.width;
		srcRect.height = Sprites[int(AnimStates::hit)].texture.height;
		srcRect.left = 0;
		srcRect.bottom = srcRect.height;
		break;
	}
	DrawTexture(player.sprite.texture, player.animationPos, srcRect);

}
void UpdateAnimationPos(float elapsedSec, Player& player)
{
	const float xDiff{ player.animationPos.left - player.dstRect.left };
	const float yDiff{ player.animationPos.bottom - player.dstRect.bottom };
	const float cellSize{ player.dstRect.height };
	const float speedModifier{ player.speedModifier };
	if (abs(xDiff) > 5.0f)
	{
		player.animationPos.left += cellSize * elapsedSec * speedModifier * (xDiff < 0.1f ? 1 : -1);
	}
	else
	{
		player.animationPos.left = player.dstRect.left;
	}
	
	if (abs(yDiff) > 5.0f)
	{
		
		player.animationPos.bottom += cellSize * elapsedSec * speedModifier * (yDiff < 0.1f ? 1 : -1);
	}
	else
	{
		player.animationPos.bottom = player.dstRect.bottom;
	}
}

void DrawWeaponInventory(const Player& player)
{
	for (int index{}; index < g_WeaponInventorySize; ++index)
	{
		const float slotHeight{ 64.f };
		const float border{2.0f};
		Rectf location{ 0, g_WindowHeight - (index + 1) * (slotHeight + border), slotHeight, slotHeight };
		SetColor(g_White);
		DrawRect(location);
		DrawTexture(player.weaponInventory[index].texture, location);
		if (player.weaponInventory[index].texture.id == NULL)
		{
			SetColor(g_Grey);
			FillRect(location);
		}
		if (player.selectedWeapon == index)
		{
			SetColor(g_Red);
			DrawRect(location, 2.f);
		}
	}
}
void DrawHealthBar(Rectf animationPosRect, float health, float maxHealth)
{
	Rectf healthBar{};
	const float healthPercentage{ health / maxHealth };
	const float healthBarNarrower{ 10.f };
	healthBar.height = 8.f;
	healthBar.left = animationPosRect.left + healthBarNarrower;
	healthBar.bottom = animationPosRect.bottom + animationPosRect.height;
	healthBar.width = healthPercentage * (animationPosRect.width - 2 * healthBarNarrower);
	Rectf healthBarBackGround{ healthBar };
	healthBarBackGround.width = animationPosRect.width - 2 * healthBarNarrower;

	SetColor(g_Grey);
	FillRect(healthBarBackGround);

	if      (healthPercentage > 0.5f)  SetColor(g_Green);
	else if (healthPercentage > 0.3f)  SetColor(g_Gold);
	else if (healthPercentage > 0.1f)  SetColor(g_Red);
	else							   SetColor(g_DarkRed);
	FillRect(healthBar);
}
void DrawPlayerHealth(const Player& player)
{
	if (player.health != player.maxHealth) 
	{
		DrawHealthBar(player.animationPos, player.health, player.maxHealth);
	}
}

void Interact(Player& player, Cell cellArr[], const int cellArrSize, Room& currentRoom)
{
	int playerIndex{ GetPlayerGridIndex(player, cellArr, cellArrSize) };
	if (cellArr[playerIndex].texture.id == FetchTexture("door_open").id
		|| cellArr[playerIndex].texture.id == FetchTexture("door_transparent_open").id
		|| cellArr[playerIndex].texture.id == FetchTexture("door_closed").id
		|| cellArr[playerIndex].texture.id == FetchTexture("door_transparent_closed").id)
	{
		GoToLinkedRoom(currentRoom, playerIndex);
	}
}

void UseWeapon(const Player& player)
{
	if (player.weaponInventory[player.selectedWeapon].type == WeaponType::sword)
	{
		UseSword(player);
	}
}
void UseSword(const Player& player) 
{
	int playerIndex{ GetPlayerGridIndex(player, g_CellArr, g_GridSize) };
	if (player.facing == Direction::up) {
		const int tilesAmount{ 3 };
		int tilesToScan[tilesAmount]{ playerIndex - g_NrCols - 1, playerIndex - g_NrCols, playerIndex - g_NrCols + 1 };
		AttackOnTiles(player, tilesToScan, tilesAmount);
	}
	else if (player.facing == Direction::down) 
	{
		const int tilesAmount{ 3 };
		int tilesToScan[tilesAmount]{ playerIndex + g_NrCols - 1, playerIndex + g_NrCols, playerIndex + g_NrCols + 1 };
		AttackOnTiles(player, tilesToScan, tilesAmount);
	}
	else if (player.facing == Direction::right) 
	{
		const int tilesAmount{ 3 };
		int tilesToScan[tilesAmount]{ playerIndex + 1, playerIndex + 1 - g_NrCols, playerIndex + 1 + g_NrCols };
		AttackOnTiles(player, tilesToScan, tilesAmount);
	}
	else if (player.facing == Direction::left) 
	{
		const int tilesAmount{ 3 };
		int tilesToScan[tilesAmount]{ playerIndex - 1, playerIndex - 1 - g_NrCols, playerIndex - 1 + g_NrCols };
		AttackOnTiles(player, tilesToScan, tilesAmount);
	}
}
void CycleWeapons(Player& player)
{
	if (player.selectedWeapon < g_WeaponInventorySize - 1)
	{
		++player.selectedWeapon;
	}
	else
	{
		player.selectedWeapon = 0;
	}
}

void AttackOnTiles(const Player& player, int tilesToScan[], int tilesAmount) {
	for (int currentTile{}; currentTile < tilesAmount; ++currentTile)
	{
		for (int index{}; index < g_EnemyArrSize; ++index)
		{
			if (GetEnemyGridIndex(g_EnemyArr[index], g_CellArr, g_GridSize) == tilesToScan[currentTile])
			{
				if (g_EnemyArr[index].health > 0.f)
				{
					g_EnemyArr[index].health -= player.weaponInventory[player.selectedWeapon].damageOutput;
				}
				else
				{
					g_EnemyArr[index].health = 0;
				}
			}
		}
	}
}
#pragma endregion playerHandling

#pragma region weaponHandling
// Weapon Handling
void InitWeapons()
{
	g_Weapons[0] = InitializeWeapon("basic_sword", "basic_sword_up", WeaponType::sword, 2.0f);
}
Weapon InitializeWeapon(const std::string& weaponName, const std::string& textureName, const WeaponType& type, float damage)
{
	Weapon createdWeapon{};
	createdWeapon.name = weaponName;
	createdWeapon.texture = FetchTexture(textureName);
	createdWeapon.type = type;
	createdWeapon.damageOutput = damage;
	return createdWeapon;
}
Weapon FetchWeapon(const std::string& name)
{
	for (int index{}; index < g_WeaponsInGame; ++index)
	{
		if (g_Weapons[index].name == name)
		{
			return g_Weapons[index];
		}
	}
	Weapon no_weapon{ InitializeWeapon("no_weapon", "not_found", WeaponType::sword, 0.0f) };
	return no_weapon;

}
#pragma endregion weaponHandling

#pragma region interactableHandling
// Interactable Handling
void InitInteractables() 
{
	InitializeInteractable("basic_sword", InteractableType::weaponDrop);
}
Interactable InitializeInteractable(const std::string& linkedItem, InteractableType type)
{
	Interactable initializedInteractable{};
	initializedInteractable.type = type;
	if (type == InteractableType::weaponDrop)
	{
		initializedInteractable.linkedWeapon = FetchWeapon(linkedItem);
	}
	else if (type == InteractableType::itemDrop)
	{
	}
	return initializedInteractable;
}
#pragma endregion interactableHandling

#pragma region enemyHandling
// Enemy Handling
int GetRandomSpawn(Cell cellArr[], const int cellArrSize)
{
	int spawn{};
	int playerIndex{ GetPlayerGridIndex(g_Player, cellArr, cellArrSize) };

	do {
		spawn = rand() % g_GridSize;
	} while (spawn == playerIndex || cellArr[spawn].isObstacle == true || HasEnemy(spawn, g_EnemyArr, g_EnemyArrSize));
	return spawn;
}
void InitEnemies(Enemy enemyArr[], const int enemyArrSize, Cell cellArr[], const int cellArrSize)
{
	int enemyNumber{};
	enemyArr[enemyNumber] = InitializeEnemy("bat");



	for (int index{}; index < enemyNumber + 1; ++index)
	{
		enemyArr[index].dstRect = cellArr[GetRandomSpawn(cellArr, cellArrSize)].dstRect;
		enemyArr[index].animationPos = enemyArr[index].dstRect;
	}
}

void DamageAllEnemies(Enemy EnemyArr[], const int enemyArrSize)
{
	for (int index{}; index < enemyArrSize; ++index)
	{
		EnemyArr[index].health -= g_Player.weaponInventory[g_Player.selectedWeapon].damageOutput;
	}
}

Enemy InitializeEnemy(const EnemyType& type, const std::string& textureName, float maxHealth, float damage, float timePerAction, int viewRange)
{
	Enemy enemy{};
	enemy.type = type;
	enemy.texture = FetchTexture(textureName);
	enemy.maxHealth = maxHealth;
	enemy.health = enemy.maxHealth;
	enemy.viewRange = viewRange;
	enemy.damageOutput = damage;
	enemy.timePerAction = timePerAction;
	return enemy;
}
Enemy InitializeEnemy(std::string enemyName)
{
	if (enemyName == "zombie") {
		return InitializeEnemy(EnemyType::basic, "enemy_zombie", 20.f, 5.f, 1.0f, 10);
	}
	if (enemyName == "bat") {
		return InitializeEnemy(EnemyType::basic, "enemy_bat", 4.f, 1.f, 0.5f, 4);
	}
	if (enemyName == "skeleton") {
		return InitializeEnemy(EnemyType::ranged, "enemy_skeleton", 5.f, 2.f, 0.8f, 10);
	}
	else
		return InitializeEnemy(EnemyType::basic, "not_found", 1.0f, 0.0f, 100.f, 0);
}

void DrawEnemies(Enemy enemyArr[], int arrSize) 
{
	for (int index{}; index < arrSize; ++index)
	{
		DrawTexture(enemyArr[index].texture, enemyArr[index].animationPos);
	}
}
void UpdateAnimationPos(float elapsedSec, Enemy& enemy)
{
	const float xDiff{ enemy.animationPos.left - enemy.dstRect.left };
	const float yDiff{ enemy.animationPos.bottom - enemy.dstRect.bottom };
	const float cellSize{ enemy.dstRect.height };
	const float speedModifier{ 5.f };
	if (abs(xDiff) > 1.0f)
	{
		enemy.animationPos.left += cellSize * elapsedSec * speedModifier * (xDiff < 0.1f ? 1 : -1);
	}
	else
	{
		enemy.animationPos.left = enemy.dstRect.left;
	}

	if (abs(yDiff) > 1.0f)
	{

		enemy.animationPos.bottom += cellSize * elapsedSec * speedModifier * (yDiff < 0.1f ? 1 : -1);
	}
	else
	{
		enemy.animationPos.bottom = enemy.dstRect.bottom;
	}
}
void DrawEnemyHealth(const Enemy& enemy)
{
	if (enemy.health != enemy.maxHealth)
	{
		DrawHealthBar(enemy.animationPos, enemy.health, enemy.maxHealth);
	}
}
void DrawEnemyHealthBars(Enemy enemyArr[])
{
	for (int index{}; index < g_EnemyArrSize; ++index)
	{
		if (enemyArr[index].health > 0) {
			DrawEnemyHealth(enemyArr[index]);
		}
	}
}
void DestroyEnemy(Enemy& enemy) 
{
	Enemy defaultEnemy{};
	enemy = defaultEnemy;
}

int GetEnemyGridIndex(Enemy& enemy, Cell cellArr[], const int arrSize)
{
	int index{};

	for (int i = 0; i < arrSize; i++)
	{
		if (enemy.dstRect.bottom == cellArr[i].dstRect.bottom && enemy.dstRect.left == cellArr[i].dstRect.left)
		{
			index = i;
		}
	}

	return index;
}

// Enemy AI
void BasicEnemyAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize)
{
	float timeBetweenActions{ enemy.timePerAction };
	const int enemyRange{ enemy.viewRange };

	const int enemyIndex{ GetEnemyGridIndex(enemy, cellArr, cellArrSize) };
	const int playerIndex{ GetPlayerGridIndex(g_Player, cellArr, cellArrSize) };
	const int indexDiffX{ playerIndex % g_NrCols - enemyIndex % g_NrCols };
	const int indexDiffY{ playerIndex / g_NrCols - enemyIndex / g_NrCols };
	const int distX{ abs(indexDiffX) };
	const int distY{ abs(indexDiffY) };

	enemy.timeTracker += elapsedSec;

	if (enemy.timeTracker > timeBetweenActions)
	{
		enemy.timeTracker -= timeBetweenActions;
		int movementDecider{ rand() % 2 };
		bool isInRangeDiagonal{ indexDiffX != 0 && distX <= enemyRange
							 && indexDiffY != 0 && distY <= enemyRange };
		bool isInRangeX{ indexDiffY == 0 && distX > 1 && distX <= enemyRange };
		bool isInRangeY{ indexDiffX == 0 && distY > 1 && distY <= enemyRange };

		bool isNextToPlayerX{ distX == 1 && distY == 0 };
		bool isNextToPlayerY{ distX == 0 && distY == 1 };

		int newIndexX{ enemyIndex + (indexDiffX > 0 ? 1 : -1) };
		int newIndexY{ enemyIndex + (indexDiffY > 0 ? g_NrCols : -g_NrCols) };

		bool canGoHoriDiagCheck{ movementDecider == 1 && isInRangeDiagonal && !cellArr[newIndexX].isObstacle 
			&& !HasEnemy(newIndexX, g_EnemyArr, g_EnemyArrSize) };
		bool canGoVertDiagCheck{ isInRangeDiagonal && !cellArr[newIndexY].isObstacle 
			&& !HasEnemy(newIndexY, g_EnemyArr, g_EnemyArrSize)	};
		bool canGoHorHorCheck{   isInRangeX && !cellArr[newIndexX].isObstacle && !HasEnemy(newIndexX, g_EnemyArr, g_EnemyArrSize) };

		bool canGoVertHorCheck{  isInRangeX && (cellArr[newIndexX].isObstacle ||  HasEnemy(newIndexX, g_EnemyArr, g_EnemyArrSize))
											&& !cellArr[newIndexY].isObstacle && !HasEnemy(newIndexY, g_EnemyArr, g_EnemyArrSize) };

		bool canGoVertVertCheck{ isInRangeY	&& !cellArr[newIndexY].isObstacle && !HasEnemy(newIndexY, g_EnemyArr, g_EnemyArrSize) };

		bool canGoHorVertCheck{  isInRangeY && (cellArr[newIndexY].isObstacle ||  HasEnemy(newIndexY, g_EnemyArr, g_EnemyArrSize))
											&& !cellArr[newIndexX].isObstacle && !HasEnemy(newIndexX, g_EnemyArr, g_EnemyArrSize) };

		if (canGoHoriDiagCheck || canGoHorHorCheck || canGoHorVertCheck)
		{
			enemy.dstRect = cellArr[newIndexX].dstRect;
		}
		else if (canGoVertDiagCheck || canGoVertVertCheck || canGoVertHorCheck)
		{
			enemy.dstRect = cellArr[newIndexY].dstRect;
		}
		else if (isNextToPlayerX 
			&&   g_Player.health > 0.f)
		{
			g_Player.sprite.texture = FetchTexture("knight_hit_anim");
			g_Player.animState = AnimStates::hit;
			g_Player.health -= enemy.damageOutput;
			enemy.animationPos.left -= (enemy.animationPos.left - g_Player.dstRect.left) / 2;
		}
		else if (isNextToPlayerY && g_Player.health > 0.f)
		{
			g_Player.sprite.texture = FetchTexture("knight_hit_anim");
			g_Player.animState = AnimStates::hit;
			g_Player.health -= enemy.damageOutput;
			enemy.animationPos.bottom -= (enemy.animationPos.bottom - g_Player.dstRect.bottom) / 2;
		}
	}
}
void RangedEnemyAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize)
{
	float timeBetweenActions{ enemy.timePerAction };
	const int enemyRange{ enemy.viewRange };

	const int enemyIndex{ GetEnemyGridIndex(enemy, cellArr, cellArrSize) };
	const int playerIndex{ GetPlayerGridIndex(g_Player, cellArr, cellArrSize) };
	const int indexDiffX{ playerIndex % g_NrCols - enemyIndex % g_NrCols };
	const int indexDiffY{ playerIndex / g_NrCols - enemyIndex / g_NrCols };
	const int distX{ abs(indexDiffX) };
	const int distY{ abs(indexDiffY) };

	enemy.timeTracker += elapsedSec;

	if (enemy.timeTracker > timeBetweenActions)
	{
		enemy.timeTracker -= timeBetweenActions;
		int movementDecider{ rand() % 2 };
		bool isInRangeDiagonal{ indexDiffX != 0 && distX <= enemyRange
							 && indexDiffY != 0 && distY <= enemyRange };
		bool isInRangeX{ indexDiffY == 0 && distX > 1 && distX <= enemyRange };
		bool isInRangeY{ indexDiffX == 0 && distY > 1 && distY <= enemyRange };

		bool isNextToPlayerX{ distX == 1 && distY == 0 };
		bool isNextToPlayerY{ distX == 0 && distY == 1 };

		if (isInRangeDiagonal)
		{
			if (distX > distY && !cellArr[enemyIndex + (indexDiffX > 0 ? -g_NrCols : g_NrCols)].isObstacle)
			{
				enemy.dstRect = cellArr[enemyIndex + (indexDiffX > 0 ? -g_NrCols : g_NrCols)].dstRect;
			}
			else if (!cellArr[enemyIndex + (indexDiffX > 0 ? 1 : -1)].isObstacle)
			{
				enemy.dstRect = cellArr[enemyIndex + (indexDiffX > 0 ? 1 : -1)].dstRect;
			}
		}
	}
}
void UpdateEnemies(float elapsedSec, Enemy enemyArr[], int enemyArrSize, Cell cellArr[], int cellArrSize)
{
	for (int index{}; index < enemyArrSize; ++index)
	{
		if (enemyArr[index].health <= 0.f && enemyArr[index].maxHealth != 0)
		{ 
			DestroyEnemy(enemyArr[index]);
		} 
		else if (enemyArr[index].type == EnemyType::basic)
		{
			BasicEnemyAI(elapsedSec, enemyArr[index], cellArr, cellArrSize);
		}
		else if (enemyArr[index].type == EnemyType::ranged)
		{
			RangedEnemyAI(elapsedSec, enemyArr[index], cellArr, cellArrSize);
		}
		UpdateAnimationPos(elapsedSec, enemyArr[index]);
	}
}
#pragma endregion enemyHandling

#pragma region playerInputHandling
// Input Handling
void ProcessMovement(Player& player, Cell cellArr[], const int arrSize, Sprite Sprites[], float elapsedSec)
{
	float minTimeBetweenActions{ 1.f / player.speedModifier }; // THIS IS PROBABLY THE PROBLEM
	int newIndex{};
	const int nrCols{ g_NrCols };
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	player.timeTracker += elapsedSec;
	bool IsReadyToAct{ player.timeTracker > minTimeBetweenActions };

	if (pStates[SDL_SCANCODE_A] && IsReadyToAct )
	{
		player.timeTracker = 0;
		newIndex = GetPlayerGridIndex(player, cellArr, arrSize) - 1;
		if (cellArr[newIndex].isObstacle == false && !HasEnemy(newIndex, g_EnemyArr, g_EnemyArrSize) )
		{
			player.dstRect = cellArr[newIndex].dstRect;
		}
	}
	if (pStates[SDL_SCANCODE_D] && IsReadyToAct) 
	{
		player.timeTracker = 0;
		newIndex = GetPlayerGridIndex(player, cellArr, arrSize) + 1;
		if (cellArr[newIndex].isObstacle == false && !HasEnemy(newIndex, g_EnemyArr, g_EnemyArrSize))
		{
			player.dstRect = cellArr[newIndex].dstRect;
		}
	}
	if (pStates[SDL_SCANCODE_W] && IsReadyToAct) 
	{
		player.timeTracker = 0;
		newIndex = GetPlayerGridIndex(player, cellArr, arrSize) - nrCols;
		if (newIndex < 0 || newIndex > g_GridSize-1)
		{
			newIndex = GetPlayerGridIndex(player, cellArr, arrSize);
		}
		if (cellArr[newIndex].isObstacle == false && !HasEnemy(newIndex, g_EnemyArr, g_EnemyArrSize))
		{
			player.dstRect = cellArr[newIndex].dstRect;
		}
	}
	if (pStates[SDL_SCANCODE_S] && IsReadyToAct) 
	{
		player.timeTracker = 0;
		newIndex = GetPlayerGridIndex(player, cellArr, arrSize) + nrCols;
		if (newIndex < 0 || newIndex > g_GridSize - 1)
		{
			newIndex = GetPlayerGridIndex(player, cellArr, arrSize);
		}
		if (cellArr[newIndex].isObstacle == false && !HasEnemy(newIndex, g_EnemyArr, g_EnemyArrSize))
		{
			player.dstRect = cellArr[newIndex].dstRect;
		}
	}
}
void ProcessFacing(Player& player, const SDL_MouseMotionEvent& e)
{
	Point2f mousePos{ float(e.x), g_WindowHeight - float(e.y) };
	Point2f playerCenter{};
	playerCenter.x = player.dstRect.left + player.dstRect.width / 2;
	playerCenter.y = player.dstRect.bottom + player.dstRect.height / 2;
	float angle{ CalculateAngleToMouse(playerCenter, mousePos) };
	float quadrantDiv{ g_Pi / 4 };

	if (angle > -quadrantDiv && angle < quadrantDiv)
	{
		player.facing = Direction::right;
	}
	else if (angle > quadrantDiv && angle < 3 * quadrantDiv)
	{
		player.facing = Direction::up;
	}
	else if (angle < -quadrantDiv && angle > -3 * quadrantDiv)
	{
		player.facing = Direction::down;
	}
	else player.facing = Direction::left;
}
void ProcessAnimState(Player& player, Sprite Sprites[])
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_S] || pStates[SDL_SCANCODE_Q] || pStates[SDL_SCANCODE_W] || pStates[SDL_SCANCODE_D])
	{
		switch (player.facing) {
		case Direction::right:
			player.sprite.texture = Sprites[int(AnimStates::runRight)].texture;
			player.animState = AnimStates::runRight;
			break;
		case Direction::left:
			player.sprite.texture = Sprites[int(AnimStates::runLeft)].texture;
			player.animState = AnimStates::runLeft;
			break;
		case Direction::up:
			player.sprite.texture = Sprites[int(AnimStates::runUp)].texture;
			player.animState = AnimStates::runUp;
			break;
		case Direction::down:
			player.sprite.texture = Sprites[int(AnimStates::runDown)].texture;
			player.animState = AnimStates::runDown;
			break;
		}
	}
	else
	{
		switch (player.facing) {
		case Direction::right:
			player.sprite.texture = Sprites[int(AnimStates::idleRight)].texture;
			player.animState = AnimStates::idleRight;
			break;
		case Direction::left:
			player.sprite.texture = Sprites[int(AnimStates::idleLeft)].texture;
			player.animState = AnimStates::idleLeft;
			break;
		case Direction::up:
			player.sprite.texture = Sprites[int(AnimStates::idleUp)].texture;
			player.animState = AnimStates::idleUp;
			break;
		case Direction::down:
			player.sprite.texture = Sprites[int(AnimStates::idleDown)].texture;
			player.animState = AnimStates::idleDown;
			break;
		}
	}
}

#pragma endregion playerInputHandling

#pragma region roomHandling
// Room save file handling
void SaveRoomLayout(Cell cellArr[],const int cellArrSize, const std::string& saveFileName)
{
	std::fstream file;
	file.open("RoomData/" + saveFileName + ".room", std::ios::out);
	if (!file)
	{
		std::cout << "CAN NOT SAVE LEVEL FILE\n";
	}

	for (int index{}; index < cellArrSize; ++index)
	{
		file << "{"
			<< FetchTextureName(cellArr[index].texture)
			<< "}";
	}

	file.close();
}
void LoadRoomLayout(Cell targetCellArr[], const std::string& fileName)
{
	std::fstream file;
	file.open("RoomData/" + fileName, std::ios::in);
	if (!file)
	{
		std::cout << "CAN NOT LOAD LEVEL FILE\n";
	}

	std::string textureNameBuffer{};
	bool isAddingToBuffer{ false };
	char currentChar;
	int index{};

	while (file)
	{
		currentChar = file.get();
		if (currentChar == '}')
		{
			targetCellArr[index].texture = FetchTexture(textureNameBuffer);
			isAddingToBuffer = false;
			textureNameBuffer = "";
			++index;
		}
		else if (isAddingToBuffer == true)
		{
			textureNameBuffer = textureNameBuffer + currentChar;
		}
		else if (currentChar == '{')
		{
			isAddingToBuffer = true;
		}
	}

	InitGrid(targetCellArr, g_NrRows, g_NrCols);

	file.close();
}

// Room Handling
void InitializeRooms(Room level[])
{
	// Below existing members we can add for example which enemies need to be loaded, 
	// which destructables/interactables are in room, what items/chests there are etc.
	Room& startingRoom = level[0];
	startingRoom.name = "starting_room";
	startingRoom.layoutToLoad = "starting_room.room";
	startingRoom.topDoorDestination = "vertical_hallway_1";

	Room& verticalHallway1 = level[1];
	verticalHallway1.name = "vertical_hallway_1";
	verticalHallway1.layoutToLoad = "vertical_hallway_1.room";
	verticalHallway1.bottomDoorDestination = "starting_room";
	verticalHallway1.topDoorDestination = "combat_room_1";

	Room& combatRoom1 = level[2];
	combatRoom1.name = "combat_room_1";
	combatRoom1.layoutToLoad = "combat_room_1.room";
	combatRoom1.bottomDoorDestination = "vertical_hallway_1";
	combatRoom1.topDoorDestination = "vertical_hallway_2";
	combatRoom1.leftDoorDestination = "horizontal_hallway_3";
	combatRoom1.rightDoorDestination = "horizontal_hallway_1";

	Room& verticalHallway2 = level[3];
	verticalHallway2.name = "vertical_hallway_2";
	verticalHallway2.layoutToLoad = "vertical_hallway_2.room";
	verticalHallway2.bottomDoorDestination = "combat_room_1";
	verticalHallway2.topDoorDestination = "pickup_room_2";

	Room& pickupRoom2 = level[4];
	pickupRoom2.name = "pickup_room_2";
	pickupRoom2.layoutToLoad = "pickup_room_2.room";
	pickupRoom2.bottomDoorDestination = "vertical_hallway_2";

	Room& horizontalHallway1 = level[5];
	horizontalHallway1.name = "horizontal_hallway_1";
	horizontalHallway1.layoutToLoad = "horizontal_hallway_1.room";
	horizontalHallway1.leftDoorDestination = "combat_room_1";
	horizontalHallway1.rightDoorDestination = "combat_room_2";

	Room& combatRoom2 = level[6];
	combatRoom2.name = "combat_room_2";
	combatRoom2.layoutToLoad = "combat_room_2.room";
	combatRoom2.leftDoorDestination = "horizontal_hallway_1";
	combatRoom2.rightDoorDestination = "horizontal_hallway_2";

	Room& horizontalHallway2 = level[7];
	horizontalHallway2.name = "horizontal_hallway_2";
	horizontalHallway2.layoutToLoad = "horizontal_hallway_2.room";
	horizontalHallway2.leftDoorDestination = "combat_room_2";
	horizontalHallway2.rightDoorDestination = "pickup_room_1";

	Room& pickupRoom1 = level[8];
	pickupRoom1.name = "pickup_room_1";
	pickupRoom1.layoutToLoad = "pickup_room_1.room";
	pickupRoom1.leftDoorDestination = "horizontal_hallway_2";

	Room& horizontalHallway3 = level[9];
	horizontalHallway3.name = "horizontal_hallway_3";
	horizontalHallway3.layoutToLoad = "horizontal_hallway_3.room";
	horizontalHallway3.leftDoorDestination = "combat_room_3";
	horizontalHallway3.rightDoorDestination = "combat_room_1";

	Room& combatRoom3 = level[10];
	combatRoom3.name = "combat_room_3";
	combatRoom3.layoutToLoad = "combat_room_3.room";
	combatRoom3.rightDoorDestination = "horizontal_hallway_3";
	combatRoom3.bottomDoorDestination = "vertical_hallway_3";
	combatRoom3.leftDoorDestination = "horizontal_hallway_4";

	Room& verticalHallway3 = level[11];
	verticalHallway3.name = "vertical_hallway_3";
	verticalHallway3.layoutToLoad = "vertical_hallway_3.room";
	verticalHallway3.topDoorDestination = "combat_room_3";
	verticalHallway3.bottomDoorDestination = "pickup_room_3";

	Room& pickupRoom3 = level[12];
	pickupRoom3.name = "pickup_room_3";
	pickupRoom3.layoutToLoad = "pickup_room_3.room";
	pickupRoom3.topDoorDestination = "vertical_hallway_3";

	Room& horizontalHallway4 = level[13];
	horizontalHallway4.name = "horizontal_hallway_4";
	horizontalHallway4.layoutToLoad = "horizontal_hallway_4.room";
	horizontalHallway4.rightDoorDestination = "combat_room_3";
	horizontalHallway4.leftDoorDestination = "boss_room";

	Room& bossRoom = level[14];
	bossRoom.name = "boss_room";
	bossRoom.layoutToLoad = "boss_room.room";
	bossRoom.rightDoorDestination = "horizontal_hallway_4";

	g_CurrentRoom = level[0];
	LoadRoom(level[0]);
}

void GoToLinkedRoom(const Room& roomOfDeparture, int playerIndex) 
{
	const int topDoor{ 6 };
	const int leftDoor{ 52 };
	const int rightDoor{ 64 };
	const int bottomDoor{ 110 };
	if (playerIndex == topDoor)
	{
		const int exit{ 97 };
		TeleportPlayer(exit, g_Player);
		g_CurrentRoom = FetchRoom(roomOfDeparture.topDoorDestination);
	}
	else if (playerIndex == leftDoor)
	{
		const int exit{ 63 };
		TeleportPlayer(exit, g_Player);
		g_CurrentRoom = FetchRoom(roomOfDeparture.leftDoorDestination);
	}
	else if (playerIndex == rightDoor)
	{
		const int exit{53};
		TeleportPlayer(exit, g_Player);
		g_CurrentRoom = FetchRoom(roomOfDeparture.rightDoorDestination);
	}
	else if (playerIndex == bottomDoor)
	{
		const int exit{19};
		TeleportPlayer(exit, g_Player);
		g_CurrentRoom = FetchRoom(roomOfDeparture.bottomDoorDestination);
	}
	LoadRoom(g_CurrentRoom);
}
Room FetchRoom(const std::string roomName)
{
	const Room nullRoom{};
	for (int i{0}; i < g_NrRoomsPerLevel; ++i)
	{
		if (g_Level[i].name == roomName)
		{
			return g_Level[i];
		}
	}
	std::cout << "Room with name '" << roomName << "' not found\n";
	return nullRoom;
}
void LoadRoom(const Room& roomToLoad)
{
	// This function will allow us to always retrieve the enemy array etc from any room we're loading by adding said code below
	std::cout << roomToLoad.layoutToLoad << '\n';
	LoadRoomLayout(g_CellArr, roomToLoad.layoutToLoad);
	// e.g. LoadEnemiesInRoom(roomToLoad.enemies);
	SetObstacles(g_CellArr, g_NrRows, g_NrCols);
}
void OpenDoors(Cell cellArr[], int size)
{
	for (int i{}; i < size; ++i)
	{
		if (cellArr[i].texture.id == FetchTexture("door_closed").id)
		{
			cellArr[i].texture = FetchTexture("door_open");
			cellArr[i].isObstacle = false;
		}
		else if (cellArr[i].texture.id == FetchTexture("door_transparent_closed").id)
		{
			cellArr[i].texture = FetchTexture("door_transparent_open");
			cellArr[i].isObstacle = false;
		}
	}
}

#pragma endregion roomHandling

#pragma region levelHandling
// Level Handling


#pragma endregion levelHandling

#pragma region spriteHandling
void InitPlayerSprites(Sprite Sprites[])
{
	// initialize idle right anim
	Sprites[int(AnimStates::idleRight)].texture = FetchTexture("knight_idle_anim_right");
	Sprites[int(AnimStates::idleRight)].cols = 4;
	Sprites[int(AnimStates::idleRight)].frames = 4;
	Sprites[int(AnimStates::idleRight)].currentFrame = 0;
	Sprites[int(AnimStates::idleRight)].accumulatedTime = 0.0f;
	Sprites[int(AnimStates::idleRight)].frameTime = 1 / 8.0f;	

	// initialize idle left anim
	Sprites[int(AnimStates::idleLeft)].texture = FetchTexture("knight_idle_anim_left");
	Sprites[int(AnimStates::idleLeft)].cols = 4;
	Sprites[int(AnimStates::idleLeft)].frames = 4;
	Sprites[int(AnimStates::idleLeft)].currentFrame = 0;
	Sprites[int(AnimStates::idleLeft)].accumulatedTime = 0.0f;
	Sprites[int(AnimStates::idleLeft)].frameTime = 1 / 8.0f;

	// initialize idle up anim
	Sprites[int(AnimStates::idleUp)].texture = FetchTexture("knight_idle_anim_up");
	Sprites[int(AnimStates::idleUp)].cols = 4;
	Sprites[int(AnimStates::idleUp)].frames = 4;
	Sprites[int(AnimStates::idleUp)].currentFrame = 0;
	Sprites[int(AnimStates::idleUp)].accumulatedTime = 0.0f;
	Sprites[int(AnimStates::idleUp)].frameTime = 1 / 8.0f;

	// initialize idle down anim
	Sprites[int(AnimStates::idleDown)].texture = FetchTexture("knight_idle_anim_down");
	Sprites[int(AnimStates::idleDown)].cols = 4;
	Sprites[int(AnimStates::idleDown)].frames = 4;
	Sprites[int(AnimStates::idleDown)].currentFrame = 0;
	Sprites[int(AnimStates::idleDown)].accumulatedTime = 0.0f;
	Sprites[int(AnimStates::idleDown)].frameTime = 1 / 8.0f;

	// initialize run right anim
	Sprites[int(AnimStates::runRight)].texture = FetchTexture("knight_run_right_anim");
	Sprites[int(AnimStates::runRight)].cols = 4;
	Sprites[int(AnimStates::runRight)].frames = 4;
	Sprites[int(AnimStates::runRight)].currentFrame = 0;
	Sprites[int(AnimStates::runRight)].accumulatedTime = 0.0f;
	Sprites[int(AnimStates::runRight)].frameTime = 1 / 15.0f;

	// initialize run down anim
	Sprites[int(AnimStates::runDown)].texture = FetchTexture("knight_run_down_anim");
	Sprites[int(AnimStates::runDown)].cols = 4;
	Sprites[int(AnimStates::runDown)].frames = 4;
	Sprites[int(AnimStates::runDown)].currentFrame = 0;
	Sprites[int(AnimStates::runDown)].accumulatedTime = 0.0f;
	Sprites[int(AnimStates::runDown)].frameTime = 1 / 15.0f;

	// initialize run up anim
	Sprites[int(AnimStates::runUp)].texture = FetchTexture("knight_run_up_anim");
	Sprites[int(AnimStates::runUp)].cols = 4;
	Sprites[int(AnimStates::runUp)].frames = 4;
	Sprites[int(AnimStates::runUp)].currentFrame = 0;
	Sprites[int(AnimStates::runUp)].accumulatedTime = 0.0f;
	Sprites[int(AnimStates::runUp)].frameTime = 1 / 15.0f;

	// initialize run left anim
	Sprites[int(AnimStates::runLeft)].texture = FetchTexture("knight_run_left_anim");
	Sprites[int(AnimStates::runLeft)].cols = 4;
	Sprites[int(AnimStates::runLeft)].frames = 4;
	Sprites[int(AnimStates::runLeft)].currentFrame = 0;
	Sprites[int(AnimStates::runLeft)].accumulatedTime = 0.0f;
	Sprites[int(AnimStates::runLeft)].frameTime = 1 / 15.0f;

	// initialize hit anim
	Sprites[int(AnimStates::hit)].texture = FetchTexture("knight_hit_anim");
	Sprites[int(AnimStates::hit)].cols = 1;
	Sprites[int(AnimStates::hit)].frames = 1;
	Sprites[int(AnimStates::hit)].currentFrame = 0;
	Sprites[int(AnimStates::hit)].accumulatedTime = 0.0f;
	Sprites[int(AnimStates::hit)].frameTime = 1 / 1.0f;


}
void UpdatePlayerSprites(Sprite Sprites[], float elapsedSec)
{
	switch (g_Player.animState)
	{
	case AnimStates::idleRight:
		Sprites[int(AnimStates::idleRight)].accumulatedTime += elapsedSec;
		if (Sprites[int(AnimStates::idleRight)].accumulatedTime > Sprites[int(AnimStates::idleRight)].frameTime)
		{
			++Sprites[int(AnimStates::idleRight)].currentFrame %= Sprites[int(AnimStates::idleRight)].frames;
			Sprites[int(AnimStates::idleRight)].accumulatedTime -= Sprites[int(AnimStates::idleRight)].frameTime;
		}
		break;
	case AnimStates::idleUp:
		Sprites[int(AnimStates::idleUp)].accumulatedTime += elapsedSec;
		if (Sprites[int(AnimStates::idleUp)].accumulatedTime > Sprites[int(AnimStates::idleUp)].frameTime)
		{
			++Sprites[int(AnimStates::idleUp)].currentFrame %= Sprites[int(AnimStates::idleUp)].frames;
			Sprites[int(AnimStates::idleUp)].accumulatedTime -= Sprites[int(AnimStates::idleUp)].frameTime;
		}
		break;
	case AnimStates::idleLeft:
		Sprites[int(AnimStates::idleLeft)].accumulatedTime += elapsedSec;
		if (Sprites[int(AnimStates::idleLeft)].accumulatedTime > Sprites[int(AnimStates::idleLeft)].frameTime)
		{
			++Sprites[int(AnimStates::idleLeft)].currentFrame %= Sprites[int(AnimStates::idleLeft)].frames;
			Sprites[int(AnimStates::idleLeft)].accumulatedTime -= Sprites[int(AnimStates::idleLeft)].frameTime;
		}
		break;
	case AnimStates::idleDown:
		Sprites[int(AnimStates::idleDown)].accumulatedTime += elapsedSec;
		if (Sprites[int(AnimStates::idleDown)].accumulatedTime > Sprites[int(AnimStates::idleDown)].frameTime)
		{
			++Sprites[int(AnimStates::idleDown)].currentFrame %= Sprites[int(AnimStates::idleDown)].frames;
			Sprites[int(AnimStates::idleDown)].accumulatedTime -= Sprites[int(AnimStates::idleDown)].frameTime;
		}
		break;
	case AnimStates::runRight:
		Sprites[int(AnimStates::runRight)].accumulatedTime += elapsedSec;
		if (Sprites[int(AnimStates::runRight)].accumulatedTime > Sprites[int(AnimStates::runRight)].frameTime)
		{
			++Sprites[int(AnimStates::runRight)].currentFrame %= Sprites[int(AnimStates::runRight)].frames;
			Sprites[int(AnimStates::runRight)].accumulatedTime -= Sprites[int(AnimStates::runRight)].frameTime;
		}
		break;
	case AnimStates::runLeft:
		Sprites[int(AnimStates::runLeft)].accumulatedTime += elapsedSec;
		if (Sprites[int(AnimStates::runLeft)].accumulatedTime > Sprites[int(AnimStates::runLeft)].frameTime)
		{
			++Sprites[int(AnimStates::runLeft)].currentFrame %= Sprites[int(AnimStates::runLeft)].frames;
			Sprites[int(AnimStates::runLeft)].accumulatedTime -= Sprites[int(AnimStates::runLeft)].frameTime;
		}
		break;
	case AnimStates::runDown:
		Sprites[int(AnimStates::runDown)].accumulatedTime += elapsedSec;
		if (Sprites[int(AnimStates::runDown)].accumulatedTime > Sprites[int(AnimStates::runDown)].frameTime)
		{
			++Sprites[int(AnimStates::runDown)].currentFrame %= Sprites[int(AnimStates::runDown)].frames;
			Sprites[int(AnimStates::runDown)].accumulatedTime -= Sprites[int(AnimStates::runDown)].frameTime;
		}
		break;
	case AnimStates::runUp:
		Sprites[int(AnimStates::runUp)].accumulatedTime += elapsedSec;
		if (Sprites[int(AnimStates::runUp)].accumulatedTime > Sprites[int(AnimStates::runUp)].frameTime)
		{
			++Sprites[int(AnimStates::runUp)].currentFrame %= Sprites[int(AnimStates::runUp)].frames;
			Sprites[int(AnimStates::runUp)].accumulatedTime -= Sprites[int(AnimStates::runUp)].frameTime;
		}
		break;
	}
}
#pragma endregion spriteHandling

#pragma region gameHandling
void DrawStartScreen()
{
	Texture startingScreen{ FetchTexture("starting_screen") };
	Rectf dstRect{};
	dstRect.width = g_WindowWidth;
	dstRect.height = g_WindowHeight;
	dstRect.left = 0;
	dstRect.bottom = 0;

	DrawTexture(startingScreen, dstRect);
}
void DrawEndScreen()
{
	Texture endScreen{ FetchTexture("end_screen") };
	Rectf dstRect{};
	dstRect.width = g_WindowWidth;
	dstRect.height = g_WindowHeight;
	dstRect.left = 0;
	dstRect.bottom = 0;

	DrawTexture(endScreen, dstRect);
}
void ClickStart(const SDL_MouseButtonEvent& e)
{
	if (g_Game == GameStates::startScreen)
	{
		Point2f mousePos{ float(e.x), g_WindowHeight - float(e.y) };
		Rectf startButton{ 516.f,63.f, 246.f,81.f };

		if (IsPointInRect(startButton, mousePos))
		{
			g_Game = GameStates::playing;
		}
	}
}
void SetGameOverScreen(Player& player)
{
	if (player.health <= 0.f)
	{
		g_Game = GameStates::gameOverScreen;
	}
}
#pragma endregion gameHandling

#pragma endregion ownDefinitions
