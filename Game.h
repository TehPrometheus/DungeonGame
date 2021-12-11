#pragma once
using namespace utils;
#pragma region gameInformation
// Set your name and group in the title here
std::string g_WindowTitle{ "Cave Knight - Aerts,Tanguy / Hertoghe, Rutger - 1DAE10/11" };

// Change the window dimensions here
float g_WindowWidth{ 1280 };
float g_WindowHeight{ 720 };
#pragma endregion gameInformation

#pragma region ownDeclarations

#pragma region Color Declarations
const Color4f	g_Red					{ 255 / 255.f ,   0 / 255.f ,   0 / 255.f , 255 / 255.f };
const Color4f	g_Blue					{   0 / 255.f ,   0 / 255.f , 241 / 255.f , 255 / 255.f };
const Color4f	g_Gold					{ 176 / 255.f , 149 / 255.f ,  58 / 255.f , 255 / 255.f };
const Color4f	g_Grey					{ 204 / 255.f , 204 / 255.f , 204 / 255.f , 255 / 255.f };
const Color4f   g_Green					{   0 / 255.f , 236 / 255.f ,   0 / 255.f , 255 / 255.f };
const Color4f	g_White					{ 255 / 255.f , 255 / 255.f , 255 / 255.f , 255 / 255.f };
const Color4f	g_DarkRed				{ 155 / 255.f ,   0 / 255.f ,   0 / 255.f , 255 / 255.f };
const Color4f	g_WhiteTransparent		{ 255 / 255.f , 255 / 255.f , 255 / 255.f ,  25 / 255.f };
#pragma endregion Color Declarations

#pragma region Integer Declarations
const int g_NrRows									{  9 };
const int g_NrCols									{ 13 };
const int g_GridSize						{ g_NrRows * g_NrCols };
const int g_RoomArrSize								{ 10 }; 
const int g_ItemsInGame								{ 10 };
const int g_TexturesSize							{ 100};
const int g_EnemyArrSize							{ 12 };
const int g_WeaponsInGame							{ 10 };
const int g_MaxProjectiles							{ 20 };
const int g_VoiceLinesSize							{ 10 };
const int g_NrRoomsPerLevel							{ 15 };
const int g_TotemSpritesSize						{  3 };
const int g_PlayerSpritesSize						{ 20 };
const int g_ItemInventorySize						{  5 };
const int g_MaxEnemiesPerRoom						{ 10 };
const int g_WeaponInventorySize						{  3 };
const int g_InteractablesInGame		 { g_WeaponsInGame + g_ItemsInGame };
const int g_MaxInteractablesRoom					{ 10 };
#pragma endregion Integer Declarations

#pragma region Enumeration Declarations
enum class GameStates
{
	startScreen,
	playing,
	gameOver,
	gameWon,
	restarting
};
enum class EnemyType
{
	basic,
	ranged,
	destructible,
	summoner,
	necromancer,
	boss
};
enum class WeaponType
{
	none,
	sword,
	bow,
	spear
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
	hit,
	death
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
	charge,
	spawnMinions,
	regenerate,
	death
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
#pragma endregion Enumeration Declarations

#pragma region Struct Declarations
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
struct WeaponAnimation
{
	float playTime;
	float elapsedTime;
	bool enabled{ true };
	bool isPlaying{ false };
	Rectf position;
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
	WeaponAnimation weaponAnimation;
	bool isWeaponOnCooldown{ false };
	float coolDownTimer;

	StatusEffect effects[g_ItemInventorySize];
	bool isPlayerHit{ false };
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
	float bossAIStateTimer;
	float bossAttackPlayerTimer;
	float bossVoiceLineTimer;
	float bossPlayerAngle{};
	bool isTalking;
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
#pragma endregion Struct Declarations

#pragma region Other Declarations
Cell			g_CellArr[g_GridSize]							{};
Room			g_Level[15]										{};
Room			g_CurrentRoom									{};
Item			g_Items[g_ItemsInGame]							{};
Boss			g_Boss											{};
Enemy			g_EnemyArr[g_EnemyArrSize]						{};
Sprite			g_TotemSprite									{};
Weapon			g_Weapons[g_WeaponsInGame]						{};
Player			g_Player										{};
Sprite			g_PlayerSprites[g_PlayerSpritesSize]			{};
Texture			g_Numbers[g_GridSize]							{};
Texture			g_VoiceLinesArr[g_VoiceLinesSize]				{};
Projectile		g_Projectiles[g_MaxProjectiles]					{};
GameStates		g_Game								{ GameStates::startScreen };
NamedTexture	g_NamedTexturesArr[g_TexturesSize]				{};
Interactable	g_Interactables[g_InteractablesInGame]			{};
#pragma endregion Other Declarations

#pragma region Function Declarations

#pragma region gameHandling
void InitGame();
void DrawGame();
void DrawEndScreen();
void DrawStartScreen();
void UpdateGame(float elapsedSec);
void ClickStart(const SDL_MouseButtonEvent& e);
void SetEndScreen(Boss boss, Player& player);
#pragma endregion gameHandling

#pragma region utilFunctions
float CalculateAngleToMouse(Point2f playerCenter, Point2f mousePos);

int GetTilePlayerFacing();
int GetIndex(const int rowIdx, const int colIdx, const int nrCols = g_NrCols); 
int GetPlayerGridIndex(const Player& player, Cell cellArr[], const int arrSize);

bool IsPointInRect(const Rectf& rectangle, const Point2f& point);
bool HasEnemy(const int cellIndex, Enemy enemyArr[], int enemyArrSize);
bool HasInteractable(const int cellIndex, Interactable interactableArr[], int arrSize);

void PrintGameInfo();
void PlayerDebugInfo();
void TeleportPlayer(const int index, Player& player);
void SetPlayerPos(Player& player, Cell cellArr[], int dstIndex);

Point2f GetPlayerRowColumn(Player& player, Cell cellArr[], int nrRows, int nrCols); 
#pragma endregion utilFunctions

#pragma region textureHandling
Texture FetchTexture(const std::string& textureName);

std::string FetchTextureName(const Texture& texture);

void DeleteTextures();
void LoadTexturesFromFolder(std::string folderPath, NamedTexture namedTextureArr[], const int arrSize);
void LoadTexture(const std::string texturePath, NamedTexture& namedTexture, std::string name, bool isObstacle);
void InitTextures(NamedTexture namedTextureArr[], const int arrSize, Texture textureNumbersArr[], const int numbersArrSize);

#pragma endregion textureHandling

#pragma region gridHandling
void InitGrid(Cell cellArr[], int nrRows, int nrCols);
void SetObstacles(Cell cellArr[], int nrRows, int nrCols);
void DrawDebugGrid(Cell cellArr[], int nrRows, int nrCols);
void DrawGridTextures(Cell cellArr[], int nrRows, int nrCols);
#pragma endregion gridHandling

#pragma region playerHandling
void InitPlayerAnimState(Sprite Sprites[]);
void DrawPlayerHealth(const Player& player);
void DrawItemInventory(const Player& player);
void DrawWeaponInventory(const Player& player);
void DrawPlayer(const Player& player, Sprite Sprites[]);
void UpdateAnimationPos(float elapsedSec, Player& player);
void InitPlayer(Player& player, Cell cellArr[], Sprite Sprites[]);
void UpdatePlayerAnimState(Sprite Sprites[], float elapsedSec);
void DrawHealthBar(Rectf entityRect, float health, float maxHealth);
void AttackOnTiles(const Player& player, int indicesToScan[], int indicesAmount);
#pragma endregion playerHandling

#pragma region playerInputHandling
void UseWeapon(Player& player);
void UseBow(const Player& player);
void CycleWeapons(Player& player);
void UseSword(const Player& player);
void UseSpear(const Player& player);
void DrawReach(const Player& player);
void DrawBowReach(const Player& player);
void DrawSpearReach(const Player& player);
void DrawSwordReach(const Player& player);
void UseItem(Player& player, int itemslot);
void ProcessAnimState(Player& player, Sprite Sprites[]);
void ProcessWeaponCooldown(Player& player, float elapsedSec);
void ProcessFacing(Player& player, const SDL_MouseMotionEvent& e);
void Interact(Player& player, Cell cellArr[], const int cellArrSize, Room& currentRoom);
void ProcessMovement(Player& player, Cell cellArr[], const int arrSize, Sprite Sprites[], float elapsedSec);
#pragma endregion playerInputHandling

#pragma region projectileHandling
Projectile InitializeProjectile(std::string type, float speed);

void DrawProjectiles();
void UpdateProjectiles(float elapsedSec);
void DestroyProjectile(Projectile& projectile);
void ClearProjectiles(Projectile projectiles[]);
void CreateProjectile(Rectf location, std::string type, float direction, float speed, float damage);
#pragma endregion projectileHandling

#pragma region weaponHandling
Weapon FetchWeapon(const std::string& name);
Weapon InitializeWeapon(const std::string& weaponName, const std::string& textureName, const WeaponType& type, float damage);

void InitWeapons();
void DrawWeaponAnimation();
void SetWeaponCooldown(const Weapon& weapon);
void UpdateWeaponAnimation(float elapsedSec);
#pragma endregion weaponHandling

#pragma region itemHandling
Item FetchItem(const std::string& name);
Item InitializeItem(const std::string& itemName, const std::string& textureName, const ItemType& type, const EffectType effect, const float duration = 0.01f, const float modifier = 1.0f);

void InitItems();
void RollForDrop(Enemy& enemy);
void DrawStatusEffects(Player& player);
void UpdateStatusEffects(float elapsedSec);
#pragma endregion itemHandling

#pragma region interactableHandling
Interactable InitializeInteractable(const std::string& linkedItem, const InteractableType& type);

void DrawInteractables();
void ClearInteractables();
void SpawnInteractablesInRoom(const Room& room);
void PickUpInteractable(int index, int location);
void SpawnInteractable(std::string name, int location, InteractableType type);
void SetInteractablePickedUp(const Room& room, std::string interactableName, int location);
void AddInteractableToRoom(const Room& room, std::string interactableName, int location, InteractableType type);
void ReplaceInteractableInRoom(const Room& room, std::string interactableToReplace, std::string interactableReplacement, int location);
#pragma endregion interactableHandling

#pragma region enemyHandling
Enemy InitializeEnemy(std::string enemyName);
Enemy InitializeEnemy(const EnemyType& type, const std::string& textureName, float maxHealth, float damage, float timePerAction, int viewRange);

int GetRandomSpawn(Cell cellArr[], const int cellArrSize);
int GetEnemyGridIndex(Enemy& enemy, Cell cellArr[], const int arrSize);

void InitEnemies();
void ClearEnemies();
void DestroyEnemy(Enemy& enemy);

void DrawEnemyHealth(const Enemy& enemy);
void DrawEnemyHealthBars(Enemy enemyArr[]);
void DrawEnemies(Enemy enemyArr[], int arrSize);

void SpawnEnemies(const EnemyShorthand enemies[]);
void UpdateAnimationPos(float elapsedSec, Enemy& enemy);
void DamageAllEnemies(Enemy EnemyArr[], const int enemyArrSize);

void BasicEnemyAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize);
void RangedEnemyAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize);
void SummonerAI(float elapsedSec, Enemy& enemy, Cell cellArr[], int cellArrSize);

bool NecromancerCheck();
bool CheckForSummonedEnemy(std::string enemyToCheckName);
void SummonEnemy(std::string enemyToSummonName, int location);

void UpdateEnemies(float elapsedSec, Enemy enemyArr[], int enemyArrSize, Cell cellArr[], int cellArrSize);
void FireArrowFromEnemy(Cell cellArr[], const int indexDiffX, const int indexDiffY, Enemy& enemy, int enemyIndex);
#pragma endregion enemyHandling

#pragma region bossHandling
std::string GetRandomMinion();

bool IsBossDead();
bool IsBossOnTilesToScan(Boss boss, int tilesToScan[], int currentTile);

float BossDistanceToChargePoint();
float GetAngle(Boss boss, Player player);


void InitBoss();
void DrawBoss();
void InitTotems();
void DrawBossHealth();
void PrepareToCharge();
void BossLookAtPlayer();
void DrawTotems(Sprite totemsprite);
void UpdateTotems(float elapsedSec);
void ChargeAtPlayer(float elapsedSec);
void BossAttackPlayer(float elapsedSec);
void UpdateBossAIState(float elapsedSec);
void UpdateBossAnimState(float elapsedSec);
void InitVoiceLines(Texture voiceLinesArr[]);
void UpdateVoiceLine(Boss& boss, float elapsedSec);
void DrawVoiceLine(const Boss& boss, const Texture voiceLinesArr[]);
#pragma endregion bossHandling

#pragma region roomHandling
Room FetchRoom(const RoomID& id);

bool CheckRoomCleared(Room& currentRoom);

void InitializeRooms(Room level[]);
void LoadRoom(const Room& roomToLoad);
void SetRoomCleared(Room& currentRoom);
void OpenDoors(Cell cellArr[], int size);
void GoToLinkedRoom(const Room& roomOfDeparture, int playerIndex);
void LoadRoomLayout(Cell targetCellArr[], const std::string& fileName);
void SaveRoomLayout(Cell cellArr[], const int cellArrSize, const std::string& saveFileName);
#pragma endregion roomHandling


#pragma endregion Function Declarations

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
