/** @file */
struct TItem {
    __cls* cls;

    UNK _04; // появляется в космосе
    int id;
    byte type;
    _gap _0D;
    _gap _0E;
    _gap _0F;
    _pair_float pos;
    int size;
    byte race;
    _gap _1D;
    _gap _1E;
    _gap _1F;

    int cost;
    _gap_32 _24; // связь со скриптовыми предметами?
    _gap _28;
    _gap _29;
    _gap _2A;
    _gap _2B;
    PTR script_item;
    _gap _30;
    _gap _31;
    _gap _32;
    _gap _33;
    byte no_drop;
    _gap _35;
    _gap _36;
    _gap _37;
};

struct TGoods: public TItem {
    _gap _38;
    _gap _39;
    _gap _3A;
    _gap _3B;
    byte natural;
    _gap _3D;
    _gap _3E;
    _gap _3F;
};

struct TEquipment: public TItem {
	WSTR sys_name;
	WSTR custom_faction;
	double durability;
	bool is_broken;
	bool explotable;
	_gap _empty1; //Пустые гапы нужны здесь только для выравнивания
	_gap _empty2;
	int equip_slot_num; //Актуально только для оружия и артефактов
	int mm; //Микромодуль (здесь нумерация бонусов начинается не с 0, а с 1)
	int acryn; //Основной акрин (здесь нумерация бонусов начинается не с 0, а с 1)
	TList* extra_acryns; //Экстраакрины (и здесь нумерация бонусов также начинается не с 0, а с 1)
	byte sub_race; //Доминаторская серия
	byte detail_improvement; //Определяет, на какой стат получит упор данное оборудование при улучшении на НБ (дальность, скорость и т.д.), но по факту оно здесь бесполезно, т.к. перед наложением улучшения оно всё равно реролится кодом
	_gap _empty3; 
	_gap _empty4;
};

struct THull: public TEquipment {
    int hitpoints;
    byte tech_level;
    byte armor;
    byte hull_type;
    _gap _73;
    int serie;
    TShip* ship;
    byte bridge; //Номер установленного для данного корпуса мостика (0 - без мостика, 1 - мостик ТИ)
    bool made_in_pirate_clan; //Флаг постройки корпуса пиратским кланом (определяет тип иконки расы для корпуса)
    bool shields_mode; //Вкл/выкл режима щитов для корпуса ТИ
    bool got_fighters; //Флаг наличия в корпусе истребителей ТИ (автоматически выставляется при добавлении мостика ТИ функцией CapitalShipStats())
    unsigned int current_energy; //Текущий запас энергии для корпуса ТИ
    unsigned int max_energy; //Максимальный запас энергии для корпуса ТИ
    TShip* fighters_target; //Выбранная цель истребителей для корпуса ТИ
    byte fighters_tactic; //Выбранная тактика истребителей для корпуса ТИ
    byte fighters_flight_time; //Выбранная продолжительность атаки (в днях) истребителей для корпуса ТИ
    _gap _8E;
    _gap _8F;
    _gap _90;
    _gap _91;
    _gap _92;
    _gap _93;
};

struct TFuelTanks: public TEquipment {
    byte tech_level;
    _gap _6D;
    _gap _6E;
    _gap _6F;
    int fuel;
    byte capacity;
    _gap _75;
    _gap _76;
    _gap _77;
    _gap _78;
    _gap _79;
    _gap _7A;
    _gap _7B;
};

struct TEngine: public TEquipment {
    byte tech_level;
    _gap _6D;
    _gap _6E;
    _gap _6F;
    int speed;
    byte parsec;
    _gap _75;
    _gap _76;
    _gap _77;
    _gap _78;
    _gap _79;
    _gap _7A;
    _gap _7B;
};

struct TRadar: public TEquipment {
    byte tech_level;
    _gap _6D;
    _gap _6E;
    _gap _6F;
    int radius;
    _gap _74;
    _gap _75;
    _gap _76;
    _gap _77;
};

struct TScaner: public TEquipment {
    byte tech_level;
    byte power;
    _gap _6E;
    _gap _6F;
    _gap _70;
    _gap _71;
    _gap _72;
    _gap _73;
};

struct TRepairRobot: public TEquipment {
    byte tech_level;
    byte recover_hit_points;
    _gap _6E;
    _gap _6F;
    _gap _70;
    _gap _71;
    _gap _72;
    _gap _73;
};

struct TCargoHook: public TEquipment {
    byte tech_level;
    _gap _6D;
    _gap _6E;
    _gap _6F;
    int pick_up_size;
    int radius;
    _gap _78;
    _gap _79;
    _gap _7A;
    _gap _7B;
    float speed_min;
    float speed_max;
};

struct TDefGenerator: public TEquipment {
    byte tech_level;
    _gap _6D;
    _gap _6E;
    _gap _6F;
    float def_factor;
    _gap _74;
    _gap _75;
    _gap _76;
    _gap _77;
};

struct TWeapon: public TEquipment {
    byte tech_level;
    _gap _6D;
    _gap _6E;
    _gap _6F;
    int radius;
    int min_dmg;
    int max_dmg;
    _gap_32 _7C;
    _gap _80;
    _gap _81;
    _gap _82;
    _gap _83;
    _gap_32 _84;
    _gap_32 _88;
};

struct TCountableItem: public TEquipment {
    _gap _6C;
    _gap _6D;
    _gap _6E;
    _gap _6F;
    _gap _70;
    _gap _71;
    _gap _72;
    _gap _73;
    _gap _74;
    _gap _75;
    _gap _76;
    _gap _77;
};

struct TEquipmentWithActCode: public TEquipment {
    _gap _6C;
    _gap _6D;
    _gap _6E;
    _gap _6F;
    _gap _70;
    _gap _71;
    _gap _72;
    _gap _73;
    int data_1;
};

struct TCistern: public TEquipment {
    int capacity;
    int fuel;
    _gap _74;
    _gap _75;
    _gap _76;
    _gap _77;
};

struct TSatellite: public TEquipment {
	byte probe_type; //Основной тип зонда
    _gap _6D;
    _gap _6E;
    _gap _6F;
    TPlanet* planet; //Получает значение, только если зонд запущен на орбиту какой-то планеты
	int orbit_num; //При снятии зонда с орбиты эта переменная не обновляется, нумерация начинается с 0
    _gap _78;
    _gap _79;
    _gap _7A;
    _gap _7B;
    byte water_speed;
    byte land_speed;
    byte hill_speed;
    _gap _7F;
};

struct TTreasureMap: public TEquipment {
    _gap _6C;
    _gap _6D;
    _gap _6E;
    _gap _6F;
    _gap _70;
    _gap _71;
    _gap _72;
    _gap _73;
    _gap _74;
    _gap _75;
    _gap _76;
    _gap _77;
    _gap _78;
    _gap _79;
    _gap _7A;
    _gap _7B;
    _gap _7C;
    _gap _7D;
    _gap _7E;
    _gap _7F;
};

struct TMicromodule: public TEquipment {
    _gap _6C;
    _gap _6D;
    _gap _6E;
    _gap _6F;
};

struct TArtefact: public TEquipmentWithActCode {};

struct TUselessItem: public TEquipmentWithActCode {
    WSTR custom_text;
    int data[3];
};

struct TProtoplasm: public TCountableItem {};

struct TArtefactTransmitter: public TArtefact {
    int power;
};

struct TArtefactTranclucator: public TArtefact {
    _gap _78;
    _gap _79;
    _gap _7A;
    _gap _7B;
};

struct TArtefactCustom: public TArtefact {
    int data_2;
    int data_3;
    STR text_data_1;
    STR text_data_2;
    STR text_data_3;
    _gap _8C;
    _gap _8D;
    _gap _8E;
    _gap _8F;
    _gap _90;
    _gap _91;
    _gap _92;
    _gap _93;
};

struct TCustomWeapon: public TWeapon {
	uint32_t custom_weapon_info; //структура TWeaponInfo
};
