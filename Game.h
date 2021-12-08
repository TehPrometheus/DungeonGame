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

const int g_NrRows				{ 9 };
const int g_NrCols				{ 13 };
const int g_GridSize			{ g_NrRows * g_NrCols };
const int g_TexturesSize		{ 100 }; // Careful, we do not know yet how many textures we'll need. ADDED WALL
const int g_EnemyArrSize		{ 12 };
const int g_MaxEnemiesPerRoom   { 10 };
const int g_MaxInteractablesRoom{ 10 };
const int g_NrRoomsPerLevel		{ 15 }; // Careful with this aswell
const int g_RoomArrSize			{ 10 }; 
const int g_ItemInventorySize	{ 5 };
const int g_ItemsInGame			{ 10 };
const int g_WeaponInventorySize	{ 3 };
const int g_WeaponsInGame		{ 10 };
const int g_InteractablesInGame	{ g_WeaponsInGame + g_ItemsInGame };
const int g_PlayerSpritesSize	{ 20 };
const int g_MaxProjectiles      { 20 };

// enums
enum class GameStates
{
	startScreen,
	playing,
	gameOverScreen,
	restarting
};

enum class EnemyType
{
	basic,
	ranged,
	destructible,
	boss
};
enum class WeaponType
{
	none,
	sword,
	bow
};
enum class ItemType
{
	potion
};
enum class InteractableType
{
	weaponDrop,
	itemDrop,
	container
};
enum class Direction
{
	up,
	down,
	left,
	right,
	none
};
enum class AnimStates
{
	idleRight,
	idleLeft,
	idleUp,
	idleDown,
	runRight,
	runLeft,
	runUp,
	runDown, 
	hit
};
enum class RoomID
{
	startingRoom,
	verticalHallway1,
	verticalHallway2,
	verticalHallway3,
	horizontalHallway1,
	horizontalHallway2,
	horizontalHallway3,
	horizontalHallway4,
	combatRoom1,
	combatRoom2,
	combatRoom3,
	pickupRoom1,
	pickupRoom2,
	pickupRoom3,
	bossRoom
};
enum class BossAIStates
{
	idle,
	basicAttack,
	charge
};
enum class EffectType
{
	none,
	health,
	regeneration,
	speedBoost,
	strengthBoost,
	shielding
};
// structs
struct Cell
{
	Rectf dstRect;
	Texture texture;
	bool isObstacle{false};
};
struct StatusEffect
{
	EffectType effectType;
	float duration;
	float timeRemaining;
	float modifier;
	int stacks{ 0 };
};
struct Item
{
	std::string name;
	Texture texture;
	ItemType type;
	StatusEffect effect;
	int count;
};
struct Weapon
{
	std::string name;
	Texture texture;
	WeaponType type{};
	float damageOutput{};
};
struct Projectile // WORK IN PROGRESS
{
	std::string type;
	float speed;
	Rectf location;
	Texture texture;
	float direction;
	float damage;
};
struct Interactable
{
	std::string name;
	InteractableType type;
	Weapon linkedWeapon;
	Item linkedItem;
	Rectf dstRect;
	int location;
	bool pickedUp{ true };
};
struct InteractableShorthand
{
	std::string name;
	int location{};
	InteractableType type;
	bool pickedUp{ false };
};
struct Sprite
{
	Texture texture;
	int frames;
	int cols;
	float frameTime;
	int currentFrame;
	float accumulatedTime;
};
struct Player
{
	Rectf dstRect;
	Rectf animationPos;
	Sprite sprite;
	float speedModifier{ 5.0f };
	float timeTracker{ 0.0f };

	AnimStates animState;
	Direction facing;

	float health{};
	float maxHealth{20.f};

	Item itemInventory[g_ItemInventorySize];
	int selectedItem;

	Weapon weaponInventory[g_WeaponInventorySize];
	int selectedWeapon;

	StatusEffect effects[g_ItemInventorySize];
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
struct Boss
{
	Rectf dstRect;
	Rectf srcRect;
	Sprite sprite;
	BossAIStates AIState;
	AnimStates animState;
	Point2f delta;
	Point2f chargeEndPoint;
	Point2f chargeStartPoint;
	float speed;
	float health;
	float maxHealth;
	float damageOutput;
	float timeTracker;
	int viewRange;
};
struct EnemyShorthand
{
	std::string type;
	int location{};
};
struct NamedTexture
{
	std::string name;
	Texture texture;
	bool isObstacle{ false };
};
struct Room
{
	RoomID id{};
	std::string layoutToLoad{};
	RoomID topDoorDestination{};
	RoomID leftDoorDestination{};
	RoomID bottomDoorDestination{};
	RoomID rightDoorDestination{};
	EnemyShorthand enemyShorthand[g_MaxEnemiesPerRoom];
	InteractableShorthand interactableShort[g_MaxInteractablesRoom];
	bool isCleared{ false };
};
struct Level
{
	Room Rooms[g_NrRoomsPerLevel]{};
};



Room g_Level[15];
Room g_CurrentRoom{};
Boss g_Boss{};
GameStates g_Game{ GameStates::playing };
Weapon g_Weapons[g_WeaponsInGame]{};
Item g_Items[g_ItemsInGame]{};
Interactable g_Interactables[g_InteractablesInGame]{};
Enemy g_EnemyArr[g_EnemyArrSize]{};
Player g_Player{};
Cell g_CellArr[g_GridSize]{};
Texture g_Numbers[g_GridSize]{};
NamedTexture g_NamedTexturesArr[g_TexturesSize]{};
Sprite g_PlayerSprites[g_PlayerSpritesSize]{};
Projectile g_Projectiles[g_MaxProjectiles]{};


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
				g_White{ 1,1,1,1 },
				g_WhiteTransparent{1.f, 1.f, 1.f, 0.1f};

// Declare your own functions here

// Utils
void PlayerDebugInfo();
int GetIndex(const int rowIdx, const int colIdx, const int nrCols = g_NrCols); 
int GetPlayerGridIndex(const Player& player, Cell cellArr[], const int arrSize);
Point2f GetPlayerRowColumn(Player& player, Cell cellArr[], int nrRows, int nrCols); 
float CalculateAngleToMouse(Point2f playerCenter, Point2f mousePos);
bool HasEnemy(const int cellIndex, Enemy enemyArr[], int enemyArrSize);
void SetPlayerPos(Player& player, Cell cellArr[], int dstIndex);
void TeleportPlayer(const int index, Player& player);
bool IsPointInRect(const Rectf& rectangle, const Point2f& point);
int GetTilePlayerFacing();
bool HasInteractable(const int cellIndex, Interactable interactableArr[], int arrSize);

// Texture Handling
void InitTextures(NamedTexture namedTextureArr[], const int arrSize, Texture textureNumbersArr[], const int numbersArrSize);
void LoadTexture(const std::string texturePath, NamedTexture& namedTexture, std::string name, bool isObstacle);
void LoadTexturesFromFolder(std::string folderPath, NamedTexture namedTextureArr[], const int arrSize);
void DeleteTextures();
Texture FetchTexture(const std::string& textureName);
std::string FetchTextureName(const Texture& texture);

// Grid Handling
void InitGrid(Cell cellArr[], int nrRows, int nrCols);
void DrawDebugGrid(Cell cellArr[], int nrRows, int nrCols);
void DrawGridTextures(Cell cellArr[], int nrRows, int nrCols);
void SetObstacles(Cell cellArr[], int nrRows, int nrCols); //sets all appropriate cells to 'isObstacle = true' for the appropriate textures

// Player Handling
void InitPlayer(Player& player, Cell cellArr[], Sprite Sprites[]);
void DrawPlayer(const Player& player, Sprite Sprites[]);
void UpdateAnimationPos(float elapsedSec, Player& player);

void DrawItemInventory(const Player& player);
void DrawWeaponInventory(const Player& player);

void DrawHealthBar(Rectf entityRect, float health, float maxHealth);
void DrawPlayerHealth(const Player& player);

void CycleWeapons(Player& player);

void UseItem(Player& player, int itemslot);

void UseWeapon(const Player& player);
void UseBow(const Player& player);
void UseSword(const Player& player);
void AttackOnTiles(const Player& player, int indicesToScan[], int indicesAmount);

void DrawReach(const Player& player);
void DrawSwordReach(const Player& player);
void DrawBowReach(const Player& player);

void Interact(Player& player, Cell cellArr[], const int cellArrSize, Room& currentRoom);
void PickUpInteractable(int index, int location);

// Item Handling
void InitItems();
Item InitializeItem(const std::string& itemName, const std::string& textureName, const ItemType& type, const EffectType effect, const float duration = 0.01f, const float modifier = 1.0f);
void UpdateStatusEffects(float elapsedSec);
void DrawStatusEffects(Player& player);
void RollForDrop(Enemy& enemy);

// Weapons Handling
void InitWeapons();
Weapon InitializeWeapon(const std::string& weaponName, const std::string& textureName, const WeaponType& type, float damage);
Weapon FetchWeapon(const std::string& name);

// Projectile Handling
void CreateProjectile(Rectf location, std::string type, float direction, float speed, float damage);
Projectile InitializeProjectile(std::string type, float speed);
void DrawProjectiles();
void UpdateProjectiles(float elapsedSec);
void DestroyProjectile(Projectile& projectile);

// Interactable Handling
void SpawnInteractable(std::string name, int location, InteractableType type);
void SpawnInteractablesInRoom(const Room& room);
void ReplaceInteractableInRoom(const Room& room, std::string interactableToReplace, std::string interactableReplacement, int location);
void SetInteractablePickedUp(const Room& room, std::string interactableName, int location);
void AddInteractableToRoom(const Room& room, std::string interactableName, int location, InteractableType type);

Interactable InitializeInteractable(const std::string& linkedItem, const InteractableType& type);
void DrawInteractables();
void ClearInteractables();

// Enemy Handling
int GetRandomSpawn(Cell cellArr[], const int cellArrSize);
void DrawEnemies(Enemy enemyArr[], int arrSize);
void UpdateAnimationPos(float elapsedSec, Enemy& enemy);
// Enemy Initialization Handling
void InitEnemies();
Enemy InitializeEnemy(std::string enemyName);
Enemy InitializeEnemy(const EnemyType& type, const std::string& textureName, float maxHealth, float damage, float timePerAction, int viewRange);
void SpawnEnemies(const EnemyShorthand enemies[]);

int GetEnemyGridIndex(Enemy& enemy, Cell cellArr[], const int arrSize);

void DrawEnemyHealth(const Enemy& enemy);
void DrawEnemyHealthBars(Enemy enemyArr[]);
void DamageAllEnemies(Enemy EnemyArr[], const int enemyArrSize);

void ClearEnemies();
void DestroyEnemy(Enemy& enemy);

// Enemy AI Handling
void BasicEnemyAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize);
void RangedEnemyAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize);
void FireArrowFromEnemy(Cell cellArr[], const int indexDiffX, const int indexDiffY, Enemy& enemy, int enemyIndex);
void UpdateEnemies(float elapsedSec, Enemy enemyArr[], int enemyArrSize, Cell cellArr[], int cellArrSize);

// Boss Handling
void InitBoss();
void DrawBoss();
void UpdateBossAnimState(float elapsedSec);
void UpdateBossAIState(float elapsedSec);
void ChargeAtPlayer(float elapsedSec);
float BossDistanceToChargePoint();
bool IsBossOnTilesToScan(Boss boss, int tilesToScan[], int currentTile);



// Input Handling
void ProcessMovement(Player& player, Cell cellArr[], const int arrSize, Sprite Sprites[], float elapsedSec);
// void SwitchPlayer(Player& player);
void ProcessFacing(Player& player, const SDL_MouseMotionEvent& e);
void ProcessAnimState(Player& player, Sprite Sprites[]);

// Level writing/reading handling
void SaveRoomLayout(Cell cellArr[], const int cellArrSize, const std::string& saveFileName);
void LoadRoomLayout(Cell targetCellArr[], const std::string& fileName);

// Room Handling
void InitializeRooms(Room level[]);
void GoToLinkedRoom(const Room& roomOfDeparture, int playerIndex);
void LoadRoom(const Room& roomToLoad);
Room FetchRoom(const RoomID& id);
void OpenDoors(Cell cellArr[], int size);
bool CheckRoomCleared(Room& currentRoom);
void SetRoomCleared(Room& currentRoom);

// Level Handling

// Sprite Handling
void InitPlayerSprites(Sprite Sprites[]);
void UpdatePlayerSprites(Sprite Sprites[], float elapsedSec);

// Game Handling
void DrawStartScreen();
void DrawEndScreen();
void ClickStart(const SDL_MouseButtonEvent& e);
void SetGameOverScreen(Player& player);

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
