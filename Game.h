#pragma once
using namespace utils;
#pragma region gameInformation
// Set your name and group in the title here
std::string g_WindowTitle{ "DungeonGame - Aerts,Tanguy / Hertoghe, Rutger - 1DAE10/11" };

// Change the window dimensions here
float g_WindowWidth{ 1280 };
float g_WindowHeight{ 720 };
#pragma endregion gameInformation



#pragma region ownDeclarations
// Declare your own global variables here
const int g_NrRows{ 9 };
const int g_NrCols{ 13 };
const int g_GridSize{ g_NrRows * g_NrCols };
const int g_TexturesSize{ 100 }; // Careful, we do not know yet how many textures we'll need. ADDED WALL
const int g_EnemyArrSize{ 12 };
const int g_NrRoomsPerLevel{ 10 }; // Careful with this aswell
const int g_RoomArrSize{ 100 }; 
const int g_LevelArrSize{ 1 }; // lets try to make one level first, each level always has g_NrRoomsPerLevel rooms
const int g_ItemInventorySize{ 5 };
const int g_WeaponInventorySize{ 3 };
const int g_WeaponsInGame{ 10 };

enum class EnemyType
{
	basic,
	strong,
	ranged,
	boss
};

enum class WeaponType 
{
	sword
};

enum class ItemType
{
	potion
};

enum class Direction
{
	up,
	down,
	left,
	right,
	none
};

struct Cell
{
	Rectf dstRect;
	Texture texture;
	bool isObstacle{false};
};
struct Item
{
	std::string name;
	ItemType type;
};
struct Weapon
{
	std::string name;
	Texture texture{};
	WeaponType type{};
	float damageOutput{};
};
struct Player
{
	Rectf dstRect;
	Rectf animationPos;
	Texture texture;
	float speedModifier{ 5.0f };
	float timeTracker{ 0.0f };

	Direction facing;

	float health{};
	float maxHealth{20.f};

	Item itemInventory[g_ItemInventorySize];
	int selectedItem;

	Weapon weaponInventory[g_WeaponInventorySize];
	int selectedWeapon;
};
struct Enemy
{
	EnemyType type;
	Rectf dstRect;
	Rectf animationPos;
	Texture texture;
	float timeTracker{ 0.f };
	float timePerAction;
	float health;
	float maxHealth;
	float damageOutput;
	int viewRange;
};
struct NamedTexture
{
	std::string name;
	Texture texture;
	bool isObstacle{ false };
};
struct Room
{
	Cell cells[g_GridSize]{};
	std::string roomName;
};
struct Level
{
	Room Rooms[g_NrRoomsPerLevel]{};
	std::string levelName;
};


Room g_Rooms[g_RoomArrSize]{};
Level g_Levels[g_LevelArrSize]{};
Weapon g_Weapons[g_WeaponsInGame];
Player g_Player{};
Enemy g_EnemyArr[g_EnemyArrSize]{};
Cell g_CellArr[g_GridSize]{};
Texture g_Numbers[g_GridSize]{};
NamedTexture g_NamedTexturesArr[g_TexturesSize]{};


const Color4f   g_Green{ 0 / 255.f, 236 / 255.f, 0 / 255.f, 255 / 255.f },
				g_GreenTransparent{ 0 / 255.f, 236 / 255.f, 0 / 255.f, 100 / 255.f },
				g_Blue{ 0 / 255.f, 0 / 255.f, 241 / 255.f , 255 / 255.f },
				g_Yellow{ 253 / 255.f, 253 / 255.f, 152 / 255.f , 255 / 255.f },
				g_Red{ 255 / 255.f, 0 / 255.f, 0 / 255.f , 255 / 255.f },
				g_DarkRed{ 155 / 255.f, 0 / 255.f, 0 / 255.f , 255 / 255.f },
				g_RedTransparent{ 255 / 255.f, 0 / 255.f, 0 / 255.f , 100 / 255.f },
				g_Gold{ 176 / 255.f, 149 / 255.f, 58 / 255.f , 255 / 255.f },
				g_Pink{ 252 / 255.f, 128 / 255.f, 125 / 255.f , 255 / 255.f },
				g_Cyan{ 0 / 255.f, 255 / 255.f, 255 / 255.f , 255 / 255.f },
				g_Magenta{ 255 / 255.f, 0 / 255.f, 255 / 255.f , 255 / 255.f },
				g_Grey{ 204 / 255.f, 204 / 255.f, 204 / 255.f, 255 / 255.f },
				g_Black{ 0,0,0,1 },
				g_White{ 1,1,1,1 };

// Declare your own functions here

// Utils
int GetIndex(const int rowIdx, const int colIdx, const int nrCols); 
int GetPlayerGridIndex(const Player& player, Cell cellArr[], const int arrSize);
Point2f GetPlayerRowColumn(Player& player, Cell cellArr[], int nrRows, int nrCols); 
float CalculateAngleToMouse(Point2f playerCenter, Point2f mousePos);
bool HasEnemy(const int cellIndex, Enemy enemyArr[], int enemyArrSize);
void SetPlayerPos(Player& player, Cell cellArr[], int dstIndex);

// Texture Handling
void InitTextures(NamedTexture namedTextureArr[], const int arrSize, Texture textureNumbersArr[], const int numbersArrSize);
void LoadTexture(const std::string texturePath, NamedTexture& namedTexture, std::string name, bool isObstacle);
void LoadTexturesFromFolder(std::string folderPath, NamedTexture namedTextureArr[], const int arrSize);
void DeleteTextures();
Texture FetchTexture(std::string textureName);
std::string FetchTextureName(Texture texture);

// Grid Handling
void InitGrid(Cell cellArr[], int nrRows, int nrCols);
void DrawDebugGrid(Cell cellArr[], int nrRows, int nrCols);
void DrawGridTextures(Cell cellArr[], int nrRows, int nrCols);
void SetObstacles(Cell cellArr[], int nrRows, int nrCols); //sets all appropriate cells to 'isObstacle = true' for the appropriate textures

// Player Handling
void InitPlayer(Player& player, Cell cellArr[]);
void DrawPlayer(const Player& player);
void UpdateAnimationPos(float elapsedSec, Player& player);

void DrawWeaponInventory(const Player& player);
void DrawHealthBar(Rectf entityRect, float health, float maxHealth);
void DrawPlayerHealth(const Player& player);

void CycleWeapons(Player& player);
void UseWeapon(const Player& player);
void UseSword(const Player& player);
void AttackOnTiles(const Player& player, int indicesToScan[], int indicesAmount);

// Weapons Handling
void InitWeapons();
Weapon InitializeWeapon(const std::string& weaponName, const std::string& textureName, const WeaponType& type, float damage);
Weapon FetchWeapon(const std::string& name);

// Enemy Handling
int GetRandomSpawn(Cell cellArr[], const int cellArrSize);
void DrawEnemies(Enemy enemyArr[], int arrSize);
void UpdateAnimationPos(float elapsedSec, Enemy& enemy);
// Enemy Initialization Handling
void InitEnemies(Enemy enemyArr[], const int enemyArrSize, Cell cellArr[], const int cellArrSize);
Enemy InitializeEnemy(std::string enemyName);
Enemy InitializeEnemy(EnemyType type, std::string textureName, float maxHealth, float damage, float timePerAction, int viewRange);

int GetEnemyGridIndex(Enemy& enemy, Cell cellArr[], const int arrSize);

void DrawEnemyHealth(const Enemy& enemy);
void DrawEnemyHealthBars(Enemy enemyArr[]);
void DamageAllEnemies(Enemy EnemyArr[], const int enemyArrSize);

// Enemy AI Handling
void BasicEnemyAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize);
void RangedEnemyAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize);
void UpdateEnemies(float elapsedSec, Enemy enemyArr[], int enemyArrSize, Cell cellArr[], int cellArrSize);

// Input Handling
void ProcessMovement(Player& player, Cell cellArr[], const int arrSize, float elapsedSec);
void SwitchPlayer(Player& player);
void ProcessFacing(Player& player, const SDL_MouseMotionEvent& e);

// Level writing/reading handling
void SaveRoomLayout(Cell cellArr[], int cellArrSize, const std::string& saveFileName);
void LoadRoomLayout(Cell targetCellArr[], const std::string& fileName);

// Room Handling
void UpdateRoom(Player& player, Cell cellArr[], const int cellArrSize);
void InitRooms(Room rooms[]);
Room FetchRoom(std::string roomName);

// Level Handling
void InitLevels(Level levels[], Room rooms[]);


#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
