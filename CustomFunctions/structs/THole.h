/** @file */
struct THole {
    __cls* cls;

    int id;
    TStar* star_1;
    _pair_float pos_1;
    TStar* star_2;
    _pair_float pos_2;

    int open_turn;
    int status;
    // 1 - обычная ЧД
    // 2 - келлерская ЧД
    // 3 - ЧД готовится к закрытию
    // 4 - КЧД новая?
    PTR graph;
    _gap _2C;
    _gap _2D;
    _gap _2E;
    _gap _2F;
    STR _30;
};