#include "pch.h"
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <fstream>
#include "Game.h"

#pragma region gameFunctions											
void Start()
{
	InitGame();
}

void Draw()
{
	DrawGame();
}

void Update(float elapsedSec)
{
	UpdateGame(elapsedSec);
}

void End()
{
	DeleteTextures();
	DeleteGlobalArrays();
}
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{
	int itemslot{ 0 };
	switch (key)
	{
	case SDLK_TAB:
		CycleWeapons(g_Player);
		break;
	case SDLK_SPACE:
		Interact(g_Player, g_CellArr, g_GridSize, g_CurrentRoom);
		break;
	case SDLK_k:
		PlayerDebugInfo();
		break;
	case SDLK_y:
		g_Player.weaponAnimation.enabled = !g_Player.weaponAnimation.enabled;
		break;
	case SDLK_i:
		PrintGameInfo();
		break;
	case SDLK_1:
		itemslot = 0;
		if (g_Player.selectedItem != itemslot)
		{
			g_Player.selectedItem = itemslot;
		}
		else 
		{
			UseItem(g_Player, itemslot);
		}
		break;
	case SDLK_2:
		itemslot = 1;
		if (g_Player.selectedItem != itemslot)
		{
			g_Player.selectedItem = itemslot;
		}
		else
		{
			UseItem(g_Player, itemslot);
		}
		break;
	case SDLK_3:
		itemslot = 2;
		if (g_Player.selectedItem != itemslot)
		{
			g_Player.selectedItem = itemslot;
		}
		else
		{
			UseItem(g_Player, itemslot);
		}
		break;
	case SDLK_4:
		itemslot = 3;
		if (g_Player.selectedItem != itemslot)
		{
			g_Player.selectedItem = itemslot;
		}
		else
		{
			UseItem(g_Player, itemslot);
		}
		break;
	case SDLK_5:
		itemslot = 4;
		if (g_Player.selectedItem != itemslot)
		{
			g_Player.selectedItem = itemslot;
		}
		else
		{
			UseItem(g_Player, itemslot);
		}
		break;
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
#pragma region gameHandling
void InitGame()
{
	InitTextures(g_NamedTexturesArr, g_TexturesSize, g_Numbers, g_GridSize);
	InitTotems(g_TotemSprite);
	InitItems();
	InitWeapons();
	InitializeRooms(g_Level);
	InitPlayer(g_Player, g_CellArr, g_PlayerSprites);
	InitVoiceLines(g_VoiceLinesArr);
}
void DrawGame()
{
	ClearBackground(0, 0, 0);

	switch (g_Game)
	{
	case GameStates::startScreen:
	{
		DrawStartScreen();
		break;
	}
	case GameStates::playing:
	{
		DrawTotems(g_TotemSprite, g_CurrentRoom);
		DrawGridTextures(g_CellArr, g_NrRows, g_NrCols);
		DrawReach(g_Player);
		DrawStatusEffects(g_Player);
		DrawInteractables();
		DrawProjectiles();
		DrawEnemies(g_EnemyArr, g_MaxEnemiesPerRoom);
		DrawEnemyHealthBars(g_EnemyArr);
		DrawBoss(g_CurrentRoom, g_Boss, g_VoiceLinesArr);
		DrawPlayer(g_Player, g_PlayerSprites);
		DrawWeaponInventory(g_Player);
		DrawItemInventory(g_Player);
		DrawPlayerHealth(g_Player);
		break;
	}
	case GameStates::gameOver:
	{
		DrawTotems(g_TotemSprite, g_CurrentRoom);
		DrawGridTextures(g_CellArr, g_NrRows, g_NrCols);
		DrawReach(g_Player);
		DrawStatusEffects(g_Player);
		DrawInteractables();
		DrawProjectiles();
		DrawEnemies(g_EnemyArr, g_MaxEnemiesPerRoom);
		DrawEnemyHealthBars(g_EnemyArr);
		DrawBoss(g_CurrentRoom, g_Boss, g_VoiceLinesArr);
		DrawPlayer(g_Player, g_PlayerSprites);
		DrawWeaponInventory(g_Player);
		DrawItemInventory(g_Player);
		DrawPlayerHealth(g_Player);
		DrawEndScreen();
		break;
	}
	case GameStates::gameWon:
	{
		DrawTotems(g_TotemSprite, g_CurrentRoom);
		DrawGridTextures(g_CellArr, g_NrRows, g_NrCols);
		DrawReach(g_Player);
		DrawStatusEffects(g_Player);
		DrawInteractables();
		DrawProjectiles();
		DrawEnemies(g_EnemyArr, g_MaxEnemiesPerRoom);
		DrawEnemyHealthBars(g_EnemyArr);
		DrawBoss(g_CurrentRoom, g_Boss, g_VoiceLinesArr);
		DrawPlayer(g_Player, g_PlayerSprites);
		DrawWeaponInventory(g_Player);
		DrawItemInventory(g_Player);
		DrawPlayerHealth(g_Player);
		DrawEndScreen();
		break;
	}
	case GameStates::restarting:
		break;
	}
}
void UpdateGame(float elapsedSec)
{
	switch (g_Game)
	{
	case GameStates::startScreen:

		break;
	case GameStates::playing:
		UpdateTotems(g_TotemSprite, g_CurrentRoom, elapsedSec);
		UpdateBossAnimState(g_CurrentRoom, g_Boss, elapsedSec);
		UpdateBossAIState(g_Boss, elapsedSec);
		UpdateVoiceLine(g_Boss, g_CurrentRoom,elapsedSec);
		UpdateAnimationPos(elapsedSec, g_Player);
		UpdateWeaponAnimation(elapsedSec);
		UpdateEnemies(elapsedSec, g_EnemyArr, g_MaxEnemiesPerRoom, g_CellArr, g_GridSize);
		ProcessMovement(g_Player, g_CellArr, g_GridSize, g_PlayerSprites, elapsedSec);
		ProcessAnimState(g_Player, g_PlayerSprites);
		ProcessWeaponCooldown(g_Player, elapsedSec);
		UpdatePlayerAnimState(g_PlayerSprites, elapsedSec);
		UpdateProjectiles(elapsedSec);
		UpdateStatusEffects(elapsedSec);
		if (CheckRoomCleared(g_CurrentRoom) && g_CurrentRoom.id != RoomID::bossRoom)
		{
			SetRoomCleared(g_CurrentRoom);
			OpenDoors(g_CellArr, g_GridSize);
		}
		SetEndScreen(g_Boss, g_Player);
		break;
	case GameStates::gameOver:
		break;
	case GameStates::gameWon:
		break;
	case GameStates::restarting:
		break;
	default:
		break;
	}
}
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
	Texture endScreen{};
	if (g_Game == GameStates::gameOver)
	{
		endScreen = FetchTexture("game_over_screen");
	}
	if (g_Game == GameStates::gameWon)
	{
		endScreen = FetchTexture("game_won_screen");
	}
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
void SetEndScreen(Boss boss, Player& player)
{
	if (player.health <= 0.f)
	{
		g_Game = GameStates::gameOver;
	}
	else if (g_CurrentRoom.id == RoomID::bossRoom && boss.health <= 0.f)
	{
		g_Game = GameStates::gameWon;
	}
}
void DeleteGlobalArrays()
{
	delete[] g_CellArr;
	delete[] g_Level;
	delete[] g_Items;
	delete[] g_EnemyArr;
	delete[] g_Weapons;
	delete[] g_PlayerSprites;
	delete[] g_Numbers;
	delete[] g_VoiceLinesArr;
	delete[] g_Projectiles;
	delete[] g_NamedTexturesArr;
	delete[] g_Interactables;

	g_CellArr = nullptr;
	g_Level = nullptr;
	g_Items = nullptr;
	g_EnemyArr = nullptr;
	g_Weapons = nullptr;
	g_PlayerSprites = nullptr;
	g_Numbers = nullptr;
	g_VoiceLinesArr = nullptr;
	g_Projectiles = nullptr;
	g_NamedTexturesArr = nullptr;
	g_Interactables = nullptr;
}
#pragma endregion gameHandling

#pragma region utilFunctions
// Utils
void PrintGameInfo() {
	std::cout << "Welcome to Cave Knight!\n\n";
	std::cout << "The goal of the game is to get to the end of the dungeon and kill the boss,\n";
	std::cout << "but to do that you will have to make your way through all the rooms first. \n\n";
	std::cout << "Whenever you enter a room, all doors will be locked until you have killed\n";
	std::cout << "all enemies within that room.\n\n";
	std::cout << "Controls are as follows:\n";
	std::cout << "- MOVING happens using the ZQSD/WASD buttons.\n";
	std::cout << "- ATTACKING can be done by CLICKING, and will always be in the DIRECTION of\n";
	std::cout << "  the MOUSE CURSOR. A light grey overlay next to your character will indicate\n";
	std::cout << "  the tiles the character will hit.\n";
	std::cout << "- INTERACTING with objects, such as going through doors or picking up items can\n";
	std::cout << "  be done using the SPACE BAR.\n";
	std::cout << "- USING ITEMS can be done by pressing NUMBER KEYS 1-5. If the item was already \n";
	std::cout << "  selected you will use the item in the selected slot, otherwise you have to \n";
	std::cout << "  press the button once to select the item, and a second time to actually use \n";
	std::cout << "  it. The selected item is indicated by a red outline.\n";
	std::cout << "- SWAPPING WEAPONS is done using the TAB KEY. The selected weapon is also indicated\n";
	std::cout << "  by a red outline.\n\n";
	std::cout << "The game has several potions that you can use, their effects are as follows:\n";
	std::cout << "- RED POTION = HEALTH POTION: restores 10 hp.\n";
	std::cout << "- YELLOW POTION = SPEED POTION: makes you run faster for 10 seconds.\n";
	std::cout << "- GREEN POTION = REGENERATION POTION: heals you for 20 hp over 10 seconds.\n";
	std::cout << "- LIGHT BLUE POTION = SHIELDING POTION: temporarily increases your health by 20hp.\n";
	std::cout << "- DARK BLUE POTION = STRENGTH POTION: doubles your attack damage for 10 seconds.\n";
	std::cout << "All potions with an effect that lasts for a duration of time cause an aura in the\n";
	std::cout << "same colour. When this disappears, the potion's effect has faded.\n\n";
	std::cout << "GOOD LUCK AND HAVE FUN EXPLORING OUR SMALL DUNGEON!\n";
}
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
bool HasInteractable(const int cellIndex, Interactable interactableArr[], int arrSize)
{
	for (int i{}; i < g_MaxInteractablesRoom; ++i)
	{
		if (g_Interactables[i].dstRect.bottom == g_CellArr[cellIndex].dstRect.bottom
			&& g_Interactables[i].dstRect.left == g_CellArr[cellIndex].dstRect.left)
		{
			return true;
		}
	}
	return false;
}
int GetTilePlayerFacing()
{
	int playerTile{ GetPlayerGridIndex(g_Player, g_CellArr, g_GridSize) };
	if (g_Player.facing == Direction::up)
	{
		return playerTile - g_NrCols;
	}
	else if (g_Player.facing == Direction::down)
	{
		return playerTile + g_NrCols;
	}
	else if (g_Player.facing == Direction::right)
	{
		return playerTile + 1;
	}
	else
	{
		return playerTile - 1;
	}
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
bool IsOverlapping(const Rectf& rectangle1, const Rectf& rectangle2)
{
	if (rectangle1.left >= rectangle2.left + rectangle2.width || rectangle2.left >= rectangle1.left + rectangle1.width)
	{
		return false;
	}

	if (rectangle1.bottom >= rectangle2.bottom + rectangle2.height || rectangle2.bottom >= rectangle1.bottom + rectangle1.height)
	{
		return false;
	}

	return true;
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

	// Delete item textures in player struct
	for (int i{ 0 }; i < g_ItemInventorySize; ++i)
	{
		DeleteTexture(g_Player.itemInventory[i].texture);
	}

	// Delete weapon textures in player struct
	for (int i{ 0 }; i < g_WeaponInventorySize; ++i)
	{
		DeleteTexture(g_Player.weaponInventory[i].texture);
	}

	// Delete weapon textures in weapon array
	for (int i{ 0 }; i < g_WeaponsInGame; ++i)
	{
		DeleteTexture(g_Weapons[i].texture);
	}

	// Delete item textures in weapon array
	for (int i{ 0 }; i < g_ItemsInGame; ++i)
	{
		DeleteTexture(g_Items[i].texture);
	}

	// Delete projectile textures in projectile array
	for (int i{ 0 }; i < g_MaxProjectiles; ++i)
	{
		DeleteTexture(g_Projectiles[i].texture);
	}

	// Delete named textures array
	for (int i{ 0 }; i < g_TexturesSize; ++i)
	{
		DeleteTexture(g_NamedTexturesArr[i].texture);
	}

	// Delete enemy textures in enemy array
	for (int i{ 0 }; i < g_MaxEnemiesPerRoom; ++i)
	{
		DeleteTexture(g_EnemyArr[i].texture);
	}

	// Delete textures stored in the grid array
	for (int i{ 0 }; i < g_GridSize; ++i)
	{
		DeleteTexture(g_CellArr[i].texture);
	}

	// Delete number textures array
	for (int i{ 0 }; i < g_GridSize; ++i)
	{
		DeleteTexture(g_Numbers[i]);
	}

	// Delete textures stored in the g_PlayerSprites array
	for (int i{ 0 }; i < g_PlayerSpritesSize; ++i)
	{
		DeleteTexture(g_PlayerSprites[i].texture);
	}

	// Delete Boss related textures
	DeleteTexture(g_Boss.sprite.texture);
	DeleteTexture(g_TotemSprite.texture);
	for (int i{ 0 }; i < g_VoiceLinesSize; ++i)
	{
		DeleteTexture(g_VoiceLinesArr[i]);
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

	InitPlayerAnimState(Sprites);
	player.sprite.texture = Sprites[idle].texture;
	player.animState = AnimStates::idleRight;
	player.dstRect = cellArr[97].dstRect; // 97 is starting pos in starting room
	player.animationPos = player.dstRect;
	player.health = player.maxHealth;
	player.selectedWeapon = 0;
	player.selectedItem = 0;
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
	if (player.facing == Direction::up || player.facing == Direction::left)
	{
		DrawWeaponAnimation();
	}
	DrawTexture(player.sprite.texture, player.animationPos, srcRect);
	if (player.facing == Direction::right || player.facing == Direction::down)
	{
		DrawWeaponAnimation();
	}
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
void InitPlayerAnimState(Sprite Sprites[])
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
	Sprites[int(AnimStates::hit)].frameTime = 1 / 15.0f;


}
void UpdatePlayerAnimState(Sprite Sprites[], float elapsedSec)
{
	if (g_Player.isPlayerHit == true)
	{
		g_Player.animState = AnimStates::hit;
	}
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
	case AnimStates::hit:
		Sprites[int(AnimStates::hit)].accumulatedTime += elapsedSec;
		if (Sprites[int(AnimStates::hit)].accumulatedTime < 0.2f)
		{
			g_Player.sprite.texture = FetchTexture("knight_hit_anim");
		}
		else
		{
			Sprites[int(AnimStates::hit)].accumulatedTime = 0;
			g_Player.isPlayerHit = false;
		}
		break;

	}
}

void DrawWeaponInventory(const Player& player)
{
	const float slotSize{ 64.f };
	Texture weaponInv{ FetchTexture("weapon_inventory") };
	Rectf origin{0, (g_WindowHeight - weaponInv.height)/2, weaponInv.width, weaponInv.height };
	DrawTexture(weaponInv, origin);
	for (int index{}; index < g_WeaponInventorySize; ++index)
	{
		const float border{2.0f};
		Rectf location{};
		location.left = origin.left + slotSize + border;
		location.bottom = origin.bottom + weaponInv.height - (index + 2) * (slotSize + border);
		location.width = slotSize;
		location.height = slotSize;
		SetColor(g_Grey);
		DrawRect(location);
		if (player.weaponInventory[index].texture.id != 0)
		{
			DrawTexture(player.weaponInventory[index].texture, location);
		}
		if (player.weaponInventory[index].name == "")
		{
			SetColor(g_Grey);
			DrawRect(location);
		}
		if (player.selectedWeapon == index)
		{
			SetColor(g_Red);
			DrawRect(location, 2.f);
		}
	}
}
void DrawItemInventory(const Player& player)
{
	const float slotSize{ 64.f };
	const float border{ 2.0f };
	Texture itemInv{ FetchTexture("item_inventory") };
	const float heightScale{ (slotSize + 2 * border) / itemInv.height };
	const float widthScale{ (slotSize * 5 + border * 4) / itemInv.width };
	Rectf origin{ 0, 0, itemInv.width * widthScale, itemInv.height * heightScale};
	DrawTexture(itemInv, origin);
	for (int index{}; index < g_ItemInventorySize; ++index)
	{
		Rectf location{};
		location.left = origin.left + (index) * (slotSize + border);
		location.bottom = origin.bottom + border; 
		location.width = slotSize;
		location.height = slotSize;
		SetColor(g_Grey);
		DrawRect(location);
		if (player.itemInventory[index].texture.id != 0)
		{
			DrawTexture(player.itemInventory[index].texture, location);
		}

		const float numberSize{ 16.f };
		Rectf amountLocation{ location.left + location.width - numberSize, location.bottom, numberSize, numberSize };
		DrawTexture(g_Numbers[player.itemInventory[index].count], amountLocation);
		Rectf keyLocation(location.left + (slotSize - numberSize)/2, location.bottom + location.height, numberSize, numberSize);
		DrawTexture(g_Numbers[index + 1], keyLocation);
		if (player.itemInventory[index].name == "")
		{
			SetColor(g_Grey);
			DrawRect(location);
		}
		if (player.selectedItem == index)
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

void AttackOnTiles(const Player& player, int tilesToScan[], int tilesAmount) 
{
	// Lower enemy health
	for (int currentTile{}; currentTile < tilesAmount; ++currentTile)
	{
		for (int index{}; index < g_MaxEnemiesPerRoom; ++index)
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

	// Lower Boss health
		if (IsBossOnTilesToScan(g_Boss, tilesToScan, currentTile))
		{
			g_Boss.health -= player.weaponInventory[player.selectedWeapon].damageOutput;
		}
		
	}
}
#pragma endregion playerHandling

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

	if (pStates[SDL_SCANCODE_A] && IsReadyToAct)
	{
		player.timeTracker = 0;
		newIndex = GetPlayerGridIndex(player, cellArr, arrSize) - 1;
		if (cellArr[newIndex].isObstacle == false && !HasEnemy(newIndex, g_EnemyArr, g_MaxEnemiesPerRoom)
			&& !HasInteractable(newIndex, g_Interactables, g_MaxInteractablesRoom))
		{
			player.dstRect = cellArr[newIndex].dstRect;
		}
	}
	if (pStates[SDL_SCANCODE_D] && IsReadyToAct)
	{
		player.timeTracker = 0;
		newIndex = GetPlayerGridIndex(player, cellArr, arrSize) + 1;
		if (cellArr[newIndex].isObstacle == false && !HasEnemy(newIndex, g_EnemyArr, g_MaxEnemiesPerRoom)
			&& !HasInteractable(newIndex, g_Interactables, g_MaxInteractablesRoom))
		{
			player.dstRect = cellArr[newIndex].dstRect;
		}
	}
	if (pStates[SDL_SCANCODE_W] && IsReadyToAct)
	{
		player.timeTracker = 0;
		newIndex = GetPlayerGridIndex(player, cellArr, arrSize) - nrCols;
		if (newIndex < 0 || newIndex > g_GridSize - 1)
		{
			newIndex = GetPlayerGridIndex(player, cellArr, arrSize);
		}
		if (cellArr[newIndex].isObstacle == false && !HasEnemy(newIndex, g_EnemyArr, g_MaxEnemiesPerRoom)
			&& !HasInteractable(newIndex, g_Interactables, g_MaxInteractablesRoom))
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
		if (cellArr[newIndex].isObstacle == false && !HasEnemy(newIndex, g_EnemyArr, g_MaxEnemiesPerRoom)
			&& !HasInteractable(newIndex, g_Interactables, g_MaxInteractablesRoom))
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
	if (pStates[SDL_SCANCODE_S] || pStates[SDL_SCANCODE_Q] || pStates[SDL_SCANCODE_W] || pStates[SDL_SCANCODE_D]
		|| pStates[SDL_SCANCODE_A] || pStates[SDL_SCANCODE_Z])
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
	else if (HasInteractable(GetTilePlayerFacing(), g_Interactables, g_InteractablesInGame))
	{
		for (int i{}; i < g_InteractablesInGame; ++i)
		{
			if (g_Interactables[i].location == GetTilePlayerFacing())
			{
				PickUpInteractable(i, g_Interactables[i].location);
			}
		}
	}
}

void UseItem(Player& player, int itemslot)
{
	Item& selectedItem{ player.itemInventory[itemslot] };
	if (selectedItem.count > 0)
	{
		--selectedItem.count;
		
		const int stacks{ player.effects[itemslot].stacks };
		player.effects[itemslot] = selectedItem.effect;
		player.effects[itemslot].stacks = stacks + 1;
	}
	
	if (selectedItem.count == 0)
	{
		Item nullItem{};
		selectedItem = nullItem;
	}
}

void UseWeapon(Player& player)
{
	if (!player.isWeaponOnCooldown && !player.weaponAnimation.isPlaying) {
		if (player.weaponInventory[player.selectedWeapon].type == WeaponType::sword)
		{
			g_Player.weaponAnimation.isPlaying = true;
			g_Player.weaponAnimation.elapsedTime = 0;
			UseSword(player);
		}
		else if (player.weaponInventory[player.selectedWeapon].type == WeaponType::bow)
		{
			g_Player.weaponAnimation.isPlaying = true;
			g_Player.weaponAnimation.elapsedTime = 0;
			UseBow(player);
		}
		else if (player.weaponInventory[player.selectedWeapon].type == WeaponType::spear)
		{
			g_Player.weaponAnimation.isPlaying = true;
			g_Player.weaponAnimation.elapsedTime = 0;
			UseSpear(player);
		}
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
void UseBow(const Player& player)
{
	const float bowDamage{ player.weaponInventory[player.selectedWeapon].damageOutput };
	const float projectileSpeed{ 500.f };
	Rectf spawnRect{ g_Player.animationPos };
	if (player.facing == Direction::right)
	{
		const float angle{ 0.0f };
		spawnRect.left += spawnRect.width /3;
		CreateProjectile(spawnRect, "arrow_right", angle, projectileSpeed, bowDamage);
	}
	else if (player.facing == Direction::left)
	{
		const float angle{ g_Pi };
		spawnRect.left -= spawnRect.width / 3;
		CreateProjectile(spawnRect, "arrow_left", angle, projectileSpeed, bowDamage);
	}
	else if (player.facing == Direction::up)
	{
		const float angle{ g_Pi / 2 };
		spawnRect.bottom += spawnRect.width / 3;
		CreateProjectile(spawnRect, "arrow_up", angle, projectileSpeed, bowDamage);
	}
	else if (player.facing == Direction::down)
	{
		const float angle{ 3 * g_Pi / 2 };
		spawnRect.bottom -= spawnRect.width /3;
		CreateProjectile(spawnRect, "arrow_down", angle, projectileSpeed, bowDamage);
	}
}
void UseSpear(const Player& player) {
	int playerIndex{ GetPlayerGridIndex(player, g_CellArr, g_GridSize) };
	const int tilesAmount{ 2 };
	if (player.facing == Direction::up) {
		int tilesToScan[tilesAmount]{ playerIndex - g_NrCols, playerIndex - 2 * g_NrCols };
		AttackOnTiles(player, tilesToScan, tilesAmount);
	}
	else if (player.facing == Direction::down)
	{
		int tilesToScan[tilesAmount]{ playerIndex + g_NrCols, playerIndex + 2 * g_NrCols };
		AttackOnTiles(player, tilesToScan, tilesAmount);
	}
	else if (player.facing == Direction::right)
	{
		int tilesToScan[tilesAmount]{ playerIndex + 1, playerIndex + 2};
		AttackOnTiles(player, tilesToScan, tilesAmount);
	}
	else if (player.facing == Direction::left)
	{
		int tilesToScan[tilesAmount]{ playerIndex - 1, playerIndex - 2 };
		AttackOnTiles(player, tilesToScan, tilesAmount);
	}
}

void CycleWeapons(Player& player)
{
	if (   player.selectedWeapon < g_WeaponInventorySize - 1 
		&& player.weaponInventory[player.selectedWeapon + 1].name != "")
	{
		++player.selectedWeapon;
	}
	else
	{
		player.selectedWeapon = 0;
	}
}
void ProcessWeaponCooldown(Player& player, float elapsedSec)
{
	if (player.coolDownTimer < 0)
	{
		player.isWeaponOnCooldown = false;
	}
	else
	{
		player.coolDownTimer -= elapsedSec;
	}
}

void DrawReach(const Player& player)
{
	WeaponType playerWeapon{ player.weaponInventory[player.selectedWeapon].type };
	SetColor(g_WhiteTransparent);
	if (playerWeapon == WeaponType::sword)
	{
		DrawSwordReach(player);
	}
	else if (playerWeapon == WeaponType::bow)
	{
		DrawBowReach(player);
	}
	else if (playerWeapon == WeaponType::spear)
	{
		DrawSpearReach(player);
	}
}
void DrawSwordReach(const Player& player)
{
	if (player.weaponInventory[player.selectedWeapon].type == WeaponType::sword) {
		int playerIndex{ GetPlayerGridIndex(player, g_CellArr, g_GridSize) };
		if (player.facing == Direction::up) {
			for (int i{ -1 }; i < 2; ++i)
			{
				const int currentTile{ playerIndex - g_NrCols + i };
				if (!g_CellArr[currentTile].isObstacle
					&& currentTile > 0 && currentTile < g_GridSize)
				{
					FillRect(g_CellArr[currentTile].dstRect);
				}
			}
		}
		else if (player.facing == Direction::down && playerIndex + g_NrCols < g_GridSize)
		{
			for (int i{ -1 }; i < 2; ++i)
			{
				const int currentTile{ playerIndex + g_NrCols + i };
				if (!g_CellArr[currentTile].isObstacle
					&& currentTile > 0 && currentTile < g_GridSize)
				{
					FillRect(g_CellArr[currentTile].dstRect);
				}
			}
		}
		else if (player.facing == Direction::right)
		{
			for (int i{ -1 }; i < 2; ++i)
			{
				const int currentTile{ playerIndex + 1 + i * g_NrCols };
				if (!g_CellArr[currentTile].isObstacle
					&& currentTile > 0 && currentTile < g_GridSize)
				{
					FillRect(g_CellArr[currentTile].dstRect);
				}
			}
		}
		else if (player.facing == Direction::left)
		{
			for (int i{-1}; i < 2; ++i)
			{
				const int currentTile{ playerIndex - 1 + i * g_NrCols };
				if (!g_CellArr[currentTile].isObstacle
					&& currentTile > 0 && currentTile < g_GridSize)
				{
					FillRect(g_CellArr[currentTile].dstRect);
				}
			}
		}
	}
}
void DrawSpearReach(const Player& player)
{
	if (player.weaponInventory[player.selectedWeapon].type == WeaponType::spear) {
		int playerIndex{ GetPlayerGridIndex(player, g_CellArr, g_GridSize) };
		if (player.facing == Direction::up) {
			for (int i{1}; i < 3; ++i)
			{
				const int currentTile{ playerIndex - g_NrCols * i };
				if (!g_CellArr[currentTile].isObstacle
					&& currentTile > 0 && currentTile < g_GridSize)
				{
					FillRect(g_CellArr[currentTile].dstRect);
				}
			}
		}
		else if (player.facing == Direction::down && playerIndex + g_NrCols < g_GridSize)
		{
			for (int i{ 1 }; i < 3; ++i)
			{
				const int currentTile{ playerIndex + g_NrCols * i };
				if (!g_CellArr[currentTile].isObstacle
					&& currentTile > 0 && currentTile < g_GridSize)
				{
					FillRect(g_CellArr[currentTile].dstRect);
				}
			}
		}
		else if (player.facing == Direction::right)
		{
			for (int i{ 1 }; i < 3; ++i)
			{
				const int currentTile{ playerIndex + i };
				if (!g_CellArr[currentTile].isObstacle
					&& currentTile > 0 && currentTile < g_GridSize)
				{
					FillRect(g_CellArr[currentTile].dstRect);
				}
			}
		}
		else if (player.facing == Direction::left)
		{
			for (int i{ 1 }; i < 3; ++i)
			{
				const int currentTile{ playerIndex - i };
				if (!g_CellArr[currentTile].isObstacle
					&& currentTile > 0 && currentTile < g_GridSize)
				{
					FillRect(g_CellArr[currentTile].dstRect);
				}
			}
		}
	}
}
void DrawBowReach(const Player& player) 
{
	const int playerIndex{ GetPlayerGridIndex(player, g_CellArr, g_GridSize) };
	int nextIndex{ playerIndex};
	if (player.facing == Direction::up)
	{
		nextIndex -= g_NrCols;
		while (!HasEnemy(nextIndex, g_EnemyArr, g_MaxEnemiesPerRoom)
			&& !g_CellArr[nextIndex].isObstacle
			&& nextIndex > 0 && nextIndex < g_GridSize)
		{
			FillRect(g_CellArr[nextIndex].dstRect);
			nextIndex -= g_NrCols;
		} 
	}
	if (player.facing == Direction::down)
	{
		nextIndex += g_NrCols;
		while (!HasEnemy(nextIndex, g_EnemyArr, g_MaxEnemiesPerRoom)
			&& !g_CellArr[nextIndex].isObstacle
			&& nextIndex > 0 && nextIndex < g_GridSize)
		{
			FillRect(g_CellArr[nextIndex].dstRect);
			nextIndex += g_NrCols;
		}
	}
	if (player.facing == Direction::left)
	{
		nextIndex -= 1;
		while (!HasEnemy(nextIndex, g_EnemyArr, g_MaxEnemiesPerRoom)
			&& !g_CellArr[nextIndex].isObstacle
			&& nextIndex > 0 && nextIndex < g_GridSize)
		{
			FillRect(g_CellArr[nextIndex].dstRect);
			nextIndex -= 1;
		}
	}
	if (player.facing == Direction::right)
	{
		nextIndex += 1;
		while (!HasEnemy(nextIndex, g_EnemyArr, g_MaxEnemiesPerRoom)
			&& !g_CellArr[nextIndex].isObstacle
			&& nextIndex > 0 && nextIndex < g_GridSize)
		{
			FillRect(g_CellArr[nextIndex].dstRect);
			nextIndex += 1;
		} 
	}
}
#pragma endregion playerInputHandling

#pragma region projectileHandling
void CreateProjectile(Rectf location, std::string type, float direction, float speed, float damage)
{
	bool foundEmpty{ false };
	int emptyIndex{};
	for (int i{}; i < g_MaxProjectiles; ++i)
	{
		if (!foundEmpty && g_Projectiles[i].type == "")
		{
			emptyIndex = i;
			foundEmpty = true;
		}
	}
	if (foundEmpty)
	{
		g_Projectiles[emptyIndex] = InitializeProjectile(type, speed);
		g_Projectiles[emptyIndex].location = location;
		g_Projectiles[emptyIndex].location.width = g_Projectiles[emptyIndex].texture.width;
		g_Projectiles[emptyIndex].location.height = g_Projectiles[emptyIndex].texture.height;
		g_Projectiles[emptyIndex].direction = direction;
		g_Projectiles[emptyIndex].damage = damage;
	}
}
Projectile InitializeProjectile(std::string type, float speed)
{
	Projectile projectile{};
	projectile.texture = FetchTexture(type); // FOR TESTING PURPOSES
	projectile.type = type;
	projectile.speed = speed;
	return projectile;
}
void DrawProjectiles()
{
	for (int i{}; i < g_MaxProjectiles; ++i)
	{
		DrawTexture(g_Projectiles[i].texture, g_Projectiles[i].location);
	}
}
void UpdateProjectiles(float elapsedSec)
{
	for (int i{}; i < g_MaxProjectiles; ++i)
	{
		g_Projectiles[i].location.bottom += sinf(g_Projectiles[i].direction) * g_Projectiles[i].speed * elapsedSec;
		g_Projectiles[i].location.left += cosf(g_Projectiles[i].direction) * g_Projectiles[i].speed * elapsedSec;
		const float projectileSize{ 16.0f };
		const Point2f projectileCenter
		{
			g_Projectiles[i].location.left + g_Projectiles[i].location.width / 2,
			g_Projectiles[i].location.bottom + g_Projectiles[i].location.height / 2 };
		const Point2f projectileTip{ 
			projectileCenter.x + projectileSize * cosf(g_Projectiles[i].direction), 
			projectileCenter.y + projectileSize * sinf(g_Projectiles[i].direction) };
		// Damage appropriate target
		FillEllipse(projectileTip, 2.0f, 2.0f);
		for (int j{}; j < g_MaxEnemiesPerRoom; ++j)
		{
			if (IsPointInRect(g_EnemyArr[j].animationPos, projectileTip))
			{
				g_EnemyArr[j].health -= g_Projectiles[i].damage;
				DestroyProjectile(g_Projectiles[i]);
			}
		}
		if (IsPointInRect(g_Player.animationPos, projectileTip))
		{
			g_Player.health -= g_Projectiles[i].damage;
			DestroyProjectile(g_Projectiles[i]);
			g_Player.isPlayerHit = true; // new line
		}
		for (int j{}; j < g_GridSize; ++j)
		{
			if (IsPointInRect(g_CellArr[j].dstRect, projectileTip) && g_CellArr[j].isObstacle)
			{
				DestroyProjectile(g_Projectiles[i]);
			}
		}
		if (projectileTip.x < 0 || projectileTip.y < 0 || projectileTip.x > g_WindowWidth || projectileTip.y > g_WindowHeight)
		{
			DestroyProjectile(g_Projectiles[i]);
		}
		if (IsPointInRect(g_Boss.dstRect, projectileTip))
		{
			g_Boss.health -= g_Projectiles[i].damage;
			DestroyProjectile(g_Projectiles[i]);
		}
	}
}
void DestroyProjectile(Projectile& projectile)
{
	Projectile nullProjectile{};
	projectile = nullProjectile;
}
void ClearProjectiles()
{
	for (int i{}; i < g_MaxProjectiles; ++i)
	{
		DestroyProjectile(g_Projectiles[i]);
	}
}
#pragma endregion projectileHandling

#pragma region weaponHandling
// Weapon Handling
void InitWeapons()
{
	g_Weapons[0] = InitializeWeapon("basic_sword", "basic_sword_up", WeaponType::sword, 2.5f);
	g_Weapons[1] = InitializeWeapon("bow", "bow_right", WeaponType::bow, 1.0f);
	g_Weapons[2] = InitializeWeapon("basic_axe", "basic_axe_up", WeaponType::sword, 4.0f);
	g_Weapons[3] = InitializeWeapon("spear", "spear_up", WeaponType::spear, 3.f);
	g_Weapons[4] = InitializeWeapon("giant_blade", "giant_blade_up", WeaponType::sword, 8.0f);
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

void SetWeaponCooldown(const Weapon& weapon)
{
	if (weapon.name == "spear")
	{
		g_Player.isWeaponOnCooldown = true;
		g_Player.coolDownTimer = 0.2f;
	}
	if (weapon.name == "basic_axe")
	{
		g_Player.isWeaponOnCooldown = true;
		g_Player.coolDownTimer = 0.1f;
	}
	if (weapon.name == "giant_blade")
	{
		g_Player.isWeaponOnCooldown = true;
		g_Player.coolDownTimer = 1.0f;
	}
}
void UpdateWeaponAnimation(float elapsedSec)
{
	WeaponAnimation& animation{ g_Player.weaponAnimation };
	const Weapon& weapon{ g_Player.weaponInventory[g_Player.selectedWeapon] };
	if (!animation.isPlaying || !animation.enabled)
	{
		switch (g_Player.facing)
		{
		case Direction::up:
			animation.position.bottom = g_Player.animationPos.bottom;
			animation.position.left = g_Player.animationPos.left + weapon.texture.width / 3;
			break;
		case Direction::down:
			animation.position.bottom = g_Player.animationPos.bottom - weapon.texture.width / 3;
			animation.position.left = g_Player.animationPos.left - weapon.texture.width / 3;
			break;
		case Direction::right:
			animation.position.bottom = g_Player.animationPos.bottom - weapon.texture.width / 3;
			animation.position.left = g_Player.animationPos.left;
			break;
		case Direction::left:
			animation.position.bottom = g_Player.animationPos.bottom - weapon.texture.width / 3;
			animation.position.left = g_Player.animationPos.left - weapon.texture.width / 2;
			break;
		}
	}
	else
	if ((animation.isPlaying && animation.elapsedTime > animation.playTime) || weapon.type == WeaponType::bow)
	{
		animation.isPlaying = false;
		SetWeaponCooldown(weapon);
		animation.elapsedTime = 0;
	}
	else if (animation.isPlaying && weapon.type == WeaponType::sword && !g_Player.isWeaponOnCooldown)
	{
		const float cellSize{ 64.f };
		const float animationDist{ 2 * cellSize };
		animation.playTime = 0.15f;
		switch (g_Player.facing)
		{
		case Direction::up:
			animation.position.left = g_Player.animationPos.left - cellSize + animation.elapsedTime * animationDist / animation.playTime;
			animation.position.bottom = g_Player.animationPos.bottom + cellSize/2;
			break;
		case Direction::down:
			animation.position.left = g_Player.animationPos.left + cellSize - animation.elapsedTime * animationDist / animation.playTime;
			animation.position.bottom = g_Player.animationPos.bottom - cellSize/2;
			break;
		case Direction::right:
			animation.position.left = g_Player.animationPos.left + cellSize/2;
			animation.position.bottom = g_Player.animationPos.bottom + cellSize - animation.elapsedTime * animationDist / animation.playTime;
			break;
		case Direction::left:
			animation.position.left = g_Player.animationPos.left - cellSize/2;
			animation.position.bottom = g_Player.animationPos.bottom + cellSize - animation.elapsedTime * animationDist / animation.playTime;
			break;
		}
		animation.elapsedTime += elapsedSec;
	}
	else if (animation.isPlaying && weapon.type == WeaponType::spear && !g_Player.isWeaponOnCooldown)
	{
		const float cellSize{ 64.f };
		const float animationDist{ 1 * cellSize };
		animation.playTime = 0.15f;
		switch (g_Player.facing)
		{
		case Direction::up:
			animation.position.left = g_Player.animationPos.left;
			animation.position.bottom = g_Player.animationPos.bottom + cellSize/2 + animation.elapsedTime * animationDist / animation.playTime;
			break;
		case Direction::down:
			animation.position.left = g_Player.animationPos.left;
			animation.position.bottom = g_Player.animationPos.bottom - cellSize/2 - animation.elapsedTime * animationDist / animation.playTime;
			break;
		case Direction::right:
			animation.position.left = g_Player.animationPos.left + cellSize/2 + animation.elapsedTime * animationDist / animation.playTime;;
			animation.position.bottom = g_Player.animationPos.bottom;
			break;
		case Direction::left:
			animation.position.left = g_Player.animationPos.left - cellSize/2 - animation.elapsedTime * animationDist / animation.playTime;;
			animation.position.bottom = g_Player.animationPos.bottom;
			break;
		}
		animation.elapsedTime += elapsedSec;
	}
	else {
		switch (g_Player.facing)
		{
		case Direction::up:
			animation.position.bottom = g_Player.animationPos.bottom;
			animation.position.left = g_Player.animationPos.left + weapon.texture.width / 3;
			break;
		case Direction::down:
			animation.position.bottom = g_Player.animationPos.bottom - weapon.texture.width / 3;
			animation.position.left = g_Player.animationPos.left - weapon.texture.width / 3;
			break;
		case Direction::right:
			animation.position.bottom = g_Player.animationPos.bottom - weapon.texture.width / 3;
			animation.position.left = g_Player.animationPos.left;
			break;
		case Direction::left:
			animation.position.bottom = g_Player.animationPos.bottom - weapon.texture.width / 3;
			animation.position.left = g_Player.animationPos.left - weapon.texture.width / 2;
			break;
		}
	}
}
void DrawWeaponAnimation()
{
	WeaponAnimation& animation{ g_Player.weaponAnimation };
	switch (g_Player.facing)
	{
	case Direction::up:
		DrawTexture(FetchTexture(g_Player.weaponInventory[g_Player.selectedWeapon].name + "_up"), animation.position);
		break;
	case Direction::left:
		DrawTexture(FetchTexture(g_Player.weaponInventory[g_Player.selectedWeapon].name + "_left"), animation.position);
		break;
	case Direction::right:
		DrawTexture(FetchTexture(g_Player.weaponInventory[g_Player.selectedWeapon].name + "_right"), animation.position);
		break;
	case Direction::down:
		DrawTexture(FetchTexture(g_Player.weaponInventory[g_Player.selectedWeapon].name + "_down"), animation.position);
		break;
	}

}
#pragma endregion weaponHandling

#pragma region itemHandling
// Item Handling
void InitItems() 
{
	g_Items[0] = InitializeItem("health_potion", "health_potion", ItemType::potion, EffectType::health);
	g_Items[1] = InitializeItem("speed_potion", "speed_potion", ItemType::potion, EffectType::speedBoost, 10.f);
	g_Items[2] = InitializeItem("strength_potion", "strength_potion", ItemType::potion, EffectType::strengthBoost, 10.f);
	g_Items[3] = InitializeItem("regen_potion", "regen_potion", ItemType::potion, EffectType::regeneration, 10.f, 2.f);
	g_Items[4] = InitializeItem("shield_potion", "shield_potion", ItemType::potion, EffectType::shielding, 10.f, 20.f);
}
Item InitializeItem(const std::string& itemName, const std::string& textureName, const ItemType& type, const EffectType effect, const float duration, const float modifier)
{
	Item item{};
	item.name = itemName;
	item.texture = FetchTexture(textureName);
	item.type = type;
	item.effect.effectType = effect;
	item.effect.duration = duration;
	item.effect.timeRemaining = duration;
	item.effect.modifier = modifier;
	item.count = 1;
	return item;
}
void UpdateStatusEffects(float elapsedSec)
{
	for (int i{}; i < g_ItemInventorySize; ++i)
	{
		StatusEffect& currentEffect = g_Player.effects[i];
		const float speedAmp{ 1.5f };
		const float strengthAmp{ 2.0f };
		// HEALTH POTION
		if (currentEffect.effectType == EffectType::health)
		{
			if (currentEffect.duration - currentEffect.timeRemaining < 0.0001f)
			{
				g_Player.health += 10.f * currentEffect.modifier;
			}
			if (g_Player.health > g_Player.maxHealth)
			{
				g_Player.health = g_Player.maxHealth;
			}
		}
		else if (currentEffect.effectType == EffectType::regeneration)
		{
			g_Player.health += elapsedSec * currentEffect.modifier;
			if (g_Player.health > g_Player.maxHealth)
			{
				g_Player.health = g_Player.maxHealth;
			}
		}
		else if (currentEffect.effectType == EffectType::speedBoost)
		{
			if (currentEffect.duration - currentEffect.timeRemaining < 0.0001f)
			{
				g_Player.speedModifier *= speedAmp * currentEffect.modifier;
			}
		}
		else if (currentEffect.effectType == EffectType::strengthBoost)
		{
			if (currentEffect.duration - currentEffect.timeRemaining < 0.0001f)
			{
				for (int i{}; i < g_WeaponInventorySize; ++i)
				{
					g_Player.weaponInventory[i].damageOutput *= strengthAmp * currentEffect.modifier;
				}
			}
		}
		else if (currentEffect.effectType == EffectType::shielding)
		{
			if (currentEffect.duration - currentEffect.timeRemaining < 0.0001f) 
			{
				g_Player.maxHealth += currentEffect.modifier;
				g_Player.health += currentEffect.modifier;
			}
		}
		
		currentEffect.timeRemaining -= elapsedSec;
		
		if (currentEffect.timeRemaining <= 0)
		{
			if (currentEffect.effectType == EffectType::speedBoost)
			{
				g_Player.speedModifier /= speedAmp * currentEffect.modifier * currentEffect.stacks;
			}
			else if (currentEffect.effectType == EffectType::strengthBoost)
			{
				for (int i{}; i < g_WeaponInventorySize; ++i)
				{
					g_Player.weaponInventory[i].damageOutput /= strengthAmp * currentEffect.modifier * currentEffect.stacks;
				}
			}
			else if (currentEffect.effectType == EffectType::shielding)
			{
				g_Player.maxHealth -= currentEffect.modifier * currentEffect.stacks;
				g_Player.health -= currentEffect.modifier * currentEffect.stacks;
				if (g_Player.health < 0)
				{
					g_Player.health = 5.f;
				}
			}
			StatusEffect nullEffect{};
			currentEffect = nullEffect;
		}
	}
}
void DrawStatusEffects(Player& player)
{
	Point2f playerCenter{ player.animationPos.left + player.animationPos.width / 2, player.animationPos.bottom + player.animationPos.height / 2 };
	float radius{ player.animationPos.width };
	for (int i{}; i < g_ItemInventorySize; ++i)
	{
		switch (player.effects[i].effectType)
		{
		case EffectType::health:
			SetColor(1.0f, 0.0f, 0.0f, 0.05f);
			break;
		case EffectType::regeneration:
			SetColor(0.0f, 1.0f, 0.0f, 0.05f);
			break;
		case EffectType::shielding:
			SetColor(0.0f, 1.0f, 1.0f, 0.05f);
			break;
		case EffectType::speedBoost:
			SetColor(1.0f, 1.0f, 0.0f, 0.05f);
			break;
		case EffectType::strengthBoost:
			SetColor(0.0f, 0.0f, 1.0f, 0.05f);
			break;
		default:
			SetColor(0.0f, 0.0f, 0.0f, 0.0f);
		}
		FillEllipse(playerCenter, radius, radius);
	}
}
Item FetchItem(const std::string& name)
{
	for (int index{}; index < g_ItemsInGame; ++index)
	{
		if (g_Items[index].name == name)
		{
			return g_Items[index];
		}
	}
	Item no_item{};
	return no_item;
}
void RollForDrop(Enemy& enemy)
{
	int location{ GetEnemyGridIndex(enemy, g_CellArr, g_GridSize) };
	int dieRoll{ rand() % 200 };
	std::string itemToSpawn{};
	InteractableType type{};
	if (dieRoll < 4)
	{
		itemToSpawn = "health_potion";
		type = InteractableType::itemDrop;
	}
	else if (dieRoll < 9)
	{
		itemToSpawn = "strength_potion";
		type = InteractableType::itemDrop;
	}
	else if (dieRoll < 14)
	{
		itemToSpawn = "speed_potion";
		type = InteractableType::itemDrop;
	}
	else if (dieRoll < 17)
	{
		itemToSpawn = "basic_axe";
		type = InteractableType::weaponDrop;
	}
	else if (dieRoll < 22)
	{
		itemToSpawn = "regen_potion";
		type = InteractableType::itemDrop;
	}
	else if (dieRoll < 27)
	{
		itemToSpawn = "shield_potion";
		type = InteractableType::itemDrop;
	}
	else if (dieRoll < 30)
	{
		itemToSpawn = "spear";
		type = InteractableType::weaponDrop;
	}

	const int underTopDoor{ 19 };
	const int nextToLeftDoor{ 52 };
	const int nextTorightDoor{ 63 };
	const int abovebottomDoor{ 97 };
	if (itemToSpawn != ""
		&& location != underTopDoor
		&& location != nextToLeftDoor
		&& location != nextTorightDoor
		&& location != abovebottomDoor)
	{
		SpawnInteractable(itemToSpawn, location, type);
		AddInteractableToRoom(g_CurrentRoom, itemToSpawn, location, type);
	}
}
#pragma endregion itemHandling

#pragma region interactableHandling
// Interactable Handling
void SpawnInteractable(std::string name, int location, InteractableType type)
{
	bool spawned{ false };
	for (int i{}; i < g_MaxInteractablesRoom; ++i)
	{
		if (g_Interactables[i].name == "" && !spawned)
		{
			g_Interactables[i] = InitializeInteractable(name, type);
			g_Interactables[i].dstRect = g_CellArr[location].dstRect;
			g_Interactables[i].location = location;
			spawned = true;
		}
	}
}
void SpawnInteractablesInRoom(const Room & room)
{
	for (int i{}; i < g_MaxInteractablesRoom; ++i) {
		const InteractableShorthand& currentInteractable{ room.interactableShort[i] };
		if (!currentInteractable.pickedUp) {
			SpawnInteractable(currentInteractable.name, currentInteractable.location, currentInteractable.type);
		}
	}
}

void ReplaceInteractableInRoom(const Room& room, std::string interactableToReplace, std::string interactableReplacement, int location)
{
	for (int i{}; i < g_NrRoomsPerLevel; ++i)
	{
		if (g_Level[i].id == g_CurrentRoom.id) {
			bool replaced{ false };
			for (int j{}; j < g_MaxInteractablesRoom; ++j)
			{
				if (g_Level[i].interactableShort[j].name == interactableToReplace &&
					g_Level[i].interactableShort[j].location == location)
				{
					g_Level[i].interactableShort[j].name = interactableReplacement;
					replaced = true;
				}
			}
		}
	}
}
void SetInteractablePickedUp(const Room& room, std::string interactableName, int location)
{
	for (int i{}; i < g_NrRoomsPerLevel; ++i)
	{
		if (g_Level[i].id == g_CurrentRoom.id) 
		{
			for (int j{}; j < g_MaxInteractablesRoom; ++j)
			{
				if (g_Level[i].interactableShort[j].name == interactableName &&
					g_Level[i].interactableShort[j].location == location)
				{
					g_Level[i].interactableShort[j].pickedUp = true;
				}
			}
		}
	}
}
void AddInteractableToRoom(const Room& room, std::string interactableName, int location, InteractableType type)
{
	bool addedToRoom{ false };
	for (int i{}; i < g_NrRoomsPerLevel; ++i)
	{
		if (g_Level[i].id == room.id)
		{
			for (int j{}; j < g_MaxInteractablesRoom; ++j)
			{
				if (g_Level[i].interactableShort[j].name == "" && !addedToRoom)
				{
					g_Level[i].interactableShort[j].location = location;
					g_Level[i].interactableShort[j].name = interactableName;
					g_Level[i].interactableShort[j].pickedUp = false;
					g_Level[i].interactableShort[j].type = type;
					addedToRoom = true;
				}
			}
		}
	}
}

void PickUpInteractable(int index, int location)
{
	Interactable placeholder{};
	bool& pickedUp{ g_Interactables[index].pickedUp };
	if (g_Interactables[index].type == InteractableType::weaponDrop) {
		for (int i{}; i < g_WeaponInventorySize; ++i)
		{
			if (g_Player.weaponInventory[i].name == "" && !pickedUp)
			{
				g_Player.weaponInventory[i] = g_Interactables[index].linkedWeapon;
				SetInteractablePickedUp(g_CurrentRoom, g_Interactables[index].name, location);
				g_Interactables[index] = placeholder;
				pickedUp = true;
			}
		}
		if (!pickedUp)
		{
			Weapon weaponHolder = g_Player.weaponInventory[g_Player.selectedWeapon];
			g_Player.weaponInventory[g_Player.selectedWeapon] = g_Interactables[index].linkedWeapon;
			g_Interactables[index].linkedWeapon = weaponHolder;
			ReplaceInteractableInRoom(g_CurrentRoom, g_Player.weaponInventory[g_Player.selectedWeapon].name, weaponHolder.name, location);
		}
	}
	else if (g_Interactables[index].type == InteractableType::itemDrop) {
		for (int i{}; i < g_ItemInventorySize; ++i)
		{
			if (g_Player.itemInventory[i].name == g_Interactables[index].linkedItem.name && !pickedUp)
			{
				++g_Player.itemInventory[i].count; 
				SetInteractablePickedUp(g_CurrentRoom, g_Interactables[index].name, location);
				g_Interactables[index] = placeholder;
				pickedUp = true;
			}
			else if (g_Player.itemInventory[i].name == "" && !pickedUp)
			{
				g_Player.itemInventory[i] = g_Interactables[index].linkedItem;
				SetInteractablePickedUp(g_CurrentRoom, g_Interactables[index].name, location);
				g_Interactables[index] = placeholder;
				pickedUp = true;
			}
		}
		if (!pickedUp)
		{
			Item itemHolder = g_Player.itemInventory[g_Player.selectedItem];
			g_Player.itemInventory[g_Player.selectedItem] = g_Interactables[index].linkedItem;
			g_Interactables[index].linkedItem = itemHolder;
			ReplaceInteractableInRoom(g_CurrentRoom, g_Player.itemInventory[g_Player.selectedItem].name, itemHolder.name, location);
		}
	}
}

Interactable InitializeInteractable(const std::string& linkedItem, const InteractableType& type)
{
	Interactable initializedInteractable{};
	initializedInteractable.name = linkedItem;
	initializedInteractable.type = type;
	if (linkedItem != "") initializedInteractable.pickedUp = false;
	if (type == InteractableType::weaponDrop)
	{
		initializedInteractable.linkedWeapon = FetchWeapon(linkedItem);
	}
	else if (type == InteractableType::itemDrop)
	{
		initializedInteractable.linkedItem = FetchItem(linkedItem);
	}
	return initializedInteractable;
}
void DrawInteractables()
{
	SetColor(g_Blue);
	for (int i{}; i < g_MaxInteractablesRoom; ++i)
	{
		if (g_Interactables[i].type == InteractableType::weaponDrop && !g_Interactables[i].pickedUp)
		{
			DrawTexture(g_Interactables[i].linkedWeapon.texture, g_Interactables[i].dstRect);
		}
		else if (g_Interactables[i].type == InteractableType::itemDrop && !g_Interactables[i].pickedUp)
		{
			DrawTexture(g_Interactables[i].linkedItem.texture, g_Interactables[i].dstRect);
		}
	}
	SetColor(g_White);
}
void ClearInteractables()
{
	Interactable nullInteractable{};
	for (int i{}; i < g_MaxInteractablesRoom; ++i)
	{
		g_Interactables[i] = nullInteractable;
	}
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
	} while (spawn == playerIndex || cellArr[spawn].isObstacle == true || HasEnemy(spawn, g_EnemyArr, g_MaxEnemiesPerRoom));
	return spawn;
}
void InitEnemies()
{
	for (int i{}; i < g_MaxEnemiesPerRoom; ++i)
	{
		g_EnemyArr[i] = InitializeEnemy("none");
	}
}
void SpawnEnemies(const EnemyShorthand enemies[])
{
	for (int i{}; i < g_MaxEnemiesPerRoom; ++i)
	{
		if (enemies[i].type != "")
		{
			g_EnemyArr[i] = InitializeEnemy(enemies[i].type);
			g_EnemyArr[i].dstRect = g_CellArr[enemies[i].location].dstRect;
			g_EnemyArr[i].animationPos = g_EnemyArr[i].dstRect;
		}
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
	else if (enemyName == "bat") {
		return InitializeEnemy(EnemyType::basic, "enemy_bat", 4.f, 1.f, 0.5f, 4);
	}
	else if (enemyName == "archer") {
		return InitializeEnemy(EnemyType::ranged, "enemy_archer", 10.f, 2.f, 0.5f, 5);
	}
	else if (enemyName == "crate")
	{
		return InitializeEnemy(EnemyType::destructible, "crate", 4.0f, 0.0f, 0.0f, 0);
	}	
	else if (enemyName == "rusher")
	{
		return InitializeEnemy(EnemyType::basic, "enemy_rusher", 1.f, 4.f, 0.2f, 10);
	}
	else if (enemyName == "gnawer")
	{
		return InitializeEnemy(EnemyType::basic, "enemy_gnawer", 10.f, 4.f, 0.5f, 10);
	}
	else if (enemyName == "summoner")
	{
		return InitializeEnemy(EnemyType::summoner, "enemy_summoner", 15.f, 0.f, 0.2f, 5);
	}
	else if (enemyName == "necromancer")
	{
		return InitializeEnemy(EnemyType::necromancer, "enemy_necromancer", 15.f, 0.f, 0.2f, 5);
	}
	else if (enemyName == "necrospawn")
	{
		return InitializeEnemy(EnemyType::basic, "enemy_necrospawn", 15.f, 5.0f, 0.8f, 10);
	}
	else
		return InitializeEnemy(EnemyType::basic, "not_found", 0.0f, 0.0f, 100.f, 0);
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
	for (int index{}; index < g_MaxEnemiesPerRoom; ++index)
	{
		if (enemyArr[index].health > 0) 
		{
			DrawEnemyHealth(enemyArr[index]);
		}
	}
}
void ClearEnemies()
{
	Enemy clearedEnemy{};
	for (int i{}; i < g_MaxEnemiesPerRoom; ++i)
	{
		g_EnemyArr[i] = clearedEnemy;
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
			&& !HasEnemy(newIndexX, g_EnemyArr, g_MaxEnemiesPerRoom) };
		bool canGoVertDiagCheck{ isInRangeDiagonal && !cellArr[newIndexY].isObstacle 
			&& !HasEnemy(newIndexY, g_EnemyArr, g_MaxEnemiesPerRoom)	};
		bool canGoHorHorCheck{   isInRangeX && !cellArr[newIndexX].isObstacle && !HasEnemy(newIndexX, g_EnemyArr, g_MaxEnemiesPerRoom) };

		bool canGoVertHorCheck{  isInRangeX && (cellArr[newIndexX].isObstacle ||  HasEnemy(newIndexX, g_EnemyArr, g_MaxEnemiesPerRoom))
											&& !cellArr[newIndexY].isObstacle && !HasEnemy(newIndexY, g_EnemyArr, g_MaxEnemiesPerRoom) };

		bool canGoVertVertCheck{ isInRangeY	&& !cellArr[newIndexY].isObstacle && !HasEnemy(newIndexY, g_EnemyArr, g_MaxEnemiesPerRoom) };

		bool canGoHorVertCheck{  isInRangeY && (cellArr[newIndexY].isObstacle ||  HasEnemy(newIndexY, g_EnemyArr, g_MaxEnemiesPerRoom))
											&& !cellArr[newIndexX].isObstacle && !HasEnemy(newIndexX, g_EnemyArr, g_MaxEnemiesPerRoom) };

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
			g_Player.animState = AnimStates::hit;
			g_Player.isPlayerHit = true; // new line
			g_Player.health -= enemy.damageOutput;
			enemy.animationPos.left -= (enemy.animationPos.left - g_Player.dstRect.left) / 2;
		}
		else if (isNextToPlayerY && g_Player.health > 0.f)
		{
			g_Player.animState = AnimStates::hit;
			g_Player.isPlayerHit = true; // new line
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

		bool isTooCloseToPlayerX{ distX < enemy.viewRange&& distY == 0 };
		bool isTooCloseToPlayerY{ distX == 0 && distY < enemy.viewRange };
		bool inRange{ distX == 0 || distY == 0 };

		int escapeIndexX{ enemyIndex + (indexDiffX > 0 ? -1 : 1) };
		int escapeIndexY{ enemyIndex + (indexDiffY > 0 ? -g_NrCols : g_NrCols) };

		int getInRangeIdxX{ enemyIndex + (indexDiffX > 0 ? 1 : -1) };
		int getInRangeIdxY{ enemyIndex + (indexDiffY > 0 ? g_NrCols : -g_NrCols) };

		bool canEscapeHorCheck{!cellArr[escapeIndexX].isObstacle && !HasEnemy(escapeIndexX, g_EnemyArr, g_MaxEnemiesPerRoom) };
		bool canEscapeVertCheck{!cellArr[escapeIndexY].isObstacle && !HasEnemy(escapeIndexY, g_EnemyArr, g_MaxEnemiesPerRoom) };

		bool canGetInRangeHorCheck{ !cellArr[getInRangeIdxX].isObstacle && !HasEnemy(getInRangeIdxX, g_EnemyArr, g_MaxEnemiesPerRoom) };
		bool canGetInRangeVertCheck{ !cellArr[getInRangeIdxY].isObstacle && !HasEnemy(getInRangeIdxY, g_EnemyArr, g_MaxEnemiesPerRoom) };
		if (isTooCloseToPlayerX && isTooCloseToPlayerY && (canEscapeHorCheck || canEscapeVertCheck))
		{
			if (canEscapeVertCheck) enemy.dstRect = cellArr[escapeIndexY].dstRect;
			else if (canEscapeHorCheck) enemy.dstRect = cellArr[escapeIndexX].dstRect;
			FireArrowFromEnemy(cellArr, indexDiffX, indexDiffY, enemy, enemyIndex);
		}
		else if (isTooCloseToPlayerX && (canEscapeHorCheck || canEscapeVertCheck))
		{
			if (canEscapeHorCheck) enemy.dstRect = cellArr[escapeIndexX].dstRect;
			else if (canEscapeVertCheck) enemy.dstRect = cellArr[escapeIndexY].dstRect;
			FireArrowFromEnemy(cellArr, indexDiffX, indexDiffY, enemy, enemyIndex);
		}
		else if (isTooCloseToPlayerY && (canEscapeHorCheck || canEscapeVertCheck))
		{
			if (canEscapeVertCheck) enemy.dstRect = cellArr[escapeIndexY].dstRect;
			else if (canEscapeHorCheck) enemy.dstRect = cellArr[escapeIndexX].dstRect;
			FireArrowFromEnemy(cellArr, indexDiffX, indexDiffY, enemy, enemyIndex);
		}
		else if (!inRange && (canGetInRangeHorCheck || canGetInRangeVertCheck))
		{
			if (canGetInRangeVertCheck) enemy.dstRect = cellArr[getInRangeIdxY].dstRect;
			else if (canGetInRangeHorCheck) enemy.dstRect = cellArr[getInRangeIdxX].dstRect;
		}
		else if (inRange)
		{
			FireArrowFromEnemy(cellArr, indexDiffX, indexDiffY, enemy, enemyIndex);
		}
	}
}
void SummonerAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize)
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

		bool isTooCloseToPlayerX{ distX < enemy.viewRange };
		bool isTooCloseToPlayerY{ distY < enemy.viewRange };
		bool inRange{ distX == 0 || distY == 0 };

		int escapeIndexX{ enemyIndex };
		if      (indexDiffX      > 0) escapeIndexX -= 1;
		else if (indexDiffX      < 0) escapeIndexX += 1;
		else if (rand() % 2     == 0) escapeIndexX -= 1;
		else                          escapeIndexX += 1;

		int escapeIndexY{ enemyIndex };
		if      (indexDiffY      > 0) escapeIndexY -= g_NrCols;
		else if (indexDiffY      < 0) escapeIndexY += g_NrCols;
		else if (rand() % 2     == 0) escapeIndexY -= g_NrCols;
		else                          escapeIndexY += g_NrCols;

		int getInRangeIdxX{ enemyIndex + (indexDiffX > 0 ? 1 : -1) };
		int getInRangeIdxY{ enemyIndex + (indexDiffY > 0 ? g_NrCols : -g_NrCols) };

		bool canEscapeHorCheck{ !cellArr[escapeIndexX].isObstacle && !HasEnemy(escapeIndexX, g_EnemyArr, g_MaxEnemiesPerRoom) };
		bool canEscapeVertCheck{ !cellArr[escapeIndexY].isObstacle && !HasEnemy(escapeIndexY, g_EnemyArr, g_MaxEnemiesPerRoom) };

		bool canGetInRangeHorCheck{ !cellArr[getInRangeIdxX].isObstacle && !HasEnemy(getInRangeIdxX, g_EnemyArr, g_MaxEnemiesPerRoom) };
		bool canGetInRangeVertCheck{ !cellArr[getInRangeIdxY].isObstacle && !HasEnemy(getInRangeIdxY, g_EnemyArr, g_MaxEnemiesPerRoom) };

		if (enemy.type != EnemyType::necromancer && CheckForSummonedEnemy("enemy_gnawer") == false)
		{
			SummonEnemy("gnawer", enemyIndex);
			
		}
		if (isTooCloseToPlayerX && isTooCloseToPlayerY)
		{
			if (movementDecider == 0 && canEscapeHorCheck) enemy.dstRect = cellArr[escapeIndexX].dstRect;
			else if (canEscapeVertCheck) enemy.dstRect = cellArr[escapeIndexY].dstRect;
		}
		else if (isTooCloseToPlayerX)
		{
			if (canEscapeVertCheck) enemy.dstRect = cellArr[escapeIndexY].dstRect;
			else if (canEscapeHorCheck) enemy.dstRect = cellArr[escapeIndexX].dstRect;
		}
		else if (isTooCloseToPlayerY)
		{
			if (canEscapeHorCheck) enemy.dstRect = cellArr[escapeIndexX].dstRect;
			else if (canEscapeVertCheck) enemy.dstRect = cellArr[escapeIndexY].dstRect;
		}
	}
}
bool CheckForSummonedEnemy(std::string enemyToCheckName)
{
	for (int i{}; i < g_MaxEnemiesPerRoom; i++)
	{
		if (g_EnemyArr[i].texture.id == FetchTexture(enemyToCheckName).id)
		{
			return true;
		}
	}
	return false;
}
bool NecromancerCheck()
{
	int location{};
	for (int i{}; i < g_MaxEnemiesPerRoom; ++i)
	{
		if (g_EnemyArr[i].type == EnemyType::necromancer)
		{
			return true;
		}
	}
	return false;
}
void SummonEnemy(std::string enemyToSummonName, int location)
{
	bool summoned{ false };
	for (int i{}; i < g_MaxEnemiesPerRoom; i++)
	{
		if (g_EnemyArr[i].maxHealth == 0.f && !summoned)
		{
			g_EnemyArr[i] = InitializeEnemy(enemyToSummonName);
			g_EnemyArr[i].dstRect = g_CellArr[location].dstRect;
			g_EnemyArr[i].animationPos = g_EnemyArr[i].dstRect;
			summoned = true;
		}
	}
}
void FireArrowFromEnemy(Cell cellArr[], const int indexDiffX, const int indexDiffY, Enemy& enemy, int enemyIndex)
{
	Rectf spawnRect{ enemy.animationPos };
	const float projectileSpeed{ 500.f };
	if (indexDiffX > 0) {
		float angle{ 0 };
		spawnRect.left += spawnRect.width / 3;
		CreateProjectile(spawnRect, "arrow_right", angle, projectileSpeed, enemy.damageOutput);
	}
	else if (indexDiffX < 0)
	{
		spawnRect.left -= spawnRect.width / 3;
		float angle{ g_Pi };
		CreateProjectile(spawnRect, "arrow_left", angle, projectileSpeed, enemy.damageOutput);
	}
	else if (indexDiffY > 0) {
		float angle{ 3 * g_Pi / 2 };
		spawnRect.bottom -= spawnRect.width / 3;
		CreateProjectile(spawnRect, "arrow_down", angle, projectileSpeed, enemy.damageOutput);
	}
	else if (indexDiffY < 0)
	{
		spawnRect.bottom += spawnRect.width / 3;
		float angle{ g_Pi / 2 };
		CreateProjectile(spawnRect, "arrow_up", angle, projectileSpeed, enemy.damageOutput);
	}
}
void UpdateEnemies(float elapsedSec, Enemy enemyArr[], int enemyArrSize, Cell cellArr[], int cellArrSize)
{
	for (int index{}; index < enemyArrSize; ++index)
	{
		
		if (enemyArr[index].health <= 0.f && enemyArr[index].maxHealth != 0)
		{
			if (NecromancerCheck() && enemyArr[index].texture.id != FetchTexture("enemy_necrospawn").id)
			{
				SummonEnemy("necrospawn", GetEnemyGridIndex(enemyArr[index], cellArr, cellArrSize));
			}
			if (g_CurrentRoom.id != RoomID::bossRoom
				&& enemyArr[index].texture.id != FetchTexture("enemy_gnawer").id
				&& enemyArr[index].texture.id != FetchTexture("enemy_necrospawn").id)
			{
				RollForDrop(enemyArr[index]);
			}
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
		else if (enemyArr[index].type == EnemyType::summoner)
		{
			SummonerAI(elapsedSec, enemyArr[index], cellArr, cellArrSize);
		}
		else if (enemyArr[index].type == EnemyType::necromancer)
		{
			SummonerAI(elapsedSec, enemyArr[index], cellArr, cellArrSize);
		}
		UpdateAnimationPos(elapsedSec, enemyArr[index]);
	}
}
#pragma endregion enemyHandling

#pragma region bossHandling
bool IsBossDead(Boss& boss)
{
	if (boss.health <= 0)
	{
		boss.AIState = BossAIStates::death;
		boss.animState = AnimStates::death;
		g_Game = GameStates::gameWon;
		return true;
	}
	else
	{
		return false;
	}
}
bool IsBossOnTilesToScan(const Boss& boss, int tilesToScan[], int currentTile)
{
	Rectf tile{ g_CellArr[tilesToScan[currentTile]].dstRect };
	return IsOverlapping(boss.dstRect, tile);
}

float BossDistanceToChargePoint(const Boss& boss)
{
	return sqrtf(powf(boss.dstRect.left - boss.chargeEndPoint.x, 2) + 
				 powf(boss.dstRect.bottom - boss.chargeEndPoint.y, 2));
}
float GetAngle(Boss& boss)
{
	return atan2(boss.delta.y , boss.delta.x);
}

void InitBoss(Boss& boss)
{
	boss.AIState = BossAIStates::idle;
	boss.animState = AnimStates::idleRight;
	boss.maxHealth = 300.f;
	boss.health = boss.maxHealth;
	boss.damageOutput = 0.8f;
	boss.bossAIStateTimer = 0.f;
	boss.bossAttackPlayerTimer = 0.f;
	boss.bossVoiceLineTimer = 0.f;
	boss.bossPlayerAngle = 0.f;
	boss.chargeEndPoint.x = 0;
	boss.chargeEndPoint.y = 0;
	boss.speed = 500.f;
	boss.isTalking = false;

	boss.sprite.texture = FetchTexture("boss_anim_idle_right");
	boss.sprite.cols = 4;
	boss.sprite.frames = 4;
	boss.sprite.currentFrame = 0;
	boss.sprite.accumulatedTime = 0.0f;
	boss.sprite.frameTime = 1 / 8.0f;

	boss.dstRect.bottom = g_CellArr[53].dstRect.bottom;
	boss.dstRect.left = g_CellArr[53].dstRect.left;
	boss.dstRect.width = 2 * g_CellArr[53].dstRect.width;
	boss.dstRect.height = 2 * g_CellArr[53].dstRect.height;


	boss.srcRect.width = boss.sprite.texture.width / boss.sprite.cols;
	boss.srcRect.height = boss.sprite.texture.height;
	boss.srcRect.bottom = boss.srcRect.height;

}
void DrawBossHealth(Boss& boss)
{
	if (boss.health != boss.maxHealth && boss.health > 0)
	{
		DrawHealthBar(boss.dstRect, boss.health, boss.maxHealth);
	}
	else if (boss.health <= 0)
	{
		boss.health = 0;
		DrawHealthBar(boss.dstRect, boss.health, boss.maxHealth);
	}
}
void DrawBoss(const Room& room, Boss& boss,const Texture voiceLines[])
{
	if (room.id != RoomID::bossRoom) return;
	DrawVoiceLine(boss, voiceLines);
	DrawBossHealth(boss);
	DrawTexture(boss.sprite.texture, boss.dstRect, boss.srcRect);
}
void ChargeAtPlayer(Boss& boss, float elapsedSec)
{
	const float minDistance{ 5.f };

	if ( BossDistanceToChargePoint(boss) > minDistance)
	{
		boss.dstRect.left	-= (boss.speed * cosf(boss.bossPlayerAngle)) * elapsedSec;
		boss.dstRect.bottom -= (boss.speed * sinf(boss.bossPlayerAngle)) * elapsedSec;
	}
	else
	{
		boss.AIState = BossAIStates::idle;
		boss.bossAIStateTimer = 0;
	}
}
void UpdateBossAIState(Boss& boss, float elapsedSec)
{
	if (g_CurrentRoom.id != RoomID::bossRoom) return;

	const int randInt			{1 + rand() % 100 };
	const int regenChance					 { 10 };
	const int spawnMinionsChance			 { 20 };
	const int chargeChance					 { 70 };
	const float thinkingTime				 { 1.f};

	IsBossDead(boss);
	BossAttackPlayer(boss, g_Player, elapsedSec);
	boss.bossAIStateTimer += elapsedSec;

	switch (boss.AIState)
	{
		case BossAIStates::idle:
		{
			if (boss.bossAIStateTimer > thinkingTime)
			{
				boss.isTalking = true;
				if (randInt > 0 && randInt <= regenChance)
				{
					if (boss.health / boss.maxHealth <= 0.3f)
					{
						boss.AIState = BossAIStates::regenerate;
						boss.bossAIStateTimer = 0;
					}
					break;
				}
				else if (randInt > regenChance && randInt <= spawnMinionsChance)
				{
					if (boss.health / boss.maxHealth <= 0.5f)
					{
					boss.AIState = BossAIStates::spawnMinions;
					boss.bossAIStateTimer = 0;
					}
					break;
				}
				else if (randInt > spawnMinionsChance && randInt <= chargeChance)
				{
					PrepareToCharge(boss);
					boss.AIState = BossAIStates::charge;
					boss.bossAIStateTimer = 0;
					break;
				}
				else
				{
					boss.bossAIStateTimer = 0;
					break;
				}
			}
			break;
		}
		case BossAIStates::charge:
		{
			ChargeAtPlayer(boss, elapsedSec);
			break;
		}
		case BossAIStates::spawnMinions:
		{
			if (!boss.isTalking)
			{
				int spawnIdx{ 15 };
				const int nrOfMinions{ g_MaxEnemiesPerRoom };
				EnemyShorthand minions[g_MaxEnemiesPerRoom]
				{{ GetRandomMinion() , spawnIdx }	   ,
				 { GetRandomMinion() , spawnIdx += 2 } ,
				 { GetRandomMinion() , spawnIdx += 2 } ,
				 { GetRandomMinion() , spawnIdx += 2 } ,
				 { GetRandomMinion() , spawnIdx += 2 }};

				SpawnEnemies(minions);
				boss.AIState = BossAIStates::idle;
			}
			break;
		}
		case BossAIStates::regenerate:
		{
			if (!boss.isTalking)
			{
				boss.health += 0.2f * boss.maxHealth;
				boss.AIState = BossAIStates::idle;
			}
			break;
		}
		case BossAIStates::death:
		{

			break;
		}
	}

}
void PrepareToCharge(Boss& boss)
{
	boss.delta.x = boss.dstRect.left - g_Player.dstRect.left;
	boss.delta.y = boss.dstRect.bottom - g_Player.dstRect.bottom;
	boss.chargeEndPoint.x = g_Player.dstRect.left;
	boss.chargeEndPoint.y = g_Player.dstRect.bottom;
	boss.bossPlayerAngle = GetAngle(boss);
}
void UpdateBossAnimState(const Room& room, Boss& boss, float elapsedSec)
{
	if (room.id != RoomID::bossRoom) return;
	BossLookAtPlayer(g_Boss, g_Player);
	switch (boss.animState)
	{
		case AnimStates::idleRight:
		{
			boss.sprite.texture = FetchTexture("boss_anim_idle_right");
			boss.sprite.frameTime = 1 / 6.0f;
			boss.sprite.accumulatedTime += elapsedSec;
			if (boss.sprite.accumulatedTime > boss.sprite.frameTime)
			{
				++boss.sprite.currentFrame %= boss.sprite.frames;
				boss.sprite.accumulatedTime -= boss.sprite.frameTime;
			}	
			boss.srcRect.left = boss.sprite.currentFrame * boss.srcRect.width;
			break;
		}
		case AnimStates::idleLeft:
		{
			boss.sprite.texture = FetchTexture("boss_anim_idle_left");
			boss.sprite.frameTime = 1 / 6.0f;
			boss.sprite.accumulatedTime += elapsedSec;
			if (boss.sprite.accumulatedTime > boss.sprite.frameTime)
			{
				++boss.sprite.currentFrame %= boss.sprite.frames;
				boss.sprite.accumulatedTime -= boss.sprite.frameTime;
			}
			boss.srcRect.left = boss.sprite.currentFrame * boss.srcRect.width;
			break;
		}
		case AnimStates::runRight:
		{
			boss.sprite.texture = FetchTexture("boss_anim_run_right");
			boss.sprite.frameTime = 1 / 20.0f;
			boss.sprite.accumulatedTime += elapsedSec;
			if (boss.sprite.accumulatedTime > boss.sprite.frameTime)
			{
				++boss.sprite.currentFrame %= boss.sprite.frames;
				boss.sprite.accumulatedTime -= boss.sprite.frameTime;
			}
			boss.srcRect.left = boss.sprite.currentFrame * boss.srcRect.width;
			break;
		}
		case AnimStates::runLeft:
		{
			boss.sprite.texture = FetchTexture("boss_anim_run_left");
			boss.sprite.frameTime = 1 / 20.0f;
			boss.sprite.accumulatedTime += elapsedSec;
			if (boss.sprite.accumulatedTime > boss.sprite.frameTime)
			{
				++boss.sprite.currentFrame %= boss.sprite.frames;
				boss.sprite.accumulatedTime -= boss.sprite.frameTime;
			}
			boss.srcRect.left = boss.sprite.currentFrame * boss.srcRect.width;
			break;
		}

		case AnimStates::death:
		{
			boss.sprite.texture = FetchTexture("boss_anim_death");
			boss.sprite.cols = 1;
			boss.sprite.frames = 1;
			boss.sprite.currentFrame = 0;
			boss.sprite.accumulatedTime = 0.0f;
			boss.sprite.frameTime = 1 / 8.0f;
			boss.srcRect.width = FetchTexture("boss_anim_death").width;
			boss.srcRect.height = FetchTexture("boss_anim_death").height;

			break;

		}

	}
}
void BossAttackPlayer(Boss& boss, Player& player, float elapsedSec)
{
	boss.bossAttackPlayerTimer += elapsedSec;
	float hitRate{ 0.5f };

	if (IsOverlapping(boss.dstRect, player.dstRect) && boss.bossAttackPlayerTimer > hitRate)
	{
		player.health -= boss.damageOutput;
		player.isPlayerHit = true; // new line
		boss.bossAttackPlayerTimer = 0;
	}
}
void BossLookAtPlayer(Boss& boss, Player& player)
{
	if (boss.dstRect.left < player.dstRect.left && boss.AIState == BossAIStates::idle)
	{
		boss.animState = AnimStates::idleRight;
	}
	else if (boss.dstRect.left > player.dstRect.left && boss.AIState == BossAIStates::idle)
	{
		boss.animState = AnimStates::idleLeft;
	}
	else if (boss.dstRect.left < player.dstRect.left && boss.AIState == BossAIStates::charge)
	{
		boss.animState = AnimStates::runRight;
	}
	else if (boss.dstRect.left > player.dstRect.left && boss.AIState == BossAIStates::charge)
	{
		boss.animState = AnimStates::runLeft;
	}


}

void InitTotems(Sprite& totemSprite)
{
	totemSprite.texture = FetchTexture("lava_totem_anim");
	totemSprite.cols = 3;
	totemSprite.frames = 3;
	totemSprite.currentFrame = 0;
	totemSprite.accumulatedTime = 0.0f;
	totemSprite.frameTime = 1 / 5.0f;
}
void DrawTotems(Sprite& totemSprite, const Room& room)
{
	if (room.id != RoomID::bossRoom) return;
	Rectf srcRect{};
	srcRect.width = totemSprite.texture.width / totemSprite.cols;
	srcRect.height = totemSprite.texture.height;
	srcRect.left = totemSprite.currentFrame * srcRect.width;
	srcRect.bottom = srcRect.height;

	for (int i = 2; i <= 10; i+=2) DrawTexture(totemSprite.texture, g_CellArr[i].dstRect, srcRect);
}
void UpdateTotems(Sprite& totemSprite, const Room& room, float elapsedSec)
{
	if (room.id != RoomID::bossRoom) return;

	totemSprite.accumulatedTime += elapsedSec;
	if (totemSprite.accumulatedTime > totemSprite.frameTime)
	{
		++totemSprite.currentFrame %= totemSprite.frames;
		totemSprite.accumulatedTime -= totemSprite.frameTime;
	}


}

std::string GetRandomMinion()
{
	const int nrOfMinions			{ 5 };
	const int randIdx		{ rand() % nrOfMinions };

	std::string minionsArr[nrOfMinions]{"", "rusher", "gnawer", "necrospawn"};

	return minionsArr[randIdx];
}

void InitVoiceLines(Texture voiceLinesArr[])
{
	int idx{};
	TextureFromString("Hmmm...", "Resources/Font.ttf", 20, g_White, voiceLinesArr[idx]);
	TextureFromString("Minions! Come to my aid!", "Resources/Font.ttf", 20, g_White, voiceLinesArr[++idx]);
	TextureFromString("'Tis but a scratch!", "Resources/Font.ttf", 20, g_White, voiceLinesArr[++idx]);
	TextureFromString("Dodge this!", "Resources/Font.ttf", 20, g_White, voiceLinesArr[++idx]);
}
void DrawVoiceLine(const Boss& boss,const Texture voiceLinesArr[])
{
	Point2f textBox{};
	const float shift{ 10.f };
	textBox.x = boss.dstRect.left + shift;
	textBox.y = boss.dstRect.bottom + boss.dstRect.height + shift;
	if (boss.isTalking)
	{
	if (boss.AIState == BossAIStates::idle) DrawTexture(voiceLinesArr[0], textBox);
	if (boss.AIState == BossAIStates::spawnMinions) DrawTexture(voiceLinesArr[1], textBox);
	if (boss.AIState == BossAIStates::regenerate) DrawTexture(voiceLinesArr[2], textBox);
	if (boss.AIState == BossAIStates::charge) DrawTexture(voiceLinesArr[3], textBox);
	}
}
void UpdateVoiceLine(Boss& boss, const Room& room, float elapsedSec)
{
	if (room.id != RoomID::bossRoom) return;
	const float voiceLineDuration{ 1.f };

	if (boss.isTalking) boss.bossVoiceLineTimer += elapsedSec;
	if (boss.bossVoiceLineTimer > voiceLineDuration)
	{
		boss.isTalking = false;
		boss.bossVoiceLineTimer = 0;
	}
}
#pragma endregion bossHandling

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
	startingRoom.id = RoomID::startingRoom;
	startingRoom.layoutToLoad = "starting_room.room";
	startingRoom.topDoorDestination = RoomID::verticalHallway1;
	startingRoom.enemyShorthand[0] = { "bat", GetIndex(1, 1) };
	startingRoom.enemyShorthand[1] = { "bat", GetIndex(1, 11) };
	startingRoom.interactableShort[0] = { "basic_sword", 70, InteractableType::weaponDrop };
	startingRoom.interactableShort[1] = { "health_potion", 72, InteractableType::itemDrop };

	Room& verticalHallway1 = level[1];
	verticalHallway1.id = RoomID::verticalHallway1;
	verticalHallway1.layoutToLoad = "vertical_hallway_1.room";
	verticalHallway1.bottomDoorDestination = RoomID::startingRoom;
	verticalHallway1.topDoorDestination = RoomID::combatRoom1;
	verticalHallway1.enemyShorthand[0] = { "zombie", GetIndex(1, 6) };

	Room& combatRoom1 = level[2];
	combatRoom1.id = RoomID::combatRoom1;
	combatRoom1.layoutToLoad = "combat_room_1.room";
	combatRoom1.bottomDoorDestination = RoomID::verticalHallway1;
	combatRoom1.topDoorDestination = RoomID::verticalHallway2;
	combatRoom1.leftDoorDestination = RoomID::horizontalHallway3;
	combatRoom1.rightDoorDestination = RoomID::horizontalHallway1;
	combatRoom1.enemyShorthand[0] = { "zombie", GetIndex(4, 6) };
	combatRoom1.enemyShorthand[1] = { "bat", GetIndex(2, 2) };
	combatRoom1.enemyShorthand[2] = { "bat", GetIndex(2, 10) };
	combatRoom1.enemyShorthand[3] = { "bat", GetIndex(6, 2) };
	combatRoom1.enemyShorthand[4] = { "bat", GetIndex(6, 10) };

	Room& verticalHallway2 = level[3];
	verticalHallway2.id = RoomID::verticalHallway2;
	verticalHallway2.layoutToLoad = "vertical_hallway_2.room";
	verticalHallway2.bottomDoorDestination = RoomID::combatRoom1;
	verticalHallway2.topDoorDestination = RoomID::pickupRoom2;
	verticalHallway2.enemyShorthand[0] = { "bat", GetIndex(2, 4) };
	verticalHallway2.enemyShorthand[1] = { "bat", GetIndex(4, 4) };
	verticalHallway2.enemyShorthand[2] = { "bat", GetIndex(6, 4) };
	verticalHallway2.enemyShorthand[3] = { "bat", GetIndex(2, 8) };
	verticalHallway2.enemyShorthand[4] = { "bat", GetIndex(4, 8) };
	verticalHallway2.enemyShorthand[5] = { "bat", GetIndex(6, 8) };

	Room& pickupRoom2 = level[4];
	pickupRoom2.id = RoomID::pickupRoom2;
	pickupRoom2.layoutToLoad = "pickup_room_2.room";
	pickupRoom2.bottomDoorDestination = RoomID::verticalHallway2;
	pickupRoom2.interactableShort[0] = { "strength_potion", GetIndex(6,4), InteractableType::itemDrop };
	pickupRoom2.interactableShort[1] = { "regen_potion", GetIndex(2,6), InteractableType::itemDrop };
	pickupRoom2.interactableShort[2] = { "speed_potion", GetIndex(6,8), InteractableType::itemDrop };

	Room& horizontalHallway1 = level[5];
	horizontalHallway1.id = RoomID::horizontalHallway1;
	horizontalHallway1.layoutToLoad = "horizontal_hallway_1.room";
	horizontalHallway1.leftDoorDestination = RoomID::combatRoom1;
	horizontalHallway1.rightDoorDestination = RoomID::combatRoom2;
	horizontalHallway1.enemyShorthand[0] = { "bat", GetIndex(3, 4) };
	horizontalHallway1.enemyShorthand[1] = { "bat", GetIndex(3, 7) };

	Room& combatRoom2 = level[6];
	combatRoom2.id = RoomID::combatRoom2;
	combatRoom2.layoutToLoad = "combat_room_2.room";
	combatRoom2.leftDoorDestination = RoomID::horizontalHallway1;
	combatRoom2.rightDoorDestination = RoomID::horizontalHallway2;
	combatRoom2.enemyShorthand[0] = { "zombie", GetIndex(2,6) };
	combatRoom2.enemyShorthand[1] = { "zombie", GetIndex(4,8) };
	combatRoom2.enemyShorthand[2] = { "zombie", GetIndex(6,6) };

	Room& horizontalHallway2 = level[7];
	horizontalHallway2.id = RoomID::horizontalHallway2;
	horizontalHallway2.layoutToLoad = "horizontal_hallway_2.room";
	horizontalHallway2.leftDoorDestination = RoomID::combatRoom2;
	horizontalHallway2.rightDoorDestination = RoomID::pickupRoom1;

	Room& pickupRoom1 = level[8];
	pickupRoom1.id = RoomID::pickupRoom1;
	pickupRoom1.layoutToLoad = "pickup_room_1.room";
	pickupRoom1.leftDoorDestination = RoomID::horizontalHallway2;
	pickupRoom1.enemyShorthand[0] = { "archer", 46 };
	pickupRoom1.enemyShorthand[1] = { "bat", GetIndex(2, 5) };
	pickupRoom1.enemyShorthand[2] = { "bat", GetIndex(6, 5) };
	pickupRoom1.interactableShort[0] = { "bow", 58, InteractableType::weaponDrop };

	Room& horizontalHallway3 = level[9];
	horizontalHallway3.id = RoomID::horizontalHallway3;
	horizontalHallway3.layoutToLoad = "horizontal_hallway_3.room";
	horizontalHallway3.leftDoorDestination = RoomID::combatRoom3;
	horizontalHallway3.rightDoorDestination = RoomID::combatRoom1;
	horizontalHallway3.enemyShorthand[0] = { "archer", GetIndex(4, 2) };
	horizontalHallway3.enemyShorthand[1] = { "zombie", GetIndex(3, 5) };
	horizontalHallway3.enemyShorthand[2] = { "zombie", GetIndex(5, 5) };


	Room& combatRoom3 = level[10];
	combatRoom3.id = RoomID::combatRoom3;
	combatRoom3.layoutToLoad = "combat_room_3.room";
	combatRoom3.rightDoorDestination = RoomID::horizontalHallway3;
	combatRoom3.bottomDoorDestination = RoomID::verticalHallway3;
	combatRoom3.leftDoorDestination = RoomID::horizontalHallway4;
	combatRoom3.enemyShorthand[0] = { "summoner", GetIndex(2, 5) };
	combatRoom3.enemyShorthand[1] = { "necromancer", GetIndex(6, 5) };
	combatRoom3.enemyShorthand[2] = { "zombie", GetIndex(4, 7) };

	Room& verticalHallway3 = level[11];
	verticalHallway3.id = RoomID::verticalHallway3;
	verticalHallway3.layoutToLoad = "vertical_hallway_3.room";
	verticalHallway3.topDoorDestination = RoomID::combatRoom3;
	verticalHallway3.bottomDoorDestination = RoomID::pickupRoom3;
	verticalHallway3.enemyShorthand[0] = { "zombie", GetIndex(5, 4) };
	verticalHallway3.enemyShorthand[1] = { "zombie", GetIndex(5, 6) };
	verticalHallway3.enemyShorthand[2] = { "zombie", GetIndex(5, 8) };
	verticalHallway3.enemyShorthand[3] = { "necromancer", GetIndex(7, 6) };

	Room& pickupRoom3 = level[12];
	pickupRoom3.id = RoomID::pickupRoom3;
	pickupRoom3.layoutToLoad = "pickup_room_3.room";
	pickupRoom3.topDoorDestination = RoomID::verticalHallway3;
	pickupRoom3.interactableShort[0] = { "health_potion", GetIndex(2,4), InteractableType::itemDrop };
	pickupRoom3.interactableShort[1] = { "shield_potion", GetIndex(2,8), InteractableType::itemDrop };
	pickupRoom3.interactableShort[2] = { "giant_blade", GetIndex(6,6), InteractableType::weaponDrop };

	Room& horizontalHallway4 = level[13];
	horizontalHallway4.id = RoomID::horizontalHallway4;
	horizontalHallway4.layoutToLoad = "horizontal_hallway_4.room";
	horizontalHallway4.rightDoorDestination = RoomID::combatRoom3;
	horizontalHallway4.leftDoorDestination = RoomID::bossRoom;
	horizontalHallway4.enemyShorthand[0] = { "rusher", GetIndex(3,4) };
	horizontalHallway4.enemyShorthand[1] = { "rusher", GetIndex(5,4) };
	horizontalHallway4.enemyShorthand[2] = { "rusher", GetIndex(3,6) };
	horizontalHallway4.enemyShorthand[3] = { "rusher", GetIndex(5,6) };
	horizontalHallway4.enemyShorthand[4] = { "rusher", GetIndex(3,5) };
	horizontalHallway4.enemyShorthand[5] = { "rusher", GetIndex(5,5) };
	horizontalHallway4.enemyShorthand[6] = { "rusher", GetIndex(3,3) };
	horizontalHallway4.enemyShorthand[7] = { "rusher", GetIndex(5,3) };

	Room& bossRoom = level[14];
	bossRoom.id = RoomID::bossRoom;
	bossRoom.layoutToLoad = "boss_room.room";
	bossRoom.rightDoorDestination = RoomID::horizontalHallway4;

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
Room FetchRoom(const RoomID& id)
{
	const Room nullRoom{};
	for (int i{0}; i < g_NrRoomsPerLevel; ++i)
	{
		if (g_Level[i].id == id)
		{
			return g_Level[i];
		}
	}
	std::cout << "Room with id '" << int(id) << "' not found\n";
	return nullRoom;
}
void LoadRoom(const Room& roomToLoad)
{
	// This function will allow us to always retrieve the enemy array etc from any room we're loading by adding said code below
	ClearInteractables();
	ClearEnemies();
	ClearProjectiles();
	LoadRoomLayout(g_CellArr, roomToLoad.layoutToLoad);
	SetObstacles(g_CellArr, g_NrRows, g_NrCols);
	SpawnInteractablesInRoom(roomToLoad);
	if (!roomToLoad.isCleared) {
		SpawnEnemies(roomToLoad.enemyShorthand);
	}
	if (roomToLoad.id == RoomID::bossRoom)
	{
		InitBoss(g_Boss);
	}
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
bool CheckRoomCleared(Room& currentRoom) 
{
	if (currentRoom.isCleared == false)
	{
		for (int i{}; i < g_MaxEnemiesPerRoom; ++i)
		{
			if (g_EnemyArr[i].health > 0.01f)
			{
				return false;
			}
		}
	}
	return true;
}
void SetRoomCleared(Room& currentRoom)
{
	for (int i{}; i < g_NrRoomsPerLevel; ++i)
	{
		if (g_Level[i].id == currentRoom.id)
		{
			g_Level[i].isCleared = true;
		}
	}
}
#pragma endregion roomHandling 
#pragma endregion ownDefinitions
