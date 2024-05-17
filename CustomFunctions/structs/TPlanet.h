/** @file */
struct TPlanet {
    __cls* cls;

    unsigned int id;
    int gen_seed;
    int rnd_seed;
    int graph_no;
    STR name;
    TStar* star;
	unsigned int polar_pos_padding; //Филлерный отступ перед record
    _pair_double polar_pos; //polar_pos.x - angle, polar_pos.y - radius
    _gap _030;
    _gap _031;
    _gap _032;
    _gap _033;
    _gap _034;
    _gap _035;
    _gap _036;
    _gap _037;
    int radius;
    double angle_speed;
	unsigned int invention_levels_padding; //Филлерный отступ перед record
	byte invention_levels[20];
    byte cur_invention;
    _gap _05B;
    _gap _05D;
    _gap _05E;
    float cur_invention_points;
    _gap _063;
    _gap _064;
    _gap _065;
    _gap _066;
    int population;
    byte eco;
    _gap _067;
    _gap _06D;
    _gap _06E;
    _gap _06F;
    _gap _070;
    _gap _071;
    _gap _072;
    byte owner;
    bool _075;
    byte race;
    byte gov;
    TGoodsShopItem goods_shop[8];
    _gap _073;
    _gap_32 _078;
    _gap _07C;
    _gap _07D;
    _gap _07E;
    _gap _07F;
    _gap _100;
    _gap _101;
    _gap _102;
    _gap _103;
    _gap _104;
    _gap _105;
    _gap _106;
    _gap _107;
    _gap _108;
    _gap _109;
    _gap _110;
    TList* relation_to_rangers;
    TList* equipment_store_items; //когда игрок садится на планету, данный список обнуляется, а его содержимое перетаскивается в глобальный список закупки (после взлёта игрока он заполняется снова)
    TList* warriors;
    int spawned_rangers_count; //число созданных планетой рейнджеров, необходимое для контроля рождаемости
    _gap _124;
    _gap _125;
    _gap _126;
    _gap _127;
    int water_total; //Общая водная площадь планеты
    int water_researched; //Исследованная водная площадь планеты
    int plains_total; //Общая равнинная площадь планеты
    int plains_researched; //Исследованная равнинная площадь планеты
    int mountains_total; //Общая гористая площадь планеты
    int mountains_researched; //Исследованная гористая площадь планеты
	char orbit_cnt; //Максимальное число орбит для размещения зондов
	bool is_visited; //Получает true, когда игрок впервые посещает эту планету
    _gap empty_align_1;
    _gap empty_align_2;
    TList* buried_items; //Список закопанных на планете предметов в структурах TGoneItem (при генерации заполняется только для незаселёнок)
	int graph_radius;
    TObjectSE* graph_object;
    WSTR graph_name;
    _gap _158;
    _gap _159;
    _gap _15A;
    _gap _15B;
    _gap_32 _15C;
    _gap_32 _15C_;
    _gap_32 _164; // PTR graph;
    byte no_landing;
    byte no_shop_update;
    bool is_this_rogeria; //задаёт логику Роджерии для данной планеты (очевидно, в ваниле имеет true только у Роджерии), также последняя планеты в сейве с данным флагом автоматически выставит на себя глобальный указатель Роджерии
    _gap empty;
};
