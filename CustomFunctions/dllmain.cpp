#include "pch.h"
#include "shlobj.h"
#include <string>
#include <ctime> //��� ������� time()
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
//��� ����������� wstring � string
#include <locale>
#include <codecvt>
//��� ������ � CString
#include <atlstr.h>
//��� ��������
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
//��� ������ � ��������� ���� vector
#include <vector>
//�������� ��������� � ��������� ���������� ������������ ������� ���������
#include "structs/include.h"
#include "wrapped-classes/include.h"
#include "TList_methods.cpp"
//��� ������ ���������� ������, ������������ rangers_average_capital �� ���������
#include <thread>
//�������� ���� ��������
#include "dllmain.h"
//��� ������� ShellExecute
#include <windows.h>
#include <ShellAPI.h>

//using namespace std;

//������ ��������� ������� Project - > CustomFunctions Properties:
//Configuration Properties > C/C++ > Code Generation > Runtime Library > Multi - threaded (/MT) (��� ������)
//Configuration Properties > C/C++ > Code Generation > Runtime Library > Multi - threaded Debug (/MTd) (��� ������)
//���������: https://stackoverflow.com/questions/3162325/after-building-exe-using-vs-2010-c-missing-msvcp100-dll

//� ���������� ������� �������� �������� �� ������������ �������, ������ ��� ����� �����, ������!
//C/C++ -> General -> SDL checks - > No

//������������ ������������� ��������� �������� � ��������� Unicode �� Multi-Byte:
//Configuration Properties -> Advanced -> Character Set -> Use Multi-Byte

WCHAR system_folder[MAX_PATH];
WCHAR log_path[MAX_PATH];

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch(ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            std::ios_base::sync_with_stdio(false);

            srand(time(0));
            SHGetSpecialFolderPathW(0, system_folder, CSIDL_PERSONAL, true);
            wcscpy(log_path, system_folder);
            wcscat(log_path, L"\\SpaceRangersHD\\########.log");
            SFT(L"UtilityFunctions.dll version 2.1.0 was linked");
            break;
        }
        case DLL_THREAD_ATTACH: break;
        case DLL_THREAD_DETACH: break;
        case DLL_PROCESS_DETACH:
        {
            //system("taskkill /F /T /IM WindowsProjectTest.exe");

            //const auto explorer = OpenProcess(PROCESS_TERMINATE, false, 5556536);
            //TerminateProcess(explorer, 1);
            //CloseHandle(explorer);
            break;
        }
    }
    return TRUE;
}

//����������� ��������� ��������� wstring, �.�. ��� ����� ��� ����������� ������ � ����
static std::wstring to_game;

static volatile int rangers_average_capital;
static volatile bool rangers_capital_override = false;

//������ ������� �������
static VMT vmt_TObject = nullptr;
static VMT vmt_TObjectEx = nullptr;
static VMT vmt_TConstellation = nullptr;
static VMT vmt_TStar = nullptr;
static VMT vmt_THole = nullptr; //�� ����������������
static VMT vmt_TPlanet = nullptr;
static VMT vmt_TRanger = nullptr;
static VMT vmt_TShip = nullptr;
static VMT vmt_TPlayer = nullptr;
static VMT vmt_TMissile = nullptr; //�� ����������������
static VMT vmt_TAsteroid = nullptr;
static VMT vmt_TItem = nullptr;
static VMT vmt_TEquipment = nullptr;
static VMT vmt_TGoods = nullptr; //�� ����������������
static VMT vmt_TScript = nullptr;
static VMT_TScriptShip *vmt_TScriptShip = nullptr;

TScriptShip* (__fastcall* TScriptShip_Create) (VMT_TScriptShip*, int8_t flag);
int(__fastcall* TScriptShip_Destroy) (TScriptShip* list, int8_t flag);

TScriptShip* (__fastcall* TScriptShip_New) (VMT_TScriptShip*);

void* (__fastcall* GetMem) (int);
void(__fastcall* FreeMem) (void*);

//������� ��� ����������� ������ ����
//������������ ��� ������ ������ � ������� ���,
//����� ��������� �������� � �������������� ���������
//(��� �������� ����� � ������ ������������ to_string())
void SFT(const std::wstring& str_out)
{
    //const wchar_t* out = str_out.c_str();
    std::string out_st = ws2s(str_out);
    const char* out = out_st.c_str(); //���������� ���������� � ������� string, ��������� ��� ��������� � UTF-8
    //"����������" ������� �� ������ ��� ������������
    //FILE* file;
    //errno_t err = fopen_s(&file, log_path, "a");

    FILE* file = _wfopen(log_path, L"a");
    fwrite(out, strlen(out), 1, file);
    fwrite("\n", 1, 1, file);
    fclose(file);
}

//�������� � ���������� � �������� ������ ��������� �� ���� (������ ������� ����������� ���������� ����� ����� �������� ����)
extern "C" __declspec(dllexport)
void UtilityFunctionsLibInit(TGalaxy* galaxy)
{
    Galaxy = galaxy;
    Player = (TPlayer*)Galaxy->rangers->items[Galaxy->player_index];

    vmt_TPlayer = Player->cls;
    VMT prev_prev_prev_parent = vmt_TPlayer;
    VMT prev_prev_parent = vmt_TPlayer;
    VMT prev_parent = vmt_TPlayer;
    VMT parent = vmt_struct(vmt_TPlayer)->parent;
    while(parent)
    {
        prev_prev_prev_parent = prev_prev_parent;
        prev_prev_parent = prev_parent;
        prev_parent = parent;
        parent = ((VMT_helper*)parent)->parent;
    }
    vmt_TObject = prev_parent;
    vmt_TObjectEx = prev_prev_parent;
    vmt_TShip = prev_prev_prev_parent;

    //����� TEquipment �������� ����� ��������, ����� TGoods, � ����� �������� � ���� ��� ����������
    parent = vmt_struct(((THull*)Player->items->items[0])->cls)->parent;
    while(parent != vmt_TObjectEx)
    {
        prev_prev_parent = prev_parent;
        prev_parent = parent;
        parent = ((VMT_helper*)parent)->parent;
    }
    vmt_TItem = prev_parent;
    vmt_TEquipment = prev_prev_parent;

    //������
    TConstellation* sector = (TConstellation*)Galaxy->sectors->items[0];
    vmt_TConstellation = sector->cls;

    TStar* star = (TStar*)Galaxy->stars->items[0];
    vmt_TStar = star->cls;
    vmt_TAsteroid = ((TAsteroid*)(star->asteroids->items[0]))->cls;
    vmt_TPlanet = ((TPlanet*)(Galaxy->planets->items[0]))->cls;
    vmt_TRanger = ((TRanger*)(Galaxy->rangers->items[0]))->cls;
    vmt_TScript = ((TScript*)(Galaxy->scripts->items[0]))->cls;

    TScriptShip* first_obj = (TScriptShip*)Player->script_ships_objects->items[0];
    vmt_TScriptShip = (VMT_TScriptShip*)first_obj->cls;

    //�������������� ������ ��� TList
    TList_methods_init(Galaxy->scripts);
    //����� ������������ � �����������
    *(void**)&TScriptShip_Destroy = *((void**)vmt_TScriptShip - 1);
    *(void**)&TScriptShip_New = *((void**)vmt_TScriptShip - 3);
    SET_VAR_WITH_OFFSET(TScriptShip_Create, TScriptShip_Destroy, 0x52C - 0x570);

    *(void**)&GetMem = (void*)0x402488;
    *(void**)&FreeMem = (void*)0x402B0C;
}

//������� ���������� ���������� ������� ShipsNearby
void ShipsNearbySort(std::vector<shipNearby>& vector, int lo, int hi)
{
    if(hi <= lo) return;

    int j = ShipsNearbyPartition(vector, lo, hi);

    ShipsNearbySort(vector, lo, j - 1);
    ShipsNearbySort(vector, j + 1, hi);
}
int ShipsNearbyPartition(std::vector<shipNearby>& vector, int lo, int hi)
{
    int i = lo;
    int j = hi + 1;

    while(true)
    {
        while(vector[++i].Dist < vector[lo].Dist)
        {
            if(i == hi) break;
        }

        while(vector[--j].Dist > vector[lo].Dist)
        {
            if(j == lo) break;
        }

        if(i >= j) break;

        std::swap(vector[i], vector[j]);
    }

    std::swap(vector[lo], vector[j]);

    return j;
}

void TListAdd(void* list, void* object)
{
    //���������� ���������� ��� ������ ������
    ASM_CALL(TList_Add, list, object);
}
void TListDelete(void* list, int el_num)
{
    //���������� ���������� ��� ������ ������
    ASM_CALL(TList_Delete, list, el_num);
}

//���������� ��������� ����� ����� ���������� min � max (������� ��������� ���������� �������)
int Rnd(int min, int max)
{
    unsigned int seed;
    //�� ������, ���� �������� �� � ���������� �� �� ������
    if(Galaxy->AA_enabled && Galaxy->AA_rnd_chaotic) seed = rand();
    else
    {
        //������ ����� ����� �����
        seed = Galaxy->rnd_seed;
        unsigned int prev_seed = seed;
        seed = seed * 7981 + 567 + seed / 7981;
        if(prev_seed == seed) //�� ������, ���� seed "����������" �� ����� �������� (����� ����, ��� ��������� ��������)
        {
            seed = seed * 7281 + 517 + seed / 7181;
        }
        //������� ����� ����� � ������� ���������
        Galaxy->rnd_seed = seed;
    }

    //����� ���������� �� ���� ����������� ������
    if(min < max) return min + int(seed % (max - min + 1));
    return max + int(seed % (min - max + 1));
}

//���������� ����������
int BankRound(double value)
{
    float fShift = (value >= 0 ? 0.5 : -0.5);
    //��������� ������� �������� ��� ����������
    if(fabs(fabs(value) - fabs(double(int(value))) - 0.5) < DBL_EPSILON)
    {
        //return (double(int(value / 2.0 + fShift) * 2));
        return (double(nearbyint(value * 0.5f) * 2.0f)); //��� ����� �� �����������
    }
    return (double(int(value + fShift)));
}
//���������� � ���������� ��������� ��� ������� �������� (������ ��� ����������, �� ��������)
int NearestOddRound(double value)
{
    float fShift = (value >= 0 ? 0.5 : -0.5);
    //��������� ������� �������� ��� ����������
    if(fabs(fabs(value) - fabs(double(int(value))) - 0.5) < DBL_EPSILON)
    {
        return (double(2 * floor(value * 0.5f) + 1));
    }
    return (double(int(value + fShift)));
}
//������� ��� �������� �������� � ��������� � ������ ������
static inline std::wstring& ltrim(std::wstring& s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), std::not_fn(isspace)));
    return s;
}
//������� ��� �������� �������� � ��������� � ����� ������
static inline std::wstring& rtrim(std::wstring& s)
{
    s.erase(find_if(s.rbegin(), s.rend(), std::not_fn(isspace)).base(), s.end());
    return s;
}
//������� ��� �������� �������� � ��������� � ����� � � ������ ������
static inline std::wstring& trim(std::wstring &s)
{
    return ltrim(rtrim(s));
}

//���������� ����� ���� ����������� �������
byte ObjectType(void* obj)
{
    VMT vmt = ((TShip*)obj)->cls;
    VMT prev_parent = vmt;
    VMT parent = vmt_struct(vmt)->parent;
    while(parent != vmt_TObjectEx && parent != vmt_TObject) //����� TStar ��������� �������� �� TObject
    {
        prev_parent = parent;
        parent = ((VMT_helper*)parent)->parent;
    }
    vmt = prev_parent;

    if(vmt == vmt_TShip)
    {
        if(ShipTypeN((TShip*)obj) < t_RC) return t_ObjShip;
        else return t_ObjStation;
    }
    else if(vmt == vmt_TItem) return t_ObjItem;
    else if(vmt == vmt_TPlanet) return t_ObjPlanet;
    else if(vmt == vmt_TStar) return t_ObjStar;
    else if(vmt == vmt_TAsteroid) return t_ObjAsteroid;
    else if(vmt == vmt_TConstellation) return t_ObjSector;
    //else if(vmt == vmt_TMissile) return t_ObjMissile;
    //else if(vmt == vmt_THole) return t_ObjHole;
    else return t_ObjNone;
}

//���������, ��������� �� � �������� ������� ��������� �������
bool ShipInNormalSpace(TShip* ship)
{
    return (!GetShipPlanet(ship) && !GetShipRuins(ship) && !ShipInHyperSpace(ship));
}
//���������, ��������� �� � ������ ��������� �������
bool ShipInHyperSpace(TShip* ship)
{
    return ship->in_hyper_space;
}
//���������� �������, �� ������� ��������� ��������� ������� (����� 0)
TPlanet* GetShipPlanet(TShip* ship)
{
    if(ship == Player)
    {
        //���� ����� ����� �� ������� (����� ����� �������), ���������� ��������� �������, �� ������� "�����" ���� �������-������
        if(Player->bridge_num) return Player->bridge_cur_planet;
    }
    return ship->cur_planet;
}
//���������� �������, �� ������� ��������� ��������� ������� (����� 0)
TShip* GetShipRuins(TShip* ship)
{
    if(ship == Player)
    {
        //���� ����� ����� �� ������� (����� ����� �������), ���������� ��������� �������, �� ������� "�����" ���� �������-������
        if(Player->bridge_num) return Player->bridge_cur_ship;
    }
    return ship->cur_ship;
}

int GoodsSellPrice(uint32_t place, int goods_num)
{
    if(ObjectType((void*)place) == t_ObjPlanet) return ((TPlanet*)place)->goods_shop[goods_num].sell_price;
    else return ((TRuins*)place)->goods_shop[goods_num].sell_price;
}
void GoodsSellPrice(uint32_t place, int goods_num, int new_cost)
{
    if(ObjectType((void*)place) == t_ObjPlanet) ((TPlanet*)place)->goods_shop[goods_num].sell_price = new_cost;
    else ((TRuins*)place)->goods_shop[goods_num].sell_price = new_cost;
}
int GoodsBuyPrice(uint32_t place, int goods_num)
{
    if(ObjectType((void*)place) == t_ObjPlanet) return ((TPlanet*)place)->goods_shop[goods_num].buy_price;
    else return ((TRuins*)place)->goods_shop[goods_num].buy_price;
}
void GoodsBuyPrice(uint32_t place, int goods_num, int new_cost)
{
    if(ObjectType((void*)place) == t_ObjPlanet) ((TPlanet*)place)->goods_shop[goods_num].buy_price = new_cost;
    else ((TRuins*)place)->goods_shop[goods_num].buy_price = new_cost;
}

//������� �� �������
//������� ��� �������� ����� ��������� ���������� �������: ���������, �������, ������� ��� �������
extern "C" __declspec(dllexport)
int GetObjectGenerationSeed(void* obj)
{
    if(obj == Galaxy) return Galaxy->gen_seed;
    else
    {
        //���������� ��� � ������ ��� ��������, � ������� ������� ��� �����
        int obj_type = ObjectType(obj);
        int* rnd_seed = nullptr;
        switch (obj_type)
        {
            case(t_ObjShip):
            case(t_ObjStation): return ((TShip*)obj)->gen_seed;
            case(t_ObjPlanet): return ((TPlanet*)obj)->gen_seed;
            case(t_ObjStar): return ((TStar*)obj)->gen_seed;
            default:
            {
                SFT(L"UtilityFunctions.dll GetObjectGenerationSeed Error! Wrong type of object was received!");
                throw 0;
            }
        }
    }
}

//������� ��� �������� �������� ����� ���������� �������: ���������, �������, ������� ��� �������
extern "C" __declspec(dllexport)
int GetObjectSeed(void* obj)
{
    if(obj == Galaxy) return Galaxy->rnd_seed;
    else
    {
        //���������� ��� � ������ ��� ��������, � ������� ������� ��� �����
        int obj_type = ObjectType(obj);
        int* rnd_seed = nullptr;
        switch(obj_type)
        {
            case(t_ObjShip):
            case(t_ObjStation):
            {
                return ((TShip*)obj)->rnd_seed;
                break;
            }
            case(t_ObjPlanet):
            {
                return ((TPlanet*)obj)->rnd_seed;
                break;
            }
            case(t_ObjStar):
            {
                return ((TStar*)obj)->rnd_seed;
                break;
            }
            default:
            {
                SFT(L"UtilityFunctions.dll GetObjectSeed Error! Wrong type of object was received!");
                throw 0;
            }
        }
    }
}

//���������� ������, ������� �� ������� ��������� (���� �� ��� ������ �� ���������, �� �� �����)
extern "C" __declspec(dllexport)
unsigned int GetSectorAdjacentToDicea()
{
    for(int i = 0; i < Galaxy->sectors->count; ++i)
    {
        TConstellation* sector = (TConstellation*)Galaxy->sectors->items[i];
        if(sector->id == 20) continue; //���������� ��� ��������� ������� ������
        if(sector->p_bound_hidden->count) return (unsigned int)sector;
    }
    return 0;
}

//����������/�������� �������� ��������� ������ ���������
extern "C" __declspec(dllexport)
int AdvancedAdjustmentSet(
    int option,
    int val
)
{
    switch(option)
    {
    case -2:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for zawarudo was received.");
                throw 0;
            }

            int old_val = Galaxy->zawarudo;
            Galaxy->zawarudo = val;
            return old_val;
        }
        return Galaxy->zawarudo;
    }
    case -1:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for iron_will_mode was received.");
                throw 0;
            }

            int old_val = Galaxy->iron_will_mode;
            Galaxy->iron_will_mode = val;
            return old_val;
        }
        return Galaxy->iron_will_mode;
    }
    case 0:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_enabled was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_enabled;
            Galaxy->AA_enabled = val;
            return old_val;
        }
        return Galaxy->AA_enabled;
    }
    case 1:
    {
        if(val != -1)
        {
            if(val < 50 || val > 500)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_kling_strength was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_kling_strength;
            Galaxy->AA_kling_strength = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_kling_strength * 6.25);
    }
    case 2:
    {
        if(val != -1)
        {
            if(val < 50 || val > 500)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_kling_aggro was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_kling_aggro;
            Galaxy->AA_kling_aggro = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_kling_aggro * 6.25);
    }
    case 3:
    {
        if(val != -1)
        {
            if(val < 50 || val > 500)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_kling_spawn was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_kling_spawn;
            Galaxy->AA_kling_spawn = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_kling_spawn * 6.25);
    }
    case 4:
    {
        if(val != -1)
        {
            if(val < 50 || val > 500)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_pirate_aggro was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_pirate_aggro;
            Galaxy->AA_pirate_aggro = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_pirate_aggro * 6.25);
    }
    case 5:
    {
        if(val != -1)
        {
            if(val < 50 || val > 200)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_coal_aggro was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_coal_aggro;
            Galaxy->AA_coal_aggro = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_coal_aggro * 6.25);
    }
    case 6:
    {
        if(val != -1)
        {
            if(val < 50 || val > 200)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_asteroid_mod was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_asteroid_mod;
            Galaxy->AA_asteroid_mod = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_asteroid_mod * 6.25);
    }
    case 7:
    {
        if(val != -1)
        {
            if(val < 50 || val > 200)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_sun_damage_mod was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_sun_damage_mod;
            Galaxy->AA_sun_damage_mod = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_sun_damage_mod * 6.25);
    }
    case 8:
    {
        if(val != -1)
        {
            if(val < 0 || val > 255)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_extra_inventions was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_extra_inventions;
            Galaxy->AA_extra_inventions = val;
            return old_val;
        }
        return Galaxy->AA_extra_inventions;
    }
    case 9:
    {
        if(val != -1)
        {
            if(val < 0 || val > 100)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_akrin_mod was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_akrin_mod;
            Galaxy->AA_akrin_mod = val;
            return old_val;
        }
        return Galaxy->AA_akrin_mod;
    }
    case 10:
    {
        if(val != -1)
        {
            if(val < 50 || val > 200)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_node_drop_mod was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_node_drop_mod;
            Galaxy->AA_node_drop_mod = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_node_drop_mod * 6.25);
    }
    case 11:
        if(val != -1)
        {
            if(val < 50 || val > 200)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_AB_drop_value_mod was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_AB_drop_value_mod;
            Galaxy->AA_AB_drop_value_mod = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_AB_drop_value_mod * 6.25);
    case 12:
        if(val != -1)
        {
            if(val < 50 || val > 200)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_drop_value_mod was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_drop_value_mod;
            Galaxy->AA_drop_value_mod = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_drop_value_mod * 6.25);
    case 13:
        if(val != -1)
        {
            if(val < 0 || val > 10)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_ag_planets was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_ag_planets;
            Galaxy->AA_ag_planets = val;
            return old_val;
        }
        return Galaxy->AA_ag_planets;
    case 14:
        if(val != -1)
        {
            if(val < 0 || val > 10)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_mi_planets was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_mi_planets;
            Galaxy->AA_mi_planets = val;
            return old_val;
        }
        return Galaxy->AA_mi_planets;
    case 15:
        if(val != -1)
        {
            if(val < 0 || val > 10)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_in_planets was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_in_planets;
            Galaxy->AA_in_planets = val;
            return old_val;
        }
        return Galaxy->AA_in_planets;
    case 16:
        if(val != -1)
        {
            if(val < 0 || val > 50)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_extra_rangers was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_extra_rangers;
            Galaxy->AA_extra_rangers = val;
            return old_val;
        }
        return Galaxy->AA_extra_rangers;
    case 17:
        if(val != -1)
        {
            if(val < 50 || val > 200)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_AB_hitpoints_mod was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_AB_hitpoints_mod;
            Galaxy->AA_AB_hitpoints_mod = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_AB_hitpoints_mod * 6.25);
    case 18:
        if(val != -1)
        {
            if(val < 50 || val > 200)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_AB_damage_mod was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_AB_damage_mod;
            Galaxy->AA_AB_damage_mod = BankRound(0.16 * val) - 8;
            return 50 + BankRound(old_val * 6.25);
        }
        return 50 + BankRound(Galaxy->AA_AB_damage_mod * 6.25);
    case 19:
        if(val != -1)
        {
            if(val < 0 || val > 50)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_AI_tolerate_junk was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_AI_tolerate_junk;
            Galaxy->AA_AI_tolerate_junk = val;
            return old_val;
        }
        return Galaxy->AA_AI_tolerate_junk;
    case 20:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_rnd_chaotic was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_rnd_chaotic;
            Galaxy->AA_rnd_chaotic = val;
            return old_val;
        }
        return Galaxy->AA_rnd_chaotic;
    case 21:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_eq_knowledge_restricted was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_eq_knowledge_restricted;
            Galaxy->AA_eq_knowledge_restricted = val;
            return old_val;
        }
        return Galaxy->AA_eq_knowledge_restricted;
    case 22:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_ruins_near_stars was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_ruins_near_stars;
            Galaxy->AA_ruins_near_stars = val;
            return old_val;
        }
        return Galaxy->AA_ruins_near_stars;
    case 23:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_ruins_targetting_full was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_ruins_targetting_full;
            Galaxy->AA_ruins_targetting_full = val;
            return old_val;
        }
        return Galaxy->AA_ruins_targetting_full;
    case 24:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_special_ships_in_game was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_special_ships_in_game;
            Galaxy->AA_special_ships_in_game = val;
            return old_val;
        }
        return Galaxy->AA_special_ships_in_game;
    case 25:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_zero_start_exp was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_zero_start_exp;
            Galaxy->AA_zero_start_exp = val;
            return old_val;
        }
        return Galaxy->AA_zero_start_exp;
    case 26:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_AB_battle_royale was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_AB_battle_royale;
            Galaxy->AA_AB_battle_royale = val;
            return old_val;
        }
        return Galaxy->AA_AB_battle_royale;
    case 27:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_kling_racial_weapons was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_kling_racial_weapons;
            Galaxy->AA_kling_racial_weapons = val;
            return old_val;
        }
        return Galaxy->AA_kling_racial_weapons;
    case 28:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_start_center was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_start_center;
            Galaxy->AA_start_center = val;
            return old_val;
        }
        return Galaxy->AA_start_center;
    case 29:
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_max_range_missiles was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_max_range_missiles;
            Galaxy->AA_max_range_missiles = val;
            return old_val;
        }
        return Galaxy->AA_max_range_missiles;
    case 30:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_old_hyper was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_old_hyper;
            Galaxy->AA_old_hyper = val;
            return old_val;
        }
        return Galaxy->AA_old_hyper;
    }
    case 31:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_pirate_nodes was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_pirate_nodes;
            Galaxy->AA_pirate_nodes = val;
            return old_val;
        }
        return Galaxy->AA_pirate_nodes;
    }
    case 32:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_AI_use_shops was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_AI_use_shops;
            Galaxy->AA_AI_use_shops = val;
            return old_val;
        }
        return Galaxy->AA_AI_use_shops;
    }
    case 33:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_ruins_use_shops was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_ruins_use_shops;
            Galaxy->AA_ruins_use_shops = val;
            return old_val;
        }
        return Galaxy->AA_ruins_use_shops;
    }
    case 34:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_duplicate_arts was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_duplicate_arts;
            Galaxy->AA_duplicate_arts = val;
            return old_val;
        }
        return Galaxy->AA_duplicate_arts;
    }
    case 35:
    {
        if(val != -1)
        {
            if(val < 0 || val > 2)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_hull_growth was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_hull_growth;
            Galaxy->AA_hull_growth = val;
            return old_val;
        }
        return Galaxy->AA_hull_growth;
    }
    case 36:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_AB_equip_change was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_AB_equip_change;
            Galaxy->AA_AB_equip_change = val;
            return old_val;
        }
        return Galaxy->AA_AB_equip_change;
    }
    case 37:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_linear_speed_formula was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_linear_speed_formula;
            Galaxy->AA_linear_speed_formula = val;
            return old_val;
        }
        return Galaxy->AA_linear_speed_formula;
    }
    case 38:
    {
        if(val != -1)
        {
            if(val < 0 || val > 1)
            {
                SFT(L"UtilityFunctions.dll AdvancedAdjustmentSet function error. Wrong value for AA_missiles_bonus_not_split was received.");
                throw 0;
            }

            int old_val = Galaxy->AA_missiles_bonus_not_split;
            Galaxy->AA_missiles_bonus_not_split = val;
            return old_val;
        }
        return Galaxy->AA_missiles_bonus_not_split;
    }

    default: return -1;
    }
}

//����������/�������� �������� ������ ���������� ������� "���� �������"
extern "C" __declspec(dllexport)
uint32_t HullExtraSettings(
    uint32_t obj,
    int setting,
    uint32_t new_val
)
{
    if(ObjectType((void*)obj) == t_ObjShip)
    {
        obj = ShipItems((TShip*)obj, 0);
    }
    else if(ObjectType((void*)obj) != t_ObjItem)
    {
        SFT(L"UtilityFunctions.dll HullExtraSettings Error! Nor 'Ship' or 'Item' object was received!");
        throw 0;
    }
    THull* hull = (THull*)obj;

    switch(setting)
    {
        //������� ����� �������
        case 0:
        {
            if((int)new_val != -1)
            {
                int old_val = hull->current_energy;
                hull->current_energy = new_val;
                return old_val;
            }
            return hull->current_energy;
        }
        //������������ ����� �������
        case 1:
        {
            if((int)new_val != -1)
            {
                int old_val = hull->max_energy;
                hull->max_energy = new_val;
                return old_val;
            }
            return hull->max_energy;
        }
        //���/���� �������������� ���
        case 2:
        {
            if((int)new_val != -1)
            {
                if(new_val < 0 || new_val > 1)
                {
                    SFT(L"UtilityFunctions.dll HullExtraSettings function error. Wrong value for shields_mode was received.");
                    throw 0;
                }

                int old_val = hull->shields_mode;
                hull->shields_mode = new_val;
                return old_val;
            }
            return hull->shields_mode;
        }
        //�������/���������� � ������� ������������ ��
        case 3:
        {
            if((int)new_val != -1)
            {
                if(new_val < 0 || new_val > 1)
                {
                    SFT(L"UtilityFunctions.dll HullExtraSettings function error. Wrong value for got_fighters was received.");
                    throw 0;
                }

                int old_val = hull->got_fighters;
                hull->got_fighters = new_val;
                return old_val;
            }
            return hull->got_fighters;
        }
        //��� ���� ��� ���������� ��� ������������ (0 - ������ �����)
        case 4:
        {
            if((int)new_val != -1)
            {
                if(new_val < 0 || new_val > 6)
                {
                    SFT(L"UtilityFunctions.dll HullExtraSettings function error. Wrong value for fighters_tactic was received.");
                    throw 0;
                }

                int old_val = hull->fighters_tactic;
                hull->fighters_tactic = new_val;
                return old_val;
            }
            return hull->fighters_tactic;
        }
        //����������������� �������� ������������
        case 5:
        {
            if((int)new_val != -1)
            {
                if(new_val < 0 || new_val > 255)
                {
                    SFT(L"UtilityFunctions.dll HullExtraSettings function error. Wrong value for fighters_flight_time was received.");
                    throw 0;
                }

                int old_val = hull->fighters_flight_time;
                hull->fighters_flight_time = new_val;
                return old_val;
            }
            return hull->fighters_flight_time;
        }
        //���� ��� ������������
        case 6:
        {
            if((int)new_val != -1)
            {
                if(new_val != 0 && ObjectType((void*)new_val) != t_ObjShip)
                {
                    SFT(L"UtilityFunctions.dll HullExtraSettings function error. Wrong value for fighters_target was received.");
                    throw 0;
                }

                TShip* old_val = hull->fighters_target;
                hull->fighters_target = (TShip*)new_val;
                return (uint32_t)old_val;
            }
            return (uint32_t)hull->fighters_target;
        }
    }
}


//����������/������������� �������� ���������� rangers_average_capital � Galaxy
extern "C" __declspec(dllexport)
int SetRangersCapital(int new_val, int override_set)
{
    static std::thread* override_thread;

    int rangers_capital_temp = Galaxy->rangers_average_capital;
    if(new_val >= 0)
    {
        rangers_average_capital = new_val;
        Galaxy->rangers_average_capital = rangers_average_capital;

        switch(override_set)
        {
            case -2:
            {
                //���������� ������ ������� ������������ ���������, ���� ��� ����������
                if(override_thread != nullptr) return 1;
                else return 0;
            }
            case -1:
            {
                //������� ��������
                if(override_thread != nullptr)
                {
                    rangers_capital_override = false;
                    override_thread->join();
                    delete override_thread;
                    override_thread = nullptr;
                }
                break;
            }
            case 1:
            {
                //��������� ��������� ����� ��� "���������" ��������, ���� �����
                if(override_thread)
                {
                    rangers_capital_override = false;
                    override_thread->join();
                    delete override_thread;
                }
                rangers_capital_override = true;
                override_thread = new std::thread(RangersCapitalOverride);
                break;
            }
        }

        return rangers_capital_temp;
    }
    else
    {
        switch(override_set)
        {
            case -2:
            {
                //���������� ������ ������� ������������ ���������, ���� ��� ����������
                if(override_thread != nullptr) return 1;
                else return 0;
            }
            case -1:
            {
                //������� ��������
                if(override_thread != nullptr)
                {
                    rangers_capital_override = false;
                    override_thread->join();
                    delete override_thread;
                    override_thread = nullptr;
                }
                break;
            }
            case 1:
            {
                //��������� ��������� ����� ��� "���������" ��������, ���� �����
                if(override_thread)
                {
                    rangers_capital_override = false;
                    override_thread->join();
                    delete override_thread;
                }
                rangers_capital_override = true;
                override_thread = new std::thread(RangersCapitalOverride);
                break;
            }
        }

        return rangers_capital_temp;
    }
}
void RangersCapitalOverride()
{
    while(rangers_capital_override)
    {
        if(Galaxy->galaxy_is_being_destroyed) break;
        if(Galaxy->rangers_average_capital != rangers_average_capital) Galaxy->rangers_average_capital = rangers_average_capital;
        using namespace std::chrono;
        std::this_thread::sleep_for(1ms);
    }
}

//������� ��� �������� ���������� � ������, �������� �������������
extern "C" __declspec(dllexport)
int ParsCountFromString(
    wchar_t* g_par_string,
    wchar_t* g_sep
)
{
    //���� ������ �����, �� ����� ���������� � �����
    if(g_par_string == nullptr) return 0;

    std::wstring par_string = g_par_string;
    std::wstring sep = g_sep;

    int count = 1;

    size_t start = 0;
    size_t end = par_string.find(sep);
    //���� ���������� ������ �� ����� ���������� ������������, ����� ���������� ����� �������
    //(��������� ��� ���� ������������ ��������)
    if(end == std::wstring::npos) return 1;

    //���������� ������, ������� ��� ��������� ����� �������������
    while(true)
    {
        start = end + sep.length();
        end = par_string.find(sep, start);

        //�������� �� ��������� �������� � ������
        if(end == std::wstring::npos) return ++count;

        ++count;
    }
}
//������� ��� �������� ����������� ��������� �� ������, �������� �������������
extern "C" __declspec(dllexport)
const wchar_t* GetParFromString(
    wchar_t* g_par_string,
    wchar_t* g_sep,
    int g_par_num,
    int clear_spaces
)
{
    to_game = GetParFromStringLocal(g_par_string, g_sep, g_par_num, clear_spaces);
    return to_game.c_str();
}

std::wstring GetParFromStringLocal(
    wchar_t* g_par_string,
    wchar_t* g_sep,
    int g_par_num,
    int clear_spaces
)
{
    //���� ������ �����, �� ����� ���������� � �����
    if(g_par_string == nullptr) return L"";
    std::wstring par_string = g_par_string;
    std::wstring sep;
    if(g_sep == nullptr) return par_string.substr(g_par_num, 1);
    else sep = g_sep;
    if(!sep.length()) return par_string.substr(g_par_num, 1);

    int count = abs(g_par_num);
    //if(count < 0) return L"";

    size_t start = 0;
    size_t end = par_string.find(sep);

    //���� ���������� ������ �� ����� ���������� ������������, ����� ���������� � �����
    if(end == std::wstring::npos)
    {
        if(!count)
        {
            //������� �� ���������� ������ ��������� ��� ������� � ��������� � ������ � � �����
            if(clear_spaces)
            {
                if(clear_spaces == 1) trim(par_string);
                //������� �� ���������� ������ ��������� ��� ������� � ��������� � �����
                else
                {
                    par_string.erase(remove(par_string.begin(), par_string.end(), ' '), par_string.end());
                    par_string.erase(remove(par_string.begin(), par_string.end(), '\t'), par_string.end());
                }
            }

            return par_string;
        }
        else return L"";
    }

    //���������� ������, ���� �������� ��� ��������� �������
    while(true)
    {
        //���� �����, �� ��������� ����
        if(!count)
        {
            par_string = par_string.substr(start, end - start);

            break;
        }
        start = end + sep.length();
        end = par_string.find(sep, start);

        //�������� �� ��������� �������� � ������
        if(end == std::wstring::npos)
        {
            end = par_string.length();
            par_string = par_string.substr(start, end - start);
            --count;

            break;
        }

        --count;
    }

    //���������� ��������� �������� � ����
    if(!count)
    {
        if(clear_spaces)
        {
            //������� �� ���������� ������ ��������� ��� ������� � ��������� � ������ � � �����
            if(clear_spaces == 1) trim(par_string);
            //������� �� ���������� ������ ��������� ��� ������� � ��������� � �����
            else
            {
                par_string.erase(remove(par_string.begin(), par_string.end(), ' '), par_string.end());
                par_string.erase(remove(par_string.begin(), par_string.end(), '\t'), par_string.end());
            }
        }

        return par_string;
    }
    //���� �������� �� ��� ������ (��� ������ ������� ������� �����), ���������� ������ ������
    else return L"";
}

//������� ��� ������ ����������� ��������� � ������, �������� �� ����������� (�� �����������)
extern "C" __declspec(dllexport)
const wchar_t* SetParFromString(
    wchar_t* g_par_string,
    wchar_t* g_sep,
    int g_par_num,
    wchar_t* g_par
)
{
    //���� ������ �����, �� ���������� ����� ���������� ��� ������ ��������
    if(g_par_string == nullptr) return g_par;

    std::wstring par_string = g_par_string;

    std::wstring par;
    if(g_par == nullptr) par = L"";
    else par = g_par;
    std::wstring sep;
    if(g_sep == nullptr) sep = L"";
    else sep = g_sep;

    int count = abs(g_par_num);
    if(count < 0) return L"";

    //������ �� ������
    if(g_par_num > par_string.length() - 1)
    {
        to_game = par_string + par;
        return to_game.c_str();
    }
    //���� ��������� ������ �������� ��������� ������ � ������
    if(!sep.length())
    {
        to_game = par_string.substr(0, g_par_num) + par + par_string.substr(g_par_num + 1);
        return to_game.c_str();
    }

    size_t start = 0;
    size_t end = par_string.find(sep);
    //���� ���������� ������ �� ����� ���������� ������������
    //�� ��������� � ��� ���������� �������� (� ������, ���� � �����), � ���������� ����� ������� �����������
    if(end == std::wstring::npos)
    {
        if(!count)
        {
            to_game = par + sep + par_string;
            return to_game.c_str();
        }
        else
        {
            to_game = par_string + sep + par;
            return to_game.c_str();
        }
    }

    //���������� ������, ���� �������� ��� ��������� �������
    while(true)
    {
        //���� �����, �� �������� ����� ������ � ������� ��������� � ��������� ����
        if(!count)
        {
            par_string = par_string.substr(0, start) + par + par_string.substr(end, par_string.length());

            break;
        }
        start = end + sep.length();
        end = par_string.find(sep, start);

        //�������� �� ��������� �������� � ������ (���������� �������� ����� �������� � �����)
        if(end == std::wstring::npos)
        {
            par_string = par_string.substr(0, start) + par;
            //�������� ����� ��������� � ������, �� ������� ���������� �������� ��� � ��� ��������
            --count;

            break;
        }

        --count;
    }

    //���������� ������� ����������� ������� � ����
    to_game = par_string;
    return to_game.c_str();
}

//������� ��� �������� ���� �������� �������� �� ������
extern "C" __declspec(dllexport)
const wchar_t* TrimNumbers(wchar_t *from_game)
{
    //���� ������� ���������� � ���� ������������� wchar_t* ��������,
    //��� ���������������, �� ������-�� ������ � ������ ������� ������������ �����, ��
    //wchar_t* trim_str;
    //trim_str = (wchar_t*)malloc(sizeof(wchar_t) * wcslen(from_game) + 1);
    //���������� ������ ����������� ������ ���� NULL. ����������� �������, ���� ����� �� ������ ���� �������.
    std::wstring trim_str = L"";
    //int i = 0;
    for(; *from_game; ++from_game)
    {
        if(!isdigit(*from_game)) trim_str += *from_game;
    }

    to_game = trim_str;
    return to_game.c_str();
    //return trim_str;
}

//������� ��� �������� �������� �� ������ ��������� � ������
extern "C" __declspec(dllexport)
unsigned int PortionInDiapason(
    unsigned int tcur, //������� ��������
    unsigned int ta, //������ ���������, � ������� ��������� ������� ��������
    unsigned int tb, //����� ���������, � ������� ��������� ������� ��������
    unsigned int tfromA, //����������� �������� ���������, � ������� ����� ��������� ������� ��������
    unsigned int ttoB //������������ �������� ���������, � ������� ����� ��������� ������� ��������
)
{
    double cur = FloatFromGame(tcur);
    double a = FloatFromGame(ta);
    double b = FloatFromGame(tb);
    double fromA = FloatFromGame(tfromA);
    double toB = FloatFromGame(ttoB);

    if(a < b)
    {
        if(cur <= a) return tfromA;
        else if(cur >= b) return ttoB;
        else return FloatToGame((cur - a) / (b - a) * (toB - fromA) + fromA);
    }
    else
    {
        if(cur >= a) return tfromA;
        else if(cur <= b) return ttoB;
        else return FloatToGame((a - cur) / (a - b) * (toB - fromA) + fromA);
    }
}
float PortionInDiapasonLocal(
    float cur, //������� ��������
    float a, //������ ���������, � ������� ��������� ������� ��������
    float b, //����� ���������, � ������� ��������� ������� ��������
    float fromA, //����������� �������� ���������, � ������� ����� ��������� ������� ��������
    float toB //������������ �������� ���������, � ������� ����� ��������� ������� ��������
)
{
    if(a < b)
    {
        if(cur <= a) return fromA;
        else if(cur >= b) return toB;
        else return (cur - a) / (b - a) * (toB - fromA) + fromA;
    }
    else
    {
        if(cur >= a) return fromA;
        else if(cur <= b) return toB;
        else return (a - cur) / (a - b) * (toB - fromA) + fromA;
    }
}

//������� ��� ���������� ������ ����� � ������ ������� ���������� �����
extern "C" __declspec(dllexport)
unsigned int RoundTo(
    unsigned int ta,
    unsigned int tround_to,
    int mark //��������� ����������:
             //0 - ������� ����������
             //< 0 - � ��������
             //1 - � ��������
             //2 - ����������
             //3+ - � ���������� ��������� ��� ������� �������� (������ ��� ����������, �� ��������)
)
{
    double a = FloatFromGame(ta);
    double round_to = FloatFromGame(tround_to);

    if(!mark) a = round_to * floor(a / round_to + 0.5);
    else if(mark < 0) a = round_to * floor(a / round_to);
    else if(mark == 1) a = round_to * ceil(a / round_to);
    else if(mark == 2) a = round_to * BankRound(a / round_to);
    else a = round_to * NearestOddRound(a / round_to);

    return FloatToGame(a);
}

//������� ��� ���������� A � ������� ����� B
extern "C" __declspec(dllexport)
unsigned int Power(
    unsigned int a,
    unsigned int b
)
{
    return FloatToGame(pow(FloatFromGame(a), FloatFromGame(b)));
}

//������� ��� ����������� ��������� ����� ����� ������� �� ������������ ���������
//��������� ���������� ����� �����
extern "C" __declspec(dllexport)
int DistCoords(
    int Xa,
    int Ya,
    int Xb,
    int Yb
)
{
    int distX = Xa - Xb;
    int distY = Ya - Yb;
    return BankRound(sqrt(distX * distX + distY * distY));
}
//��������������� �������, ���������� ����, ����� ������ ���������� �� ������
//���������� ���������� X ��� ���������� �������
int ShipCoordX(TShip* ship)
{
    return ship->pos.x;
}
//���������� ���������� Y ��� ���������� �������
int ShipCoordY(TShip* ship)
{
    return ship->pos.y;
}
//������� � ���������� ���������� ����� ����� ���������
int DistShips(TShip* ship1, TShip* ship2)
{
    return DistCoords(ShipCoordX(ship1), ShipCoordY(ship1), ShipCoordX(ship2), ShipCoordY(ship2));
}
//��������, ��������� �� ������� � ���������� ������
bool ShipInScript(TShip* ship)
{
    if(ship == Player) return true;
    TScriptShip* script_object = ship->script_ship;
    if(script_object) return true;
    return false;
}
TScript* GetScriptByName(const std::wstring& script_name)
{
    if(script_name == L"") return 0;
    std::wstring w_script_name = L"Script.";
    w_script_name += script_name;

    TScript* target_script = 0;
    TList* scripts_list = Galaxy->scripts;
    for(int i = 0; i < scripts_list->count; ++i)
    {
        TScript* script = (TScript*)scripts_list->items[i];
        if(wcscmp(script->name, w_script_name.c_str())) continue; //wcscmp ���������� 0, ���� ������ �����
        target_script = script;
        break;
    }

    return target_script;
}
TScriptGroup* GetScriptGroupByNum(TScript* script, int group_num)
{
    return (TScriptGroup*)script->groups->items[group_num];
}
TScriptGroup* GetScriptGroupByName(TScript* script, const std::wstring& group_name)
{
    if(group_name == L"") return 0;
    std::wstring w_group_name = group_name;

    TScriptGroup* target_group = 0;
    TList* groups_list = script->groups;
    for(int i = 0; i < groups_list->count; ++i)
    {
        TScriptGroup* group = (TScriptGroup*)groups_list->items[i];
        if(wcscmp(group->name, w_group_name.c_str())) continue; //wcscmp ���������� 0, ���� ������ �����
        target_group = group;
        break;
    }

    return target_group;
}
TScriptState* GetScriptStateByName(TScript* script, const std::wstring& state_name)
{
    if(state_name == L"") return 0;
    std::wstring w_state_name = state_name;

    TScriptState* target_state = 0;
    TList* state_list = script->states;
    for(int i = 0; i < state_list->count; ++i)
    {
        TScriptState* state = (TScriptState*)state_list->items[i];
        if(wcscmp(state->name, w_state_name.c_str())) continue; //wcscmp ���������� 0, ���� ������ �����
        target_state = state;
        break;
    }

    return target_state;
}
int GetScriptGroupNum(TScript* script, TScriptGroup* group)
{
    TScriptGroup* target_group = 0;
    TList* groups_list = script->groups;
    for(int i = 0; i < groups_list->count; ++i)
    {
        TScriptGroup* cur_group = (TScriptGroup*)groups_list->items[i];
        if(cur_group != group) continue;
        return i;
    }
    return -1;
}
TScriptState* GetScriptStateByNum(TScript* script, int state_num)
{
    TList* states_list = script->states;
    return (TScriptState*)states_list->items[state_num];
}
bool IsSpecialAgent(TShip* ship)
{
    if(ship == Player) return false;

    TScriptShip* ship_script_object = ship->script_ship;
    if(!ship_script_object) return false;

    TScript* script = ship_script_object->script;
    if(wcscmp(script->name, L"Script.PC_fem_rangers")) return false; //wcscmp() ���������� 0, ���� ������ �����

    TScriptGroup* group = (TScriptGroup*)script->groups->items[ship_script_object->group];
    if(wcscmp(group->name, L"GroupFem")) return false;

    return true;
}
//���������� ��������� ��� ������� (��� ���� ��������� �� ��������� �������� ����� ����� ���������� "")
std::wstring ShipType(TShip* ship)
{
    wchar_t* type = ship->custom_type_name;
    if(type) return type;
    //������� ������ ���� ��� ��������� �������� (�� ��������� ��� "")
    else
    {
        switch(ShipTypeN(ship))
        {
            case(t_Kling): return L"Kling";
            case(t_Ranger):
            {
                if(IsSpecialAgent(ship)) return L"FemRanger";
                return L"Ranger";
            }
            case(t_Transport):
            {
                switch(ShipSubType(ship))
                {
                    case(0): return L"Transport";
                    case(1): return L"Liner";
                    case(2): return L"Diplomat";
                }
            }
            case(t_Pirate): return L"Pirate";
            case(t_Warrior):
            {
                switch(ShipSubType(ship))
                {
                    case(0): return L"Warrior";
                    case(1): return L"WarriorBig";
                }
            }
            case(t_Tranclucator): return L"Tranclucator";
            case(t_RC): return L"RC";
            case(t_PB): return L"PB";
            case(t_WB):
            {
                if(ship == Player->cur_bridge) return L"PlayerBridge";
                else return L"WB";
            }
            case(t_SB): return L"SB";
            case(t_BK): return L"BK";
            case(t_MC): return L"MC";
            case(t_CB): return L"CB";
            case(t_UB):
            {
                SFT(L"UtilityFunctions.dll ShipType function error. Wrong ruin t_UB ShipType determination.");
                throw 0;
            }
        }

        //�� ������ ������
        return L"";
    }
}
//���������� ��������� ������� �������
std::wstring ShipCustomFaction(TShip* ship)
{
    TScriptShip* script_object = ship->script_ship;
    if(!script_object) return L"";

    wchar_t* faction_str = script_object->custom_faction;
    if(faction_str) return faction_str;
    else return L"";
}

//������� ��� ������� ��������� ��������� ���������� (���������� �� ���������� ������� ��������)
extern "C" __declspec(dllexport)
int CustomArtCostCalc(
    int cost //������� �������� ���� ��������� (������������� �������� 1000-3000)
)
{
    cost = Rnd(cost - 500, cost + 500);
    double TL_coef = -4.0 + GalaxyTechLevel(); //����� �� ���������� �� ��������� overflow
    if(TL_coef < 0) TL_coef = 0;
    TL_coef = 0.5 * TL_coef + 1.0;
    double Diff_coef = 0.85 + 0.15 * trunc(GalaxyDiffLevels(1, true));
    cost = BankRound(TL_coef * Diff_coef * cost);
    cost = cost + Rnd(-50, 100);
    if(cost <= 0) cost = 1;

    return cost;
}

//������� ��� ������� ������� ��������� ���������� (���������� �� ���������� ������� ��������)
extern "C" __declspec(dllexport)
int CustomArtSizeCalc(
    int size //������� �������� ������� ���������
)
{
    size = BankRound(0.01 * GalaxyDiffLevels(4) * size) + Rnd(0, 7);
    if(size <= 0) size = 1;
    return size;
}

extern "C" __declspec(dllexport)
int RndObject(int min, int max, void* obj)
{
    //���������� ��� � ������ ��� ��������, � ������� ������� ��� �����
    int* rnd_seed = nullptr;
    if(obj == Galaxy) rnd_seed = &const_cast<TGalaxy*>(Galaxy)->rnd_seed;
    else
    {
        int obj_type = ObjectType(obj);
        switch(obj_type)
        {
            case(t_ObjShip):
            case(t_ObjStation):
            {
                rnd_seed = &((TShip*)obj)->rnd_seed;
                break;
            }
            case(t_ObjPlanet):
            {
                rnd_seed = &((TPlanet*)obj)->rnd_seed;
                break;
            }
            case(t_ObjStar):
            {
                rnd_seed = &((TStar*)obj)->rnd_seed;
                break;
            }
            default:
            {
                SFT(L"UtilityFunctions.dll RndObject Error! Wrong type of object was received!");
                throw 0;
            }
        }
    }

    unsigned int seed;
    //�� ������, ���� �������� �� � ���������� �� �� ������
    if(Galaxy->AA_enabled && Galaxy->AA_rnd_chaotic) seed = rand();
    else
    {
        //������ ����� ����� �����
        seed = *rnd_seed;
        unsigned int prev_seed = seed;
        seed = seed * 7981 + 567 + seed / 7981;
        if(prev_seed == seed) seed = seed * 7281 + 517 + seed / 7181; //�� ������, ���� seed "����������" �� ����� �������� (����� ����, ��� ��������� ��������)
        *rnd_seed = seed; //���������� ����� ����� ������� � ����������
    }

    //����� ���������� �� ���� ����������� ������
    if(min < max) return min + int(seed % (max - min + 1));
    else return max + int(seed % (min - max + 1));
}

extern "C" __declspec(dllexport)
unsigned int RndFloat(unsigned int min, unsigned int max, void* obj)
{
    float temp = 0.0000001f * (float)RndObject(int(10000000.0 * FloatFromGame(min)), int(10000000.0 * FloatFromGame(max)), obj);
    return FloatToGame(temp);
}

//���������� ���������� ����� ��� ���������� ������, ���� ����������/������� ���� � ��������� ������
extern "C" __declspec(dllexport)
uint32_t GetPlanetOrbitProbe(TPlanet* planet, int orbit_num, int probe_get)
{
    if(orbit_num > -1)
    {
        if(orbit_num >= planet->orbit_cnt) return 0;

        //���� � ���������� ����, ������� ��������� �� ��������� ������
        for(int i = 0; i < Player->satellites->count; ++i)
        {
            TSatellite* probe = (TSatellite*)Player->satellites->items[i];
            if(probe->planet == planet && probe->orbit_num == orbit_num)
            {
                //���� ���������� ������ ������ ���� � ������
                if(probe_get)
                {
                    probe->planet = 0;
                    TListDelete(Player->satellites, i);
                    //��������� ����������� � ���������� ������ (���� ��� �������)
                }

                return (uint32_t)probe;
            }
        }

        return 0;
    }

    //����� ������ ���������� ������������ ����� ����� ������� ��� �����
    return planet->orbit_cnt;
}

//��������� ���������/�������� ��������� �������������/������������ �������� �� ��������� ������� (��������� ������ ��� ����������)
extern "C" __declspec(dllexport)
int PlanetItemsHiddency(
    TPlanet* planet,
    int item_num,
    wchar_t* req_type,
    int new_val
    )
{
    if(ObjectType(planet) != t_ObjPlanet)
    {
        SFT(L"UtilityFunctions.dll PlanetItemsHiddency function error. First argument is not a planet.");
        throw 0;
    }

    if(planet->buried_items == nullptr) return 0; //������ ��� ��� ������� ����� ������ ���� ������, ���� � �� ��� ��������� - ������� ����� �����

    //������� �������� �� ����� ���������� �� ��� ���������� �� ������� ���������
    if(item_num < 0)
    {
        int buried_items_cnt = 0;
        for(int i = planet->buried_items->count - 1; i >= 0; --i)
        {
            TGoneItem* buried_item = (TGoneItem*)planet->buried_items->items[i];
            if( (buried_item->terrain_type <= 0 && buried_item->terrain_needed > planet->water_researched) ||
                (buried_item->terrain_type == 1 && buried_item->terrain_needed > planet->plains_researched) ||
                (buried_item->terrain_type > 1 && buried_item->terrain_needed > planet->mountains_researched) ) ++buried_items_cnt;
        }

        return buried_items_cnt;
    }

    if(item_num > planet->buried_items->count - 1)
    {
        SFT(L"UtilityFunctions.dll PlanetItemsHiddency function error. Received item number is out of PlanetItems range.");
        throw 0;
    }

    std::wstring request_type = req_type;
    TGoneItem* buried_item = (TGoneItem*)planet->buried_items->items[item_num];
    int return_val = 0;

    if(request_type == L"IsHidden") //������� �������� �� ��, ��� �� ��� �������� ��������� ������� (����� 1, ���� ������� �� ��� �������)
    {
        if( (buried_item->terrain_type <= 0 && buried_item->terrain_needed > planet->water_researched)    ||
            (buried_item->terrain_type == 1 && buried_item->terrain_needed > planet->plains_researched)   ||
            (buried_item->terrain_type > 1 && buried_item->terrain_needed > planet->mountains_researched) ) return 1;
        return 0;
    }
    else if(request_type == L"TerrainType")
    {
        return_val = buried_item->terrain_type;
        if(new_val > -1) buried_item->terrain_type = min(new_val, 2);
        return return_val;
    }
    else if(request_type == L"TerrainNeeded")
    {
        return_val = buried_item->terrain_needed;
        if(new_val > -1)
        {
            if(buried_item->terrain_type <= 0) buried_item->terrain_needed = min(new_val, planet->water_total);
            else if(buried_item->terrain_type == 1) buried_item->terrain_needed = min(new_val, planet->plains_total);
            else /*if(buried_item->terrain_type == 2)*/ buried_item->terrain_needed = min(new_val, planet->mountains_total);
        }
        return return_val;
    }
    else if(request_type == L"GridPosX")
    {
        return_val = buried_item->pos.x;
        if(new_val > -1) buried_item->pos.x = min(new_val, 13);
        return return_val;
    }
    else if(request_type == L"GridPosY")
    {
        return_val = buried_item->pos.y;
        if(new_val > -1) buried_item->pos.y = min(new_val, 6);
        return return_val;
    }
    else
    {
        SFT(L"UtilityFunctions.dll PlanetItemsHiddency function error. Unknown request type was received.");
        throw 0;
    }

    return return_val;
}

//������� ��� ������� � ����������� ���������� (��� ��� ��������� �������) ������� � ���������� �������
extern "C" __declspec(dllexport)
uint32_t ShipNearbyShips(
    TShip* ship, //�������� �������
    int dist_num, //����� ���������� �� ���� �������, ������� ���������� �������: -1 - ������� ����� ��������, ���������� �� �������� ��������
    int req_type, //��� �������: 0 - ����� ������, != 0 - ������ �� ��� ������������� �������
    int types_set, //��� ����� ��������, ���������� ��� ��������� � ������: -2 - ������ �������, -1 - ������ �������, 0 - ���, 1+ - ���������� ���
    wchar_t* types_str, //����� ��������� ����� �������� ����� �������
    wchar_t* custom_faction //����� ��������� ������� �������� ����� ������� ��� �������� "!" �������������� ��� ������� ��� �������
)
{
    if(dist_num < -1) return 0;
    byte obj_type = ObjectType(ship);
    if(obj_type != t_ObjShip && obj_type != t_ObjStation)
    {
        SFT(L"UtilityFunctions.dll ShipNearbyShips function error. First argument is not a ship or station.");
        throw 0;
    }
    if(ShipInHyperSpace(ship) /*!ShipInNormalSpace(ship) && ShipOrder(ship) != TakeOffOrder*/) return 0;

    //���� ������ �������� ������� � ��������������� ����������� �������
    if(!req_type)
    {
        switch(types_set)
        {
            //���
            case 0: types_set = 16383; break;
            //������ �������
            case -1: types_set = 63; break;
            //������ �������
            case -2: types_set = 16320; break;
        }

        //������� ���������� ���������� ������
        ShipsNearby.clear();
        ShipsNearby.push_back({ ship, 0 }); //��� ������� ������� ������ ����� ������ ��������� �������
        TStar* star = ShipStar(ship);
        for(int i = 0; i < StarShips(star); ++i)
        {
            TShip* cur_ship = StarShips(star, i);
            if(cur_ship == ship || !ShipInNormalSpace(cur_ship)) continue;
            //��������� �������/������� ������������ ����� (��������� ������� ��������� ������)
            if(!(types_set & (1 << ShipTypeN(cur_ship)))) continue;
            //����������� ����� �������������� ������� �� ������, ���� �������� ������ ����/������� ���������� � ������ �������, �������� "Ranger" � "FemRanger"
            if(types_str != nullptr)
            {
                std::wstring types_filt = L",";
                types_filt += types_str;
                types_filt += L",";
                if(types_filt.find(L"," + ShipType(cur_ship) + L",") == std::wstring::npos) continue;
            }
            if(custom_faction != nullptr)
            {
                std::wstring faction_filt = L",";
                faction_filt += custom_faction;
                faction_filt += L",";
                std::wstring faction = L"," + ShipCustomFaction(cur_ship) + L",";
                if(faction == L",," || faction == L",SubFactionFixedStanding,") faction = L",!,";
                if(faction_filt.find(faction) == std::wstring::npos) continue;
            }

            //������� ���������� �������/������� � ������
            ShipsNearby.push_back({ cur_ship, DistShips(ship, cur_ship) });
        }
        //��������� ���������� ������
        if(ShipsNearby.size() > 1) ShipsNearbySort(ShipsNearby, 0, ShipsNearby.size() - 1);
    }

    //���������� �������/������� ��� ��������� ������� (��� ������� ������� ��������� �������� �������)
    if(dist_num == -1) return ShipsNearby.size() - 1;
    if(dist_num > ShipsNearby.size() - 1) return 0;
    return (uint32_t)ShipsNearby[dist_num].Ship;
}

//���������� ��������� ���������� ������, � ������� ��������� ��������� �������
extern "C" __declspec(dllexport)
TStar* ShipPrevStar(TShip* ship)
{
    byte obj_type = ObjectType(ship);
    if(obj_type != t_ObjShip && obj_type != t_ObjStation)
    {
        SFT(L"UtilityFunctions.dll ShipPrevStar function error. Given argument is not a ship or station.");
        throw 0;
    }

    return ship->prev_star;
}

//������ �������� ����������� ������� �� ��� ���� �����
extern "C" __declspec(dllexport)
void SetShipExpByType(TShip* ship, int new_val, int type)
{
    byte obj_type = ObjectType(ship);
    if(obj_type != t_ObjShip && obj_type != t_ObjStation)
    {
        SFT(L"UtilityFunctions.dll SetShipExpByType function error. Given argument is not a ship or station.");
        throw 0;
    }

    if(ship == Player)
    {
        switch(type)
        {
            default: ship->exp_points = new_val; break;
            case 1: Player->exp_points_for_dominators_kills = new_val; break;
            case 2: Player->exp_points_for_pirates_kills = new_val; break;
            case 3: Player->exp_points_for_good_ships_kills = new_val; break;
            case 4: Player->exp_points_for_trade = new_val; break;
        }
    }
    else ship->exp_points = new_val;
}

//����� ��������� ����� ����� ���������� ���������
extern "C" __declspec(dllexport)
void ShipExchangeToShip(TShip* ship1, TShip* ship2, int type)
{
    switch(type)
    {
        case 0:
        {
            //������ ������ �����
            dword pointer = (dword)ship1->items;
            ship1->items = ship2->items;
            ship2->items = (TList*)pointer;

            //������ ���������� �� �������� ����� ������������
            pointer = (dword)ship1->hull;
            ship1->hull = ship2->hull;
            ship2->hull = (THull*)pointer;

            pointer = (dword)ship1->fuel_tank;
            ship1->fuel_tank = ship2->fuel_tank;
            ship2->fuel_tank = (TFuelTanks*)pointer;

            pointer = (dword)ship1->engine;
            ship1->engine = ship2->engine;
            ship2->engine = (TEngine*)pointer;

            pointer = (dword)ship1->radar;
            ship1->radar = ship2->radar;
            ship2->radar = (TRadar*)pointer;

            pointer = (dword)ship1->scanner;
            ship1->scanner = ship2->scanner;
            ship2->scanner = (TScaner*)pointer;

            pointer = (dword)ship1->droid;
            ship1->droid = ship2->droid;
            ship2->droid = (TRepairRobot*)pointer;

            pointer = (dword)ship1->gripper;
            ship1->gripper = ship2->gripper;
            ship2->gripper = (TCargoHook*)pointer;

            pointer = (dword)ship1->shield_generator;
            ship1->shield_generator = ship2->shield_generator;
            ship2->shield_generator = (TDefGenerator*)pointer;

            const static int cnt = sizeof(ship1->weapons) / sizeof(TWeapon*);
            for(int i = 0; i < cnt; i = i + 1)
            {
                pointer = (dword)ship1->weapons[i];
                ship1->weapons[i] = ship2->weapons[i];
                ship2->weapons[i] = (TWeapon*)pointer;
            }

            //������ ������ �����
            pointer = (dword)ship1->arts;
            ship1->arts = ship2->arts;
            ship2->arts = (TList*)pointer;
        }
        case 1:
        {
            TList* specs = ship1->ship_special_bonuses;
            ship1->ship_special_bonuses = ship2->ship_special_bonuses;
            ship2->ship_special_bonuses = specs;
        }
        case 2:
        {
            PTR pointer = ship1->graph_ship;
            ship1->graph_ship = ship2->graph_ship;
            ship2->graph_ship = pointer;

            STR skin = ship1->graph_path;
            ship1->graph_path = ship2->graph_path;
            ship2->graph_path = skin;

            bool cham = ship1->is_chameleon;
            ship1->is_chameleon = ship2->is_chameleon;
            ship2->is_chameleon = cham;
        }
    }
}

unsigned int GetShipScriptData(TShip* ship, int data_num)
{
    if(data_num < 0 || data_num > 3) return 0;
    if(ship == Player) return 0;
    TScriptShip* ship_script_obj = ship->script_ship;
    if(!ship_script_obj) return 0;
    return ship_script_obj->data[data_num];
}

void SetShipScriptData(TShip* ship, int data_num, unsigned int new_val)
{
    if(data_num < 0 || data_num > 3) return;
    if(ship == Player) return;
    TScriptShip* ship_script_obj = ship->script_ship;
    if(!ship_script_obj) return;
    ship_script_obj->data[data_num] = new_val;
}

int GetScriptGroupBaseState(TScriptGroup* group)
{
    return group->state;
}
//���������� ��� �������, ������ � ������ (�� �� ��� ������) ��� ���������� �������
extern "C" __declspec(dllexport)
const wchar_t* GetShipScriptName(
    TShip* ship,
    int to_return
)
{
    if(ship == Player) return L"";
    byte obj_type = ObjectType(ship);
    if(obj_type != t_ObjShip && obj_type != t_ObjStation)
    {
        SFT(L"UtilityFunctions.dll GetShipScriptName function error. First argument is not a ship or station.");
        throw 0;
    }

    TScriptShip* ship_script_obj = ship->script_ship;
    //���� ��������� ������� � �������� ������ � ������
    if(ship_script_obj)
    {
        TScript* script = ship_script_obj->script;
        switch(to_return)
        {
            default /* � ��� ����� 0 */: return script->name + 7; //�������� ��������� ��������� "Script." � ������ ����� ������ �������
            case 1:
            {
                TScriptGroup* group = (TScriptGroup*)script->groups->items[ship_script_obj->group];
                return group->name;
            }
            case 2:
            {
                TScriptState* state = ship_script_obj->state;
                return state->name;
            }
            case 3:
            {
                //�������� ������ �� ����� ������� � ����� ������
                static wchar_t* out = nullptr;
                if(out) delete[] out;
                TScriptGroup* group = (TScriptGroup*)script->groups->items[ship_script_obj->group];
                out = new wchar_t[wcslen(script->name + 7) + wcslen(group->name) + 2];
                wcscpy(out, script->name + 7);
                wcscat(out, L".");
                wcscat(out, group->name);
                return out;
            }
            case 4:
            {
                //�������� ������ ������ �� ����� �������, ����� ������ � ����� ������
                static wchar_t* out = nullptr;
                if(out) delete[] out;
                TScriptGroup* group = (TScriptGroup*)script->groups->items[ship_script_obj->group];
                TScriptState* state = ship_script_obj->state;
                out = new wchar_t[wcslen(script->name + 7) + wcslen(group->name) + wcslen(state->name) + 3];
                wcscpy(out, script->name + 7);
                wcscat(out, L".");
                wcscat(out, group->name);
                wcscat(out, L".");
                wcscat(out, state->name);
                return out;
            }
        }
    }
    else return L"";
}

//��������� ������� (�� �� ������) � ��������� ���������� ������ ������ �������
extern "C" __declspec(dllexport)
void ShipJoinToScript(
    TShip* ship, //�������, ������� ���������� �������� � ���������� ������
    wchar_t* path
)
{
    if(ship == Player) return;
    byte obj_type = ObjectType(ship);
    if(obj_type != t_ObjShip && obj_type != t_ObjStation)
    {
        SFT(L"UtilityFunctions.dll ShipJoinToScript function error. First argument is not a ship or station.");
        throw 0;
    }

    //�������� �� ���������� ����������� � ������� ����� ������� � ������
    wchar_t sep[] = L".";

    TScript* script = GetScriptByName(GetParFromStringLocal(path, sep, 0, 0));
    if(!script)
    {
        std::wstring mess = L"UtilityFunctions.dll Warning! ShipJoinToScript cannot find script by it's name. Path is: ";
        mess += path;
        SFT(mess);
        return;
    }

    TScriptGroup* group = GetScriptGroupByName(script, GetParFromStringLocal(path, sep, 1, 0));
    if(!group)
    {
        std::wstring mess = L"UtilityFunctions.dll Warning! ShipJoinToScript cannot find script group by it's name. Path is: ";
        mess += path;
        SFT(mess);
        return;
    }

    TScriptState* state;
    std::wstring state_name = GetParFromStringLocal(path, sep, 2, 0);
    if(state_name != L"")
    {
        state = GetScriptStateByName(script, state_name);
        if(!state)
        {
            //���� � ���������� ��������� ����� ������ �� ���, �� ������� �������� � ������� ��� ��� ����� ������ �����
            state = GetScriptStateByNum(script, GetScriptGroupBaseState(group));
            std::wstring mess = L"UtilityFunctions.dll Warning! ShipJoinToScript cannot find script state by it's name. Ship will be added in base group state. Path is: ";
            mess += path;
            SFT(mess);
        }
    }
    //���� ����� �� ������, �� ��������� ������� ������� � �����������
    else state = GetScriptStateByNum(script, GetScriptGroupBaseState(group));

    TScriptShip* ship_script_obj = ship->script_ship;
    //���� ������� ��� ������ � ������, �� ������ �������� � �������, ����� ���� �������� ��� ��� ���������� ����������
    TScript* old_script = 0;
    if(ship_script_obj)
    {
        old_script = ship_script_obj->script;
        //���� ������� �� ��������� � ��� �� �������, � ������� ��� ��������� �������
        if(script != old_script)
        {
            TList* ship_script_objs_list = old_script->ships;
            for(int i = 0; i < ship_script_objs_list->count; ++i)
            {
                TScriptShip* cur_obj = (TScriptShip*)ship_script_objs_list->items[i];
                if(cur_obj == ship_script_obj)
                {
                    //WList::Delete(ship_script_objs_list, i);
                    //TList_Delete(ship_script_objs_list, i);
                    TListDelete(ship_script_objs_list, i);
                    break;
                }

                //�� ������, ���� �� ����� ����� ����������� ������� ������� � ������
                if(i == ship_script_objs_list->count - 1)
                {
                    std::wstring mess = L"UtilityFunctions.dll ShipJoinToScript Warning! Can't find ship script object in it's script. Path is: ";
                    mess += path;
                    SFT(mess);
                    return;
                }
            }
        }
        //���� ������� ��������� � ��� �� �������
        else
        {
            //���� ������� ��������� � ��� �� ������ � � ��� �� ������, � ������� ��� ��������� �������
            if(group == GetScriptGroupByNum(script, ship_script_obj->group) && state == ship_script_obj->state)
            {
                std::wstring mess = L"UtilityFunctions.dll ShipJoinToScript Warning! This ship is already in target script group and target script state. Path is: ";
                mess += path;
                SFT(mess);
                return;
            }
        }

        //�������� ��� ���������� � ������� ����������� ������� �������, �� ������ ���� ������� ����������� � ������ ������
        //(��� ������� ����� ������ ��������� �� �����)
        if(group != GetScriptGroupByNum(script, ship_script_obj->group))
        {
            //TScriptShip_Destroy(ship_script_obj, 0); //��� ������ ������� ���������� ������ ������� ��������� ��� �������� �������������
            for(int i = 0; i <= 3; ++i) SetShipScriptData(ship, i, 0);
            ship_script_obj->custom_faction = 0;
            ship_script_obj->end_order = 0;
            ship_script_obj->hit = 0;
            ship_script_obj->hit_player = 0;
        }
    }
    //����� ������ ������ ��� ����� ������� ���������� ������
    else
    {
        int sz = *(int*)(int(vmt_TScriptShip) - 40);
        char* tstr = *(char**)(int(vmt_TScriptShip) - 44);
        //ship_script_obj = TScriptShip_Create((VMT_TScriptShip*)vmt_TScriptShip->methods, 1);
        //ship_script_obj = TScriptShip_New(vmt_TScriptShip);
        ship_script_obj = (TScriptShip*)GetMem(sz);
        if(!ship_script_obj)
        {
            SFT(L"UtilityFunctions.dll ShipJoinToScript function error. No game memory available to create new script object!");
            throw 0;
        }
        *(VMT_TScriptShip**)ship_script_obj = vmt_TScriptShip;
        for(int i = 4; i < sz; ++i) ((char*)ship_script_obj)[i] = 0;
        ship_script_obj->ship = ship;
    }

    //������� ���������� ������ ������� � ������ ������ �������
    //WList::Add(script->ships, (uint32_t)ship_script_obj);
    //TList_Add(script->ships, (uint32_t)ship_script_obj);
    if(script != old_script) TListAdd(script->ships, ship_script_obj);
    ship_script_obj->script = script;
    //���������� ����� ������, � ������� �������� �������
    ship_script_obj->group = GetScriptGroupNum(script, group);
    //������� ������� � �����
    ship_script_obj->state = state;
    ship->script_ship = ship_script_obj;
}

//���������� ������ ��������, ������� ������������� ��������� ������� �������� NoKling ���� NoComeKling
extern "C" __declspec(dllexport)
const wchar_t* GetScriptNoKlingMarksFromStar(
    TStar* star
)
{
    if(!star)
    {
        SFT(L"UtilityFunctions.dll GetScriptNoKlingMarksFromStar function error. Argument is a null.");
        throw 0;
    }
    if(ObjectType(star) != t_ObjStar)
    {
        SFT(L"UtilityFunctions.dll GetScriptNoKlingMarksFromStar function error. Argument is not a star.");
        throw 0;
    }

    std::wstring scripts_list = L"";
    for(int i = 0; i < Galaxy->scripts->count; ++i)
    {
        TScript* script = (TScript*)Galaxy->scripts->items[i];
        for(int j = 0; j < script->script_stars->count; ++j)
        {
            TScriptStar* script_star = (TScriptStar*)script->script_stars->items[j];

            //��� ������� (���������� � UTF-8 ��� ��� � ���� ��� ����� ��������� ����� ����������)
            //SFT(convert.to_bytes(script_star->star->name));
            //SFT(convert.to_bytes(script_star->script_star_name));

            if(script_star->star != star) continue;

            int mark_type = 0;
            if(script_star->no_kling) mark_type += 1;
            if(script_star->no_come_kling) mark_type += 2;

            if(!mark_type) continue;

            if(!scripts_list.empty()) scripts_list += L",";
            scripts_list += (script->name + 7);
            scripts_list += L"." + std::to_wstring(mark_type);
        }
    }

    if(scripts_list != L"")
    {
        to_game = scripts_list;
        return to_game.c_str();
    }
    else return L"";
}

extern "C" __declspec(dllexport)
unsigned int GetShipPath(TShip* ship, wchar_t* to_return)
{
    byte obj_type = ObjectType(ship);
    if(obj_type != t_ObjShip && obj_type != t_ObjStation)
    {
        SFT(L"UtilityFunctions.dll GetShipPath function error. First argument is not a ship or station.");
        throw 0;
    }

    //���� last_point ����� nullptr, ������, ������� ����� �� ����� � ������ �� ��������
    TSPathUnit* end_point = ship->head_path_obj->last_point;
    if(!wcscmp(to_return, L"EndCoordX")) if(end_point) return FloatToGame(end_point->pos.x); else return FloatToGame(ship->pos.x);
    else if(!wcscmp(to_return, L"EndCoordY")) if(end_point) return FloatToGame(end_point->pos.y); else return FloatToGame(ship->pos.y);
    else /* if(!wcscmp(to_return, L"EndAngle")) */ if(end_point) return FloatToGame(end_point->angle); else return FloatToGame((float)ShipAngle(ship));
    return FloatToGame(0.0f);
}

extern "C" __declspec(dllexport)
void SetShipPath(TShip* ship, int coordX, int coordY)
{
    byte obj_type = ObjectType(ship);
    if(obj_type != t_ObjShip && obj_type != t_ObjStation)
    {
        SFT(L"UtilityFunctions.dll SetShipPath function error. First argument is not a ship or station.");
        throw 0;
    }

    //���� �����-�� ���� ��� ��������� �����
    dword points_count = ship->head_path_obj->points_count;
    if(points_count)
    {
        TSPathUnit* cur_point = ship->head_path_obj->last_point;
        float offsetX = coordX - cur_point->pos.x;
        float offsetY = coordY - cur_point->pos.y;
        float deltaX = offsetX / (points_count - 1);
        float deltaY = offsetY / (points_count - 1);

        while(cur_point)
        {
            cur_point->pos.x += offsetX;
            cur_point->pos.y += offsetY;
            offsetX -= deltaX;
            offsetY -= deltaY;
            //������� ����, ��� ������ ���� ���������� ����� � �������� - ��� "�����������"
            if(cur_point->next_point) cur_point->angle = AngleBetweenShipCoords(cur_point->next_point->pos.x, cur_point->next_point->pos.y, cur_point->pos.x, cur_point->pos.y) * 180.0f / M_PI;

            cur_point = cur_point->prev_point;
        }

        //����� �� ���� ����������� ����� ��������� ������� � ������������� � ��������� �����
        if(points_count > 1)
        {
            cur_point = ship->head_path_obj->last_point;
            cur_point->angle = cur_point->prev_point->angle;
        }
    }
}

//������� ��� ���������/��������� ���������� ������� ����� �������� �������
extern "C" __declspec(dllexport)
dword ShipVisibility(
    TShip* ship,
    int type,
    dword new_val
)
{
    byte obj_type = ObjectType(ship);
    if(obj_type != t_ObjShip && obj_type != t_ObjStation)
    {
        SFT(L"UtilityFunctions.dll ShipVisibility function error. First argument is not a ship or station.");
        throw 0;
    }

    switch(type)
    {
        default: return FloatToGame(ship->anim_alpha); //������ ������� �����
        case 1: return FloatToGame(ship->anim_alpha_delta); //������ ������� ������ �����
        case -2: //����������� ������� �����
        {
            ship->anim_alpha = (float)FloatFromGame(new_val);
            ship->anim_alpha_delta = 0.00001; //��� ������������� ���������� ������� ����� �� ����� ������, �������� ��������� ��������� ������
            
            //����� ����� ��������� ���������� �������� ����� ��������������� � ����������� ������� �������, ����� ��������� ������������ � ���� ���������
            //(�� ����� ������, ���� �� ����� �����)
            //if(*ship->graph_path == L'S') ((TShip2SE*)ship->graph_ship)->graph_alpha = (byte)FloatFromGame(new_val);
            //else ((TRuinsSE*)ship->graph_ship)->graph_alpha = (byte)FloatFromGame(new_val);
            return 0;
        }
        case -1: //����������� ������� ������ �����
        {
            ship->anim_alpha_delta = (float)FloatFromGame(new_val);
            return 0;
        }
    }

    return 0;
}

//������� ��� ���������/��������� ����� ����������
extern "C" __declspec(dllexport)
int ShipSubrace(
    TShip* ship,
    int sub_race
)
{
    byte obj_type = ObjectType(ship);
    if(obj_type != t_ObjShip && obj_type != t_ObjStation)
    {
        SFT(L"UtilityFunctions.dll ShipSubrace function error. First argument is not a ship or station.");
        throw 0;
    }

    if(ShipTypeN(ship) != t_Kling)
    {
        //SFT(L"UtilityFunctions.dll ShipSubrace function warning. First argument is not a dominator!");
        return -1;
    }

    if(sub_race <= -1) return ((TKling*)ship)->sub_race;
    int old_sub_race = ((TKling*)ship)->sub_race;
    ((TKling*)ship)->sub_race = sub_race;
    return old_sub_race;
}

//���� � ��������� �� �� ���������� Id � ��� ������ ���������� �
extern "C" __declspec(dllexport)
uint32_t IdToHole(
    int id
)
{
    if(id <= 0)
    {
        SFT(L"UtilityFunctions.dll IdToHole function error. Invalid Id number was received.");
        throw 0;
    }

    for(int i = 0; i < Galaxy->holes->count; ++i)
    {
        THole* hole = (THole*)Galaxy->holes->items[i];
        if(hole->id == id) return (uint32_t)hole;
    }

    return 0;
}

extern "C" __declspec(dllexport)
int HoleStatus(
    THole* hole,
    int status
)
{
    if(status == -1) return hole->status;
    else hole->status = status;
}

extern "C" __declspec(dllexport)
void AdjustRuinsGoodsPricesToStorage(
    int min_cnt,
    int max_cnt,
    unsigned int t_highest,
    unsigned int t_lowest
)
{
    float highest = (float)FloatFromGame(t_highest);
    float lowest = (float)FloatFromGame(t_lowest);

    for(int i = 0; i < StorageItems(); ++i)
    {
        TItem* item = StorageItems(i);

        int goods_type = ItemType(item);
        if(goods_type > t_Narcotics) continue;

        uint32_t place = StorageItemLocation(i);
        if(ObjectType((void*)place) != t_ObjStation) continue;

        int target_price = PortionInDiapasonLocal((float)ItemSize(item), (float)min_cnt, (float)max_cnt, highest, lowest) * BankRound((float)ItemCost(item) / (float)ItemSize(item));
        if(GoodsBuyPrice(place, goods_type) <= target_price) continue;
        GoodsBuyPrice(place, goods_type, target_price);
    }
}

//������� ��� ������� ������������ �������� �� txt-�������
extern "C" __declspec(dllexport)
const wchar_t* GetParFromTxt(
    wchar_t* txt_path,
    wchar_t* s_param_name
)
{
    return GetParFromTxtT(txt_path, s_param_name);
}

const wchar_t* GetParFromTxtT(
    wchar_t* txt_path,
    wchar_t* s_param_name
)
{
    //������� ����������
    std::wstring param_name;
    std::wstring cur_line;
    if(s_param_name == nullptr) param_name = L"";
    else param_name = s_param_name;

    //���������� ���� �� ����� "��� ���������"
    //��� ������������� �������� �������������� ����������, � �������������� ��������� ������� ����������
    //���� ������� ������������� �������������� ��������� ������ Uni�ode
    std::wstring path = system_folder;
    path += L"\\";
    path += txt_path;
    //��������� ����
    std::wifstream file(path, std::ios::in);
    //���� ���������� txt-���� �� ���������� ����
    if(file)
    {
        //���������� ������� � �����, ���� ������ ��� � ������� ������
        while(getline(file, cur_line))
        {
            int sep = cur_line.find('=');
            if(cur_line.substr(0, sep) == param_name)
            {
                cur_line = cur_line.substr(sep + 1);
                to_game = cur_line;
                //���������� ������� ���������� ��������, ���� ����� ������ ������� � �����
                return to_game.c_str();
            }
        }

        //��������� ����
        file.close();
    }

    //���� ����� ���, ���� �� ����� � �� ������ ������
    return L"";
}

//������� ��� ������ ������������ �������� � txt-������
extern "C" __declspec(dllexport)
const wchar_t* SetParFromTxt(
    wchar_t* txt_path,
    wchar_t* s_param_name,
    wchar_t* value
)
{
    return SetParFromTxtT(txt_path, s_param_name, value);
}

const wchar_t* SetParFromTxtT(
    wchar_t* txt_path,
    wchar_t* s_param_name,
    wchar_t* value
)
{
    //��������� ����������
    std::wstring param_name;
    std::wstring cur_line;
    std::wstring add_line;
    if(s_param_name == nullptr) param_name = L"";
    else param_name = s_param_name;

    //���������� ���� �� ����� "��� ���������"
    //��� ������������� �������� �������������� ����������, � �������������� ��������� ������� ����������
    //���� ������� ������������� �������������� ��������� ������ Uni�ode
    std::wstring path = system_folder;
    path += L"\\";
    path += txt_path;
    std::wifstream file(path, std::ios::in);
    std::wstring str_value = value;
    std::wstring old_value = L"";

    //���� ���������� txt-���� �� ���������� ����
    if(file)
    {
        //���������� ������� � �����, ���������� �� � ������ ��� ����������� ������
        int check = 0;
        while(getline(file, cur_line))
        {
            int sep = cur_line.find('=');
            //���� ����� ������, � ������� ��� ����� �������� ��������, ���������� � � ������ ���������� ��� � ����� ���������
            if(cur_line.substr(0, sep) == param_name)
            {
                //������ ���������� ������ �������� ��������� ��� �������� �������
                old_value = cur_line.substr(sep + 1);
                to_game = old_value;

                if(!add_line.empty()) add_line += L"\n";
                add_line += param_name + L"=" + str_value;
                check = 1;
            }
            //����� ������ ���������� ������ � ������ ���������� ��� ����
            else
            {
                if(!add_line.empty()) add_line += L"\n";
                add_line += cur_line;
            }
        }
        //���� ������� ������ � ����� �� ���������, �� ��������� � � ����� �����
        if(!check)
        {
            if(add_line != L"") add_line += L"\n";
            add_line += param_name + L"=" + str_value;
        }
        //��������� ����
        file.close();
    }
    //���� �� ����� ������� �����, �� ������������ ������� � ������������� txt ����� ������ ������� ��������
    else add_line = param_name + L"=" + str_value;
    //��������� / ������ ����
    std::wofstream file_add(path, std::ios::out);
    //���������� ������� ������ ���������� � �������� ��� ������ txt
    file_add << add_line;
    //��������� ����, �������� � ������ ������
    file_add.close();

    if(old_value != L"") return to_game.c_str();
    else return L"";
}

//������� ��� �������� �������������� ������� ������ Shift+Ctrl
extern "C" __declspec(dllexport)
int IsShiftCtrlPressed()
{
    if((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
    {
        return 1;
    }
    return 0;
}

//������� ��� ������������ ���� float ����� ������� � � �������� ������
extern "C" __declspec(dllexport)
const wchar_t* FloatToString(unsigned int game_float, int decimal_places)
{
    std::wstring float_str = std::format(L"{:.{}f}", FloatFromGame(game_float), max(decimal_places, 1));
    to_game = float_str;
    return to_game.c_str();
}

//������� ��� ��������� ��������� �������� � ����� ������
extern "C" __declspec(dllexport)
void SendStringToClipboard(wchar_t* from_game)
{
    CString to_clipboard = from_game;
    if(OpenClipboard(GetActiveWindow())) //��������� ����� ������
    {
        HGLOBAL hgBuffer;
        char* chBuffer;
        EmptyClipboard(); //������� �����
        hgBuffer = GlobalAlloc(GMEM_DDESHARE, to_clipboard.GetLength() + 1); //�������� ������
        chBuffer = (char*)GlobalLock(hgBuffer); //��������� ������
        strcpy_s(chBuffer, to_clipboard.GetLength() + 1, LPCSTR(to_clipboard));
        GlobalUnlock(hgBuffer); //������������ ������
        SetClipboardData(CF_TEXT, hgBuffer); //�������� ����� � ����� ������
        CloseClipboard(); //��������� ����� ������
    }
}

//������� ��� ������� ��������� �������� �� ������ ������
extern "C" __declspec(dllexport)
const wchar_t* GetStringFromClipboard()
{
    wchar_t* data = nullptr;
    if(OpenClipboard(GetActiveWindow())) //��������� ����� ������
    {
        HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT); //��������� ����� �� ������ ������
        data = (wchar_t*)GlobalLock(hClipboardData); //��������� ������
        GlobalUnlock(hClipboardData); //������������ ������
        CloseClipboard(); //��������� ����� ������
    }

    return data;
}

//������� �� ������� �������� ��� �������� ������
extern "C" __declspec(dllexport)
int OpenExternalLink(wchar_t* t_ext_link)
{
    CString ext_link = t_ext_link;
    if(32 >= (int)ShellExecute(NULL, "open", ext_link, NULL, NULL, SW_SHOWNORMAL)) return 0;
    return 1;
}