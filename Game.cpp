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
	LoadRoomLayout(g_CellArr, "starting_room.room");
	SetObstacles(g_CellArr, g_NrRows, g_NrCols);
	InitWeapons();
	InitPlayer(g_Player, g_CellArr, g_PlayerSprites);
	InitInteractables();
	InitEnemies(g_EnemyArr, g_EnemyArrSize, g_CellArr, g_GridSize);
}

void Draw()
{
	ClearBackground(0,0,0);

	// Put your own draw statements here
	DrawGridTextures(g_CellArr, g_NrRows, g_NrCols);
	DrawEnemies(g_EnemyArr, g_EnemyArrSize);
	DrawEnemyHealthBars(g_EnemyArr);
	DrawPlayer(g_Player, g_PlayerSprites);
	DrawWeaponInventory(g_Player);
	DrawPlayerHealth(g_Player);

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
	case SDLK_e:
		EnterRoom(g_Player, g_CellArr, g_GridSize);
		break;
	case SDLK_k:
		PlayerDebugInfo();
		break;
	case SDLK_o: 
		{
			// opens all doors. Remove when room clearing is implemented
			Point2f top{ 6 , 97 };
			Point2f left{ 52 , 63 };
			Point2f right{ 64 , 53 };
			Point2f bottom{ 110 , 19 };

			g_CellArr[int(top.x)].isObstacle = false;
			g_CellArr[int(left.x)].isObstacle = false;
			g_CellArr[int(right.x)].isObstacle = false;
			g_CellArr[int(bottom.x)].isObstacle = false;

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
Room FetchRoom(const std::string& roomName)
{
	Room tempRoom{};

	LoadRoomLayout(tempRoom.cells, roomName);

	return tempRoom;

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
void TeleportPlayer(const int index, Player& player)
{
	player.dstRect = g_CellArr[index].dstRect;
	player.animationPos = g_CellArr[index].dstRect;
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
/* void SwitchPlayer(Player& player)
{
	Texture playerRight = FetchTexture("player_right");
	Texture playerLeft = FetchTexture("player_left");
	Texture playerUp = FetchTexture("player_up");
	Texture playerDown = FetchTexture("player_down");

	if (player.facing == Direction::right) player.sprite.texture = playerRight;
	if (player.facing == Direction::up) player.sprite.texture = playerUp;
	if (player.facing == Direction::left) player.sprite.texture = playerLeft;
	if (player.facing == Direction::down) player.sprite.texture = playerDown;
}*/

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
void EnterRoom(Player& player, Cell cellArr[], const int cellArrSize)
{
	int playerIndex{ GetPlayerGridIndex(player, cellArr, cellArrSize)};

	Point2f top		{ 6 , 97 }; 
	Point2f left	{ 52 , 63 };
	Point2f right	{ 64 , 53 };
	Point2f bottom	{ 110 , 19 };
	// x contains the index of a door, e.g. the top door
	// y contains the index of the cell you land on after entering that door, e.g. in front of the bottom door

	switch (g_CurrentRoom)
		{
		case RoomStates::starting_room:
			{
				if (playerIndex == top.x)
				{
					LoadRoomLayout(cellArr, "vertical_hallway_1.room");
					TeleportPlayer(int(top.y), player);
					g_CurrentRoom = RoomStates::vertical_hallway_1;
					TeleportPlayer(int(top.y), g_Player);
				}
				break;
			}

		case RoomStates::vertical_hallway_1:
			{
				if (playerIndex == top.x)
				{
					LoadRoomLayout(cellArr, "combat_room_1.room");
					TeleportPlayer(int(top.y), player);
					g_CurrentRoom = RoomStates::combat_room_1;
					TeleportPlayer(int(top.y), g_Player);
				}
				else if (playerIndex == bottom.x)
				{
					LoadRoomLayout(cellArr, "starting_room.room");
					TeleportPlayer(int(bottom.y), player);
					g_CurrentRoom = RoomStates::starting_room;
					TeleportPlayer(int(bottom.y), g_Player);
				}
				break;
			}

		case RoomStates::vertical_hallway_2:
			{
			if (playerIndex == top.x)
			{
				LoadRoomLayout(cellArr, "pickup_room_2.room");
				TeleportPlayer(int(top.y), player);
				g_CurrentRoom = RoomStates::pickup_room_2;
			}
			else if (playerIndex == bottom.x)
			{
				LoadRoomLayout(cellArr, "combat_room_1.room");
				TeleportPlayer(int(bottom.y), player);
				g_CurrentRoom = RoomStates::combat_room_1;
			}
			break;
		}

		case RoomStates::vertical_hallway_3:
			{
			if (playerIndex == top.x)
			{
				LoadRoomLayout(cellArr, "combat_room_3.room");
				TeleportPlayer(int(top.y), player);
				g_CurrentRoom = RoomStates::combat_room_3;
			}
			else if (playerIndex == bottom.x)
			{
				LoadRoomLayout(cellArr, "pickup_room_3.room");
				TeleportPlayer(int(bottom.y), player);
				g_CurrentRoom = RoomStates::pickup_room_3;
			}
			break;
		}

		case RoomStates::horizontal_hallway_1:
			{
				if (playerIndex == right.x)
				{
					LoadRoomLayout(cellArr, "combat_room_2.room");
					TeleportPlayer(int(right.y), player);
					g_CurrentRoom = RoomStates::combat_room_2;
				}
				if (playerIndex == left.x)
				{
					LoadRoomLayout(cellArr, "combat_room_1.room");
					TeleportPlayer(int(left.y), player);
					g_CurrentRoom = RoomStates::combat_room_1;
				}
				break;
			}

		case RoomStates::horizontal_hallway_2:
			{
				if (playerIndex == right.x)
				{
					LoadRoomLayout(cellArr, "pickup_room_1.room");
					TeleportPlayer(int(right.y), player);
					g_CurrentRoom = RoomStates::pickup_room_1;
				}
				if (playerIndex == left.x)
				{
					LoadRoomLayout(cellArr, "combat_room_2.room");
					TeleportPlayer(int(left.y), player);
					g_CurrentRoom = RoomStates::combat_room_2;
				}
				break;
			}

		case RoomStates::horizontal_hallway_3:
			{
				if (playerIndex == right.x)
				{
					LoadRoomLayout(cellArr, "combat_room_1.room");
					TeleportPlayer(int(right.y), player);
					g_CurrentRoom = RoomStates::combat_room_1;
				}
				if (playerIndex == left.x)
				{
					LoadRoomLayout(cellArr, "combat_room_3.room");
					TeleportPlayer(int(left.y), player);
					g_CurrentRoom = RoomStates::combat_room_3;
				}
				break;
			}

		case RoomStates::horizontal_hallway_4:
			{
			if (playerIndex == right.x)
			{
				LoadRoomLayout(cellArr, "combat_room_3.room");
				TeleportPlayer(int(right.y), player);
				g_CurrentRoom = RoomStates::combat_room_3;
			}
			if (playerIndex == left.x)
			{
				LoadRoomLayout(cellArr, "boss_room.room");
				TeleportPlayer(int(left.y), player);
				g_CurrentRoom = RoomStates::boss_room;
			}
			break;
		}

		case RoomStates::combat_room_1:
			{
				if (playerIndex == bottom.x)
				{
					LoadRoomLayout(cellArr, "vertical_hallway_1.room");
					TeleportPlayer(int(bottom.y), player);
					g_CurrentRoom = RoomStates::vertical_hallway_1;
				}
				else if (playerIndex == left.x)
				{
					LoadRoomLayout(cellArr, "horizontal_hallway_3.room");
					TeleportPlayer(int(left.y), player);
					g_CurrentRoom = RoomStates::horizontal_hallway_3;
				}
				else if (playerIndex == top.x)
				{
					LoadRoomLayout(cellArr, "vertical_hallway_2.room");
					TeleportPlayer(int(top.y), player);
					g_CurrentRoom = RoomStates::vertical_hallway_2;
				}
				else if (playerIndex == right.x)
				{
					LoadRoomLayout(cellArr, "horizontal_hallway_1.room");
					TeleportPlayer(int(right.y), player);
					g_CurrentRoom = RoomStates::horizontal_hallway_1;
				}
				break;
			}

		case RoomStates::combat_room_2:
			{
				if (playerIndex == left.x)
				{
					LoadRoomLayout(cellArr, "horizontal_hallway_1.room");
					TeleportPlayer(int(left.y), player);
					g_CurrentRoom = RoomStates::horizontal_hallway_1;
				}
				else if (playerIndex == right.x)
				{
					LoadRoomLayout(cellArr, "horizontal_hallway_2.room");
					TeleportPlayer(int(right.y), player);
					g_CurrentRoom = RoomStates::horizontal_hallway_2;
				}
				break;
			}

		case RoomStates::combat_room_3:
			{
				if (playerIndex == left.x)
				{
					LoadRoomLayout(cellArr, "horizontal_hallway_4.room");
					TeleportPlayer(int(left.y), player);
					g_CurrentRoom = RoomStates::horizontal_hallway_4;
				}
				else if (playerIndex == right.x)
				{
					LoadRoomLayout(cellArr, "horizontal_hallway_3.room");
					TeleportPlayer(int(right.y), player);
					g_CurrentRoom = RoomStates::horizontal_hallway_3;
				}
				else if (playerIndex == bottom.x)
				{
					LoadRoomLayout(cellArr, "vertical_hallway_3.room");
					TeleportPlayer(int(bottom.y), player);
					g_CurrentRoom = RoomStates::vertical_hallway_3;
				}
				break;
			}

		case RoomStates::pickup_room_1:
			{
				if (playerIndex == left.x)
				{
					LoadRoomLayout(cellArr, "horizontal_hallway_2.room");
					TeleportPlayer(int(left.y), player);
					g_CurrentRoom = RoomStates::horizontal_hallway_2;
				}
				break;
			}

		case RoomStates::pickup_room_2:
			{
				if (playerIndex == bottom.x)
				{
					LoadRoomLayout(cellArr, "vertical_hallway_2.room");
					TeleportPlayer(int(bottom.y), player);
					g_CurrentRoom = RoomStates::vertical_hallway_2;
				}
				break;
			}

		case RoomStates::pickup_room_3:
			{
			if (playerIndex == top.x)
			{
				LoadRoomLayout(cellArr, "vertical_hallway_3.room");
				TeleportPlayer(int(top.y), player);
				g_CurrentRoom = RoomStates::vertical_hallway_3;
			}
			break;
		}

		case RoomStates::boss_room:
			{
				if (playerIndex == right.x)
				{
					LoadRoomLayout(cellArr, "horizontal_hallway_4.room");
					TeleportPlayer(int(right.y), player);
					g_CurrentRoom = RoomStates::horizontal_hallway_4;
				}
				break;
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

#pragma endregion ownDefinitions
