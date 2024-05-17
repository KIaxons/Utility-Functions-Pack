#pragma once
#include "pch.h"
#include "shlobj.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include "inc\Tests.h"
//using namespace std;

#define M_PI 3.14159265358979323846

#define t_ObjNone 0
#define t_ObjStar 1
#define t_ObjHole 2
#define t_ObjPlanet 3
#define t_ObjStation 4
#define t_ObjShip 5
#define t_ObjItem 6
#define t_ObjMissile 7
#define t_ObjAsteroid 8
#define t_ObjSector 9

#define t_Kling 0
#define t_Ranger 1
#define t_Transport 2
#define t_Pirate 3
#define t_Warrior 4
#define t_Tranclucator 5
#define t_RC 6
#define t_PB 7
#define t_WB 8
#define t_SB 9
#define t_BK 10
#define t_MC 11
#define t_CB 12
#define t_UB 13

#define t_Food 0
#define t_Medicine 1
#define t_Technics 2
#define t_Luxury 3
#define t_Minerals 4
#define t_Alcohol 5
#define t_Arms 6
#define t_Narcotics 7
#define t_Artefact 8
#define t_Artefact2 9
#define t_ArtefactHull 10
#define t_ArtefactFuel 11
#define t_ArtefactSpeed 12
#define t_ArtefactPower 13
#define t_ArtefactRadar 14
#define t_ArtefactScaner 15
#define t_ArtefactDroid 16
#define t_ArtefactNano 17
#define t_ArtefactHook 18
#define t_ArtefactDef 19
#define t_ArtefactAnalyzer 20
#define t_ArtefactMiniExpl 21
#define t_ArtefactAntigrav 22
#define t_ArtefactTransmitter 23
#define t_ArtefactBomb 24
#define t_ArtefactTranclucator 25
#define t_ArtDefToEnergy 26
#define t_ArtEnergyPulse 27
#define t_ArtEnergyDef 28
#define t_ArtSplinter 29
#define t_ArtDecelerate 30
#define t_ArtMissileDef 31
#define t_ArtForsage 32
#define t_ArtWeaponToSpeed 33
#define t_ArtGiperJump 34
#define t_ArtBlackHole 35
#define t_ArtDefToArms1 36
#define t_ArtDefToArms2 37
#define t_ArtArtefactor 38
#define t_ArtBio 39
#define t_ArtPDTurret 40
#define t_ArtFastRacks 41
#define t_Hull 42
#define t_FuelTanks 43
#define t_Engine 44
#define t_Radar 45
#define t_Scaner 46
#define t_RepairRobot 47
#define t_CargoHook 48
#define t_DefGenerator 49
#define t_Weapon1 50
#define t_Weapon2 51
#define t_Weapon3 52
#define t_Weapon4 53
#define t_Weapon5 54
#define t_Weapon6 55
#define t_Weapon7 56
#define t_Weapon8 57
#define t_Weapon9 58
#define t_Weapon10 59
#define t_Weapon11 60
#define t_Weapon12 61
#define t_Weapon13 62
#define t_Weapon14 63
#define t_Weapon15 64
#define t_Weapon16 65
#define t_Weapon17 66
#define t_Weapon18 67
#define t_CustomWeapon 68
#define t_Protoplasm 69
#define t_UselessItem 70
#define t_MicroModule 71
#define t_Cistern 72
#define t_Satellite 73
#define t_UselessCountableItem 74

#define AutoBattleOrder -1
#define NoneOrder 0
#define MoveOrder 1
#define LandingOrder 2
#define JumpOrder 3
#define JumpHoleOrder 4
#define TakeOffOrder 5
#define FollowOrder 6
#define TeleportOrder 7

static volatile TGalaxy* Galaxy = nullptr;
static TPlayer* Player = nullptr;

//Конвертация wstring в string
std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}
//Конвертация string в wstring
std::wstring s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

//Структура для массива с указателем корабля и дистанцией его удалённости от целевого корабля
struct shipNearby
{
public:
    TShip* Ship;
    int Dist;
};
static std::vector <shipNearby> ShipsNearby;
void ShipsNearbySort(std::vector<shipNearby>& vector, int lo, int hi);
int ShipsNearbyPartition(std::vector<shipNearby>& vector, int lo, int hi);

void SFT(const std::wstring& str_out);
//Функция для преобразования переменной типа unsigned int, полученной как float (на самом деле в игре double, но передаётся как float) из скрипта, в стандартный double, используемый в C++
double FloatFromGame(unsigned int a) { return *(float*)&a; }
//Функция для преобразования переменной типа double из C++ в значение unsigned int, распознаваемое скриптом
unsigned int FloatToGame(float a)    { return *(unsigned int*)&(a); }
int Rnd(int min, int max);
byte ObjectType(void* obj);
byte GalaxyTechLevel()               { return Galaxy->GTL; } //Возвращает текущий ГТУ
int GalaxyDiffLevels(int diff_num, bool base = false) { if(!base) return ((Galaxy->diff_levels[diff_num] + 1) * 50); else return Galaxy->diff_levels[diff_num]; } //Возвращает значение указанной игровой сложности из 8 доступных (по умолчанию в формате 50-500)
byte ShipTypeN(TShip* ship)          { return ship->type; }
std::wstring ShipType(TShip* ship);
bool ShipInNormalSpace(TShip* ship);
bool ShipInHyperSpace(TShip* ship);
TPlanet* GetShipPlanet(TShip* ship);
TShip* GetShipRuins(TShip* ship);
TShip* ShipGetBad(TShip* ship)       { return ship->ship_bad; }
std::wstring ShipName(TShip* ship)   { std::wstring name = ship->name; return name; }
double ShipAngle(TShip* ship)        { return (float)ship->angle; }
//Возвращает числовой подтип корабля
byte ShipSubType(TShip* ship)
{
    switch(ShipTypeN(ship))
    {
        case(t_Kling):     return ((TKling*)ship)->sub_type;
        case(t_Transport): return ((TTransport*)ship)->sub_type;
        case(t_Pirate):    return ((TPirate*)ship)->sub_type;
        case(t_Warrior):   return ((TWarrior*)ship)->sub_type;
    }
    return 0;
}

const wchar_t* GetParFromTxtT(
    wchar_t* txt_path,
    wchar_t* s_param_name
);
const wchar_t* SetParFromTxtT(
    wchar_t* txt_path,
    wchar_t* s_param_name,
    wchar_t* value
);
int ShipItems(TShip* ship)                    { return ship->items->count; }
int ItemType(TItem* item)                     { return item->type; }
uint32_t ShipItems(TShip* ship, int item_num) { return ship->items->items[item_num]; }
int ShipArts(TShip* ship)                     { return ship->arts->count; }
uint32_t ShipArts(TShip* ship, int art_num)   { return ship->arts->items[art_num]; }
byte ShipOrder(TShip* ship)                   { return ship->order; }
void ShipOrder(TShip* ship, byte new_order)   { ship->order = new_order; }
int ItemCost(TItem* item)                     { return item->cost; }
int ItemSize(TItem* item)                     { return item->size; }
int StarShips(TStar* star)                    { return star->ships->count; }
TShip* StarShips(TStar* star, int index)      { return (TShip*)star->ships->items[index]; }
TStar* ShipStar(TShip* ship)                  { return ship->cur_star; }
std::wstring GetParFromStringLocal(wchar_t* g_par_string, wchar_t* g_sep, int g_par_num, int clear_spaces);
int GalaxyStars()                             { return Galaxy->stars->count; }
TStar* GalaxyStar(int star_num)               { return (TStar*)Galaxy->stars->items[star_num]; }
int StorageItems() { return Player->global_item_storage->count; }
TItem* StorageItems(int num) { return (TItem*)((TStorageUnit*)Player->global_item_storage->items[num])->item; }
uint32_t StorageItemLocation(int num) { return (uint32_t)((TStorageUnit*)Player->global_item_storage->items[num])->place; }
float AngleBetweenShipCoords(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    if(!dx && !dy) return 0.0f;
    //return (float)atan2(dy, dx); //Для вычисления нормального, а не дабоугла
    return (float)atan2(-dx, dy);
}

//Объявляем для запуска отдельного потока
void RangersCapitalOverride();