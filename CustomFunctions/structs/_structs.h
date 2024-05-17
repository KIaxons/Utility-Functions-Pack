/** @file */
struct TPoint {
    int x;
    int y;
};

struct TRect {
    int x_1;
    int y_1;
    int x_2;
    int y_2;
};

struct TStarDist {
    int dist;
    TStar* star;
};


struct TPlanetTempl {
    __cls* cls;
    int type;
    STR mask_0;
    STR light_0;
    STR mask_1;
    STR light_1;
    int _14;
};


struct TShipGoodsItem {
    int cnt;
    int cost;
    int _8;
    int _C;
};


struct TGoodsShopItem { //Тип товара определяется положением структуры в массиве
    int goods_count;
    float cost_real; //Вроде как используется во время распродаж, но хз
    int sell_price; //Цена, за которую планета или станция продаёт данный товар
    int buy_price; //Цена, за которую планета или станция покупает данный товар
};


struct TGoneItem {
    _pair_byte pos; //Номер ячейки на карте планеты, в которой находится предмет (в пределах 0-13 по X и 0-6 по Y)
    byte terrain_type; //Тип местности планеты, на которой закопан предмет
    _gap empty_align_gap;
    int terrain_needed; //Какая площадь местности должна быть раскрыта, чтобы предмет можно было собрать
    TItem* item; //Указатель на предмет
};


struct TDomResearchProgress {
    float progress;
    int material;
};


struct TGalaxyEvent {
    __cls* cls;

    int type;
    int turn;
    TList* data_list;
    TList* text_list;
};


struct TSetItem {
    int a;
    int b;
    word c;
};


struct TMessagePlayer {
    __cls* cls;

    _gap_32 _004;
    _gap_32 _008;
    _gap_32 _00C;
    _gap_32 _010;
    _gap_32 _014;
    _gap_32 _018;
    _gap_32 _01C;
    _gap_32 _020;
    _gap_32 _024;
    _gap_32 _028;
    _gap_32 _02C;
    _gap_32 _030;
    _gap_32 _034;
    _gap_32 _038;
    _gap_32 _03C;
};

// struct TDemo {
//     __cls* cls;
//     TBufEC* buf_p;
// };


struct TStorageUnit {
    PTR place;
    int item_slot_num;
    TItem* item;
    // _gap_32 _00C;
    // _gap_32 _010;
    // _gap_32 _014;
    // _gap_32 _018;
    // _gap_32 _01C;
};


struct TQuestGameContent {};
struct TTextField {};
struct TQuestParameter {};
struct TArrayRectGR {};
struct TShopSlot {};

//Предмет в хранилище Vault
struct TStoredItem {
	
};

struct TPlayerHoldUnit {};
struct TJournalRecord {}; // 0x0c


struct TPlanetNews {
    int _00;
    int date;
    byte type;
    _gap _09;
    _gap _0A;
    _gap _0B;
    STR text;
};

struct TEther {
    __cls* cls;

    _gap_32 _004;
    _gap_32 _008;
    _gap_32 _00C;
    _gap_32 _010;
    _gap_32 _014;
};

struct TSPath { //Указатель на этот головной объект записан в классе TShip
    __cls* cls;

    TSPathUnit* first_point;
    TSPathUnit* last_point;
    TSPathUnit* first_empty_point; //Обе эти пустые точки находятся в конце пути, сразу после last_point, как и возможные дополнительные пустые точки между ними (таким образом Даб резервировал место под возможное удлинение пути)
    TSPathUnit* last_empty_point;
    int points_count; //Число валидных (не empty) маршрутных точек в текущем пути
};

struct TSPathUnit {
    TSPathUnit* prev_point;
    TSPathUnit* next_point;
	_pair_float pos;   //Координаты текущей маршрутной точки
	float 		angle; //Направление носа корабля (В ГРАДУСАХ, КАРЛ, В ГРАДУСАХ, БЛЯТЬ!!!), в момент прохождения его по данной точке (по логике должно указывать на следующую точку маршрута)
};

struct TSpecialBonuses {
    char type; //В оригинале тут используется один Enum бонусов
	_gap offset1;
	_gap offset2;
	_gap offset3;
    int value;
};

/*

TEther - 18
TEtherUnit - 14

TSputnik - 10




TObjectGI - 11c
    TPSWeaponGI - 130
        TPSWeapon01GI - 14c
        TPSWeapon06GI - 144
        TPSWeapon08GI - 150
        TPSWeapon09GI - 160
        TPSWeapon09BranchGI - 148
        TPSWeapon13GI - 150
        TPSRocketGI - 158
        TPSLBoltGI - 168
        TPSPDWeaponGI - 144
        TPSAcidWeaponGI - 170
        TPSRadEffectGI - 16c
        TPSRayGI - 164
        TPSRailRayGI - 2bc
            TPSBlueWhirlGI - 2c0
            TPSGreenWhirlGI - 2c0
        TPSLaserCannonRayGI - 178
        TPSEyesGI - 15c
        TPSWindGI - 16c
        TPSPhaserGI - 168

*/