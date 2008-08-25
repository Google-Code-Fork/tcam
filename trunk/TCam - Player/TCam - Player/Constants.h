/*********************************************************
Player Info 
*********************************************************/
#ifndef CONSTANTS_H
#define CONSTANTS_H

static int ExpMark = 0x626C64;
static int HeadMark = 0x62F190;

static int *PLAYER_HP					= (int *)(ExpMark + 8); // 8.22
static int *PLAYER_HP_MAX				= (int *)(ExpMark + 4); // 8.22
static int *PLAYER_MANA					= (int *)(ExpMark - 20); // 8.22
static int *PLAYER_MANA_MAX				= (int *)(ExpMark - 24); // 8.22
static int *PLAYER_CAP					= (int *)(ExpMark - 36); // 8.22
static int *PLAYER_EXP					= (int *)(ExpMark + 0); // 8.22
static int *PLAYER_LEVEL				= (int *)(ExpMark - 4); // 8.22
static int *PLAYER_MAGIC_LEVEL			= (int *)(ExpMark - 8); // 8.22
static int *PLAYER_LEVEL_PER			= (int *)(ExpMark - 12); // 8.22
static int *PLAYER_MAGIC_LEVEL_PER		= (int *)(ExpMark - 16); // 8.22
static int *PLAYER_SOUL					= (int *)(ExpMark - 28); // 8.22
static int *PLAYER_STAMINA				= (int *)(ExpMark - 32); // 8.22
static int *PLAYER_ID					= (int *)(ExpMark + 12); // 8.22
static int *PLAYER_X 					= (int *)(ExpMark + 144); // 8.22
static int *PLAYER_Y 					= (int *)(ExpMark + 148); // 8.22
static int *PLAYER_Z 					= (int *)(ExpMark + 152); // 8.22

static int *PLAYER_FIST 				= (int *)(ExpMark - 76); // 8.22
static int *PLAYER_CLUB					= (int *)(ExpMark - 72); // 8.22
static int *PLAYER_SWORD				= (int *)(ExpMark - 68); // 8.22
static int *PLAYER_AXE	 				= (int *)(ExpMark - 64); // 8.22
static int *PLAYER_DIST 				= (int *)(ExpMark - 60); // 8.22
static int *PLAYER_SHIELD 				= (int *)(ExpMark - 56); // 8.22
static int *PLAYER_FISH 				= (int *)(ExpMark - 52); // 8.22

static int *PLAYER_FIST_PER 			= (int *)(ExpMark - 104); // 8.22
static int *PLAYER_CLUB_PER 			= (int *)(ExpMark - 100); // 8.22
static int *PLAYER_SWORD_PER 			= (int *)(ExpMark - 96); // 8.22
static int *PLAYER_AXE_PER 	 			= (int *)(ExpMark - 92); // 8.22
static int *PLAYER_DIST_PER  			= (int *)(ExpMark - 88); // 8.22
static int *PLAYER_SHIELD_PER  			= (int *)(ExpMark - 84); // 8.22
static int *PLAYER_FISH_PER  			= (int *)(ExpMark - 80); // 8.22

static int *PLAYER_SLOT_HEAD			= (int *)(HeadMark + 0); // 8.22
static int *PLAYER_SLOT_NECKLACE		= (int *)(HeadMark + 12); // 8.22
static int *PLAYER_SLOT_CONTAINER		= (int *)(HeadMark + 24); // 8.22
static int *PLAYER_SLOT_ARMOR			= (int *)(HeadMark + 36); // 8.22
static int *PLAYER_SLOT_RIGHT			= (int *)(HeadMark + 48); // 8.22
static int *PLAYER_SLOT_LEFT			= (int *)(HeadMark + 60); // 8.22
static int *PLAYER_SLOT_LEGS			= (int *)(HeadMark + 72); // 8.22
static int *PLAYER_SLOT_FEET			= (int *)(HeadMark + 84); // 8.22
static int *PLAYER_SLOT_RING			= (int *)(HeadMark + 96); // 8.22
static int *PLAYER_SLOT_AMMO			= (int *)(HeadMark + 108); // 8.22

static int *PLAYER_SLOT_RIGHT_COUNT		= (int *)(HeadMark + 48 + 4); // 8.22
static int *PLAYER_SLOT_LEFT_COUNT		= (int *)(HeadMark + 60 + 4); // 8.22
static int *PLAYER_SLOT_AMMO_COUNT		= (int *)(HeadMark + 108 + 4); // 8.22

static int *PLAYER_CONNECTION			= (int *)0x77F3F8; // 8.22

/*********************************************************
BattleList Info
*********************************************************/

static DWORD BATTLELIST_BEGIN			= (ExpMark + 108); // 8.22
static DWORD BATTLELIST_END				= (ExpMark + 108) + (160*150); // 8.22

static int BATTLELIST_ID                              = 0;
static int BATTLELIST_NAME                            = 4;
static int BATTLELIST_X                               = 36;
static int BATTLELIST_Y                               = 40;
static int BATTLELIST_Z                               = 44;
static int BATTLELIST_WALKING                         = 76; 
static int BATTLELIST_DIRECTION                       = 80; 
static int BATTLELIST_OUTFIT                          = 96;
static int BATTLELIST_HEAD_COLOR                      = 100;
static int BATTLELIST_BODY_COLOR                      = 104;
static int BATTLELIST_LEGS_COLOR                      = 108;
static int BATTLELIST_FEET_COLOR                      = 112;
static int BATTLELIST_ADDON                           = 116; 
static int BATTLELIST_LIGHT                           = 120;
static int BATTLELIST_LIGHT_COLOR                     = 124;
static int BATTLELIST_LIGHT_PATTEN                    = 127;
static int BATTLELIST_HPBAR                           = 136;
static int BATTLELIST_WALK_SPEED                      = 140;
static int BATTLELIST_ISVISIBLE                       = 144;
static int BATTLELIST_SKULL                           = 148; 
static int BATTLELIST_PARTY                           = 152;
static int BATTLELIST_STEP							  = 160;

/*********************************************************
Containers
*********************************************************/

static DWORD CONTAINER_BEGIN		    = 0x62F208; // 8.22
static DWORD CONTAINER_END				= (CONTAINER_BEGIN + (16 * 492)); // 8.22
static int CONTAINER_STEP               = 492;
static int CONTAINER_NUM_OF_ITEMS       = 56;
static int CONTAINER_ITEM_DISTANCE      = 60;
static int CONTAINER_ITEM_STEP          = 12;

/*********************************************************
Window Info
*********************************************************/
static int *ScreenPtr1 = (int *)0x631AC0; // 8.22
static int *PopupWindow = (int *)0x77F458; // 8.22

/*********************************************************
Misc.
*********************************************************/

static int *TARGET_ID 					= (int *)0x626C3C; // 8.22
static unsigned long *XTeaAddress		= (unsigned long *)0x77BDB4;          // 8.22
static int *MAP_POINTER                 = (int *)0x636610; // 8.22
static int MapTileDist					= 172;
static int MapObjectDist				= 12;
static int MapObjectIdDist				= 0;
static int MapObjectDataDist			= 4;
static int MapObjectIdOffset			= 4;
static int MapObjectDataOffset			= 8;
static int *MOUSE_POINTER               = (int *)0x77F458; // 8.22

static int VIP_BEGIN					= 0x624990; // 8.22
static int VipPlayerDist				= 44;
static int VipNameOffset				= 4;
static int VipStatusOffset				= 34;
static int VIP_END						= (VIP_BEGIN + 4400); // 8.22

static DWORD LoginServ0					= 0x776CF0; // 8.22
static DWORD LoginServ1					= (LoginServ0 + 112); // 8.22
static DWORD LoginServ2					= (LoginServ1 + 112); // 8.22
static DWORD LoginServ3					= (LoginServ2 + 112); // 8.22
static DWORD LoginServ4					= (LoginServ3 + 112); // 8.22
static DWORD LoginServ5					= (LoginServ4 + 112); // 8.22
static DWORD LoginServ6					= (LoginServ5 + 112); // 8.22
static DWORD LoginServ7					= (LoginServ6 + 112); // 8.22
static DWORD LoginServ8					= (LoginServ7 + 112); // 8.22
static DWORD LoginServ9					= (LoginServ8 + 112); // 8.22

static int LoginPort0					= 0x776D54; // 8.22
static int LoginPort1					= (LoginPort0 + 112); // 8.22
static int LoginPort2					= (LoginPort1 + 112); // 8.22
static int LoginPort3					= (LoginPort2 + 112); // 8.22
static int LoginPort4					= (LoginPort3 + 112); // 8.22
static int LoginPort5					= (LoginPort4 + 112); // 8.22
static int LoginPort6					= (LoginPort5 + 112); // 8.22
static int LoginPort7					= (LoginPort6 + 112); // 8.22
static int LoginPort8					= (LoginPort7 + 112); // 8.22
static int LoginPort9					= (LoginPort8 + 112); // 8.22

#endif