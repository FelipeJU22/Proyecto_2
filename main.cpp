#include <iostream>
#include <raylib.h>
#include <boost/asio.hpp>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <queue>

using namespace std;

//using boost::asio::ip::tcp;



/**
 * Se setean las distintas ventanas que se van a utilizar en el juego
 */
typedef enum GameScreen {INICIO, SELECCION, NIVEL1, NIVEL2, NIVEL3, NIVEL4, GANAR, PERDER} GameScreen;
/**
 * Estructura utilizada para el jugador, se le da velocidad
 */
typedef struct Jugador {
    Vector2 velocidad;
}Jugador;
/**
 * Estructura utilizada para el enemigo, se le da velocidad y posición
 */
typedef struct Enemigos{
    Vector2 velene;
    int posEnex;
    int posEney;
    int mEnex;
    int mEney;
}Enemigos;
/**
 * Estructura utilizada para las pastillas y para las super-pastillas, se le da velocidad
 */
typedef struct Pastillas{
    Rectangle rec;
    bool active;
    int numero;
    int MxSupP;
    int MySupP;
}Pastillas;
// Delay que va a haber entre cada movimiento del enemigo
static float delayTimerMax = 15.f;
static float delayTimer = 0.f;

static float delayTimerMax2 = 15.f;
static float delayTimer2 = 0.f;


/**
 * Varibles generales del juego
 */
static float pastillaDelayMax = 220.f;
static float pastillaDelay = 0.f;
static float spawnTimerMax = 350.f;
static float spawnTimer = 0.f;
static float spawnTimerMax2 = 350.f;
static float spawnTimer2 = 0.f;
static float spawnTimerMax3 = 350.f;
static float spawnTimer3 = 0.f;
static float spawnTimerMax4 = 350.f;
static float spawnTimer4 = 0.f;
static bool nivel1 = true;
static bool nivel2 = false;

static int cont1 = 0;
static int random_number;
static int random_number2;
static int random_number3;

const int ROWS = 12;
const int COLS = 21;

const int ROWSM4 = 14;

/**
 * Estructura para definir coordenadas en general
 */
struct Coord {
    int row;
    int col;
};

// Vector para almacenar la ruta más rápida
std::vector<std::pair<int, int>> fastest_route;
std::vector<std::pair<int, int>> fastest_route2;
std::vector<std::pair<int, int>> fastest_route3;
std::vector<std::pair<int, int>> fastest_route4;

// Vector que usamos para que el enemigo se mueva
std::vector<std::pair<int, int>> mover;
std::vector<std::pair<int, int>> mover2;
std::vector<std::pair<int, int>> mover3;
std::vector<std::pair<int, int>> mover4;

bool p0 = true;
bool p1 = true;
bool p2 = true;
bool p3 = true;
bool p4 = true;
bool p5 = true;

/**
 * Matrices utilizadas en el juego
 */
vector<std::vector<std::string>> matriz1A = {
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"},
        {"#","0","0","0","0","#","0","0","0","0","0","0","0","0","0","#","0","0","0","0","#"},
        {"#","0","#","#","0","#","0","#","#","#","#","#","#","#","0","#","0","#","#","0","#"},
        {"#","0","#","0","0","#","0","0","0","0","0","0","0","0","0","#","0","0","#","0","#"},
        {"#","0","#","0","0","0","0","0","#","#","0","#","#","0","0","0","0","0","#","0","#"},
        {"#","0","0","0","#","#","#","0","#","0","0","0","#","0","#","#","#","0","0","0","#"},
        {"#","0","#","0","0","0","0","0","#","0","0","0","#","0","0","0","0","0","#","0","#"},
        {"#","0","#","0","#","#","#","0","#","#","#","#","#","0","#","#","#","0","#","0","#"},
        {"#","0","#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#","0","#"},
        {"#","0","#","0","#","#","#","#","#","#","0","#","#","#","#","#","#","0","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"}
};
static string matriz1[ROWS][COLS] = {
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"},
        {"#","0","0","0","0","#","0","0","0","0","0","0","0","0","0","#","0","0","0","0","#"},
        {"#","0","#","#","0","#","0","#","#","#","#","#","#","#","0","#","0","#","#","0","#"},
        {"#","0","#","0","0","#","0","0","0","0","0","0","0","0","0","#","0","0","#","0","#"},
        {"#","0","#","0","0","0","0","0","#","#","0","#","#","0","0","0","0","0","#","0","#"},
        {"#","0","0","0","#","#","#","0","#","0","0","0","#","0","#","#","#","0","0","0","#"},
        {"#","0","#","0","0","0","0","0","#","0","0","0","#","0","0","0","0","0","#","0","#"},
        {"#","0","#","0","#","#","#","0","#","#","#","#","#","0","#","#","#","0","#","0","#"},
        {"#","0","#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#","0","#"},
        {"#","0","#","0","#","#","#","#","#","#","0","#","#","#","#","#","#","0","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"}
};
vector<std::vector<std::string>> matriz2A = {
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"},
        {"#","0","#","0","#","0","#","0","0","0","0","0","0","0","#","0","#","0","#","0","#"},
        {"#","0","#","0","#","0","#","0","#","#","#","#","#","0","#","0","#","0","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#"},
        {"#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#"},
        {"#","0","#","0","#","0","#","0","0","0","#","0","0","0","#","0","#","0","#","0","#"},
        {"#","0","0","0","#","0","#","0","#","#","#","#","#","0","#","0","#","0","0","0","#"},
        {"#","0","#","0","0","0","#","0","0","0","#","0","0","0","#","0","0","0","#","0","#"},
        {"#","0","#","#","0","0","0","0","#","0","#","0","#","0","0","0","0","#","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"}
};


static string matriz2[ROWS][COLS] = {
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"},
        {"#","0","#","0","#","0","#","0","0","0","0","0","0","0","#","0","#","0","#","0","#"},
        {"#","0","#","0","#","0","#","0","#","#","#","#","#","0","#","0","#","0","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#"},
        {"#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#","0","#"},
        {"#","0","#","0","#","0","#","0","0","0","#","0","0","0","#","0","#","0","#","0","#"},
        {"#","0","0","0","#","0","#","0","#","#","#","#","#","0","#","0","#","0","0","0","#"},
        {"#","0","#","0","0","0","#","0","0","0","#","0","0","0","#","0","0","0","#","0","#"},
        {"#","0","#","#","0","0","0","0","#","0","#","0","#","0","0","0","0","#","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"}
};

vector<std::vector<std::string>> matriz3A = {
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"},
        {"#","0","0","0","0","#","0","0","0","0","0","0","0","0","0","#","0","0","0","0","#"},
        {"#","0","#","#","0","#","0","0","#","#","#","#","#","0","0","#","0","#","#","0","#"},
        {"#","0","#","#","0","#","#","0","#","#","#","#","#","0","#","#","0","#","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","0","#","#","#","#","0","#","#","#","#","#","#","#","0","#","#","#","#","0","#"},
        {"#","0","#","0","0","#","0","#","0","0","0","0","0","#","0","#","0","0","#","0","#"},
        {"#","0","#","0","0","#","0","#","#","#","0","#","#","#","0","#","0","0","#","0","#"},
        {"#","0","0","0","0","#","0","#","#","#","0","#","#","#","0","#","0","0","0","0","#"},
        {"#","0","#","#","#","#","0","#","#","0","0","0","#","#","0","#","#","#","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"}
};
static string matriz3[ROWS][COLS] = {
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"},
        {"#","0","0","0","0","#","0","0","0","0","0","0","0","0","0","#","0","0","0","0","#"},
        {"#","0","#","#","0","#","0","0","#","#","#","#","#","0","0","#","0","#","#","0","#"},
        {"#","0","#","#","0","#","#","0","#","#","#","#","#","0","#","#","0","#","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","0","#","#","#","#","0","#","#","#","#","#","#","#","0","#","#","#","#","0","#"},
        {"#","0","#","0","0","#","0","#","0","0","0","0","0","#","0","#","0","0","#","0","#"},
        {"#","0","#","0","0","#","0","#","#","#","0","#","#","#","0","#","0","0","#","0","#"},
        {"#","0","0","0","0","#","0","#","#","#","0","#","#","#","0","#","0","0","0","0","#"},
        {"#","0","#","#","#","#","0","#","#","0","0","0","#","#","0","#","#","#","#","0","#"},
        {"#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"}
};

vector<std::vector<std::string>> matriz4A = {
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"},
        {"#","0","#","#","0","#","0","#","#","#","#","#","#","#","0","#","0","#","#","0","#"},
        {"#","0","0","0","0","#","0","0","0","0","#","0","0","0","0","#","0","0","0","0","#"},
        {"#","#","#","#","0","#","#","#","0","0","#","0","0","#","#","#","0","#","#","#","#"},
        {"#","#","#","#","0","#","0","0","0","0","0","0","0","0","0","#","0","#","#","#","#"},
        {"#","#","#","#","0","#","0","0","#","#","0","#","#","0","0","#","0","#","#","#","#"},
        {"#","0","0","0","0","0","0","0","#","0","0","0","#","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","0","#","0","0","#","0","0","0","#","0","0","#","0","#","#","#","#"},
        {"#","#","#","#","0","#","0","0","#","#","#","#","#","0","0","#","0","#","#","#","#"},
        {"#","#","#","#","0","#","0","0","0","0","0","0","0","0","0","#","0","#","#","#","#"},
        {"#","0","0","0","0","0","0","0","#","#","#","#","#","0","0","0","0","0","0","0","#"},
        {"#","0","#","0","#","#","#","0","0","0","#","0","0","0","#","#","#","0","0","0","#"},
        {"#","0","#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"}
};


static string matriz4[ROWSM4][COLS] = {
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"},
        {"#","0","#","#","0","#","0","#","#","#","#","#","#","#","0","#","0","#","#","0","#"},
        {"#","0","0","0","0","#","0","0","0","0","#","0","0","0","0","#","0","0","0","0","#"},
        {"#","#","#","#","0","#","#","#","0","0","#","0","0","#","#","#","0","#","#","#","#"},
        {"#","#","#","#","0","#","0","0","0","0","0","0","0","0","0","#","0","#","#","#","#"},
        {"#","#","#","#","0","#","0","0","#","#","0","#","#","0","0","#","0","#","#","#","#"},
        {"#","0","0","0","0","0","0","0","#","0","0","0","#","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","0","#","0","0","#","0","0","0","#","0","0","#","0","#","#","#","#"},
        {"#","#","#","#","0","#","0","0","#","#","#","#","#","0","0","#","0","#","#","#","#"},
        {"#","#","#","#","0","#","0","0","0","0","0","0","0","0","0","#","0","#","#","#","#"},
        {"#","0","0","0","0","0","0","0","#","#","#","#","#","0","0","0","0","0","0","0","#"},
        {"#","0","#","0","#","#","#","0","0","0","#","0","0","0","#","#","#","0","0","0","#"},
        {"#","0","#","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","#"},
        {"#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#","#"}
};

/**
 * Varibles generales del juego
 */

static string mencel;

static int posJugx;
static int posJugy;
static Jugador jugador1 = { 0 };
static Enemigos enemigo1 = { 0 };
static Enemigos enemigo2 = {0};
static Enemigos enemigo3 = {0};
static Enemigos enemigo4 = {0};
static int cont = 0;
static int cont2 = 0;


static const int cantP1 = 116;
static const int cantP2 = 121;
static const int cantP3 = 102;
static const int cantP4 = 122;
static Pastillas pastillas1[cantP1] = {0};
static Pastillas pastillas2[cantP2] = {0};
static Pastillas pastillas3[cantP3] = {0};
static Pastillas pastillas4[cantP4] = {0};
static Pastillas superP1[5] = {0};
static Pastillas superP2[6] = {0};
static Pastillas superP3[5] = {0};
static Pastillas superP4[6] = {0};
static bool pastilla = false;
static bool respawn = false;
static bool respawn2 = false;
static bool respawn3 = false;
static bool respawn4 = false;

/**
 * Posiciones pastillas
 */
static int posicionxP1[cantP1] = {64,64,64,64,64,64,64,64,64,64,
                               110,110,110,
                               156,156,156,156,156,156,156,156,156,
                               202,202,202,202,202,202,202,
                               248,248,248,248,
                               294,294,294,294,294,294,294,
                               340,340,340,340,340,340,340,340,
                               386,386,386,386,
                               432,432,432,432,
                               478,478,478,478,
                               524,524,524,524,
                               570,570,570,570,
                               616,616,616,616,616,616,616,616,
                               662,662,662,662,662,662,662,
                               708,708,708,708,
                               754,754,754,754,754,754, 754,
                               800,800,800,800,800,800,800,800,800,
                               846,846,846,
                               892,892,892,892,892,892,892,892,892,892};

static int posicionyP1[cantP1] = {64,110,156,202,248,294,340,386,432,478,
                               64,248,478,
                               64,156,202,248,294,340,386,432,478,
                               64,110,156,202,294,386,478,
                               202,294,386,478,
                               64,110,156,202,294,386,478,
                               64,156,202,248,294,340,386,478,
                               64,156,386,478,
                               64,156,386,478,
                               64,156,386,478,
                               64,156,386,478,
                               64,156,386,478,
                               64,156,202,248,294,340,386,478,
                               64,110,156,202,294,386,478,
                               202,294,386,478,
                               64,110,156,202,294,386,478,
                               64,156,202,248,294,340,386,432,478,
                               64,248,478,
                               64,110,156,202,248,294,340,386,432,478};

static int posMxP1[cantP1] = {1,1,1,1,1,1,1,1,1,1,
                             2,2,2,
                             3,3,3,3,3,3,3,3,3,
                             4,4,4,4,4,4,4,
                             5,5,5,5,
                             6,6,6,6,6,6,6,
                             7,7,7,7,7,7,7,7,
                             8,8,8,8,
                             9,9,9,9,
                             10,10,10,10,
                             11,11,11,11,
                             12,12,12,12,
                             13,13,13,13,13,13,13,13,
                             14,14,14,14,14,14,14,
                             15,15,15,15,
                             16,16,16,16,16,16,16,
                             17,17,17,17,17,17,17,17,17,
                             18,18,18,
                             19,19,19,19,19,19,19,19,19,19};

static int posMyP1[cantP1] = {1,2,3,4,5,6,7,8,9,10,
                              1,5,10,
                              1,3,4,5,6,7,8,9,10,
                              1,2,3,4,6,8,10,
                              4,6,8,10,
                              1,2,3,4,6,8,10,
                              1,3,4,5,6,7,8,10,
                              1,3,8,10,
                              1,3,8,10,
                              1,3,8,10,
                              1,3,8,10,
                              1,3,8,10,
                              1,3,4,5,6,7,8,10,
                              1,2,3,4,6,8,10,
                              4,6,8,10,
                              1,2,3,4,6,8,10,
                              1,3,4,5,6,7,8,9,10,
                              1,5,10,
                              1,2,3,4,5,6,7,8,9,10,};

static int posicionxP2[cantP2] = {64,64,64,64,64,64,64,64,64,64,
                               110,110,110,
                               156,156,156,156,156,156,156,156,156,
                               202,202,202,202,
                               248,248,248,248,248,248,248,248,248,248,
                               294,294,294,
                               340,340,340,340,340,340,340,340,340,340,
                               386,386,386,386,
                               432,432,432,432,432,432,432,
                               478,
                               524,524,524,524,524,524,524,
                               570,570,570,570,
                               616,616,616,616,616,616,616,616,616,616,
                               662,662,662,
                               708,708,708,708, 708,708,708,708,708,708,
                               754,754,754,754,
                               800,800,800,800,800,800,800,800,800,
                               846,846,846,
                               892,892,892,892,892,892,892,892,892,892};

static int posicionyP2[cantP2] = {64,110,156,202,248,294,340,386,432,478,
                               156,340,478,
                               64,110,156,202,248,294,340,386,478,
                               156,386,432,478,
                               64,110,156,202,248,294,340,386,432,478,
                               156,432,478,
                               64,110,156,202,248,294,340,386,432,478,
                               156,294,386,478,
                               156,202,248,294,386,432,478,
                               156,
                               156,202,248,294,386,432,478,
                               156,294,386,478,
                               64,110,156,202,248,294,340,386,432,478,
                               156,432,478,
                               64,110,156,202,248,294,340,386,432,478,
                               156,386,432,478,
                               64,110,156,202,248,294,340,386,478,
                               156,340,478,
                               64,110,156,202,248,294,340,386,432,478};

static int posMxP2[cantP2] = {1,1,1,1,1,1,1,1,1,1,
                              2,2,2,
                              3,3,3,3,3,3,3,3,3,
                              4,4,4,4,
                              5,5,5,5,5,5,5,5,5,5,
                              6,6,6,
                              7,7,7,7,7,7,7,7,7,7,
                              8,8,8,8,
                              9,9,9,9,9,9,9,
                              10,
                              11,11,11,11,11,11,11,
                              12,12,12,12,
                              13,13,13,13,13,13,13,13,13,13,
                              14,14,14,
                              15,15,15,15,15,15,15,15,15,15,
                              16,16,16,16,
                              17,17,17,17,17,17,17,17,17,
                              18,18,18,
                              19,19,19,19,19,19,19,19,19,19};

static int posMyP2[cantP2] = {1,2,3,4,5,6,7,8,9,10,
                              3,7,10,
                              1,2,3,4,5,6,7,8,10,
                              3,8,9,10,
                              1,2,3,4,5,6,7,8,9,10,
                              3,9,10,
                              1,2,3,4,5,6,7,8,9,10,
                              3,6,8,10,
                              3,4,5,6,8,9,10,
                              3,
                              3,4,5,6,8,9,10,
                              3,6,8,10,
                              1,2,3,4,5,6,7,8,9,10,
                              3,9,10,
                              1,2,3,4,5,6,7,8,9,10,
                              3,8,9,10,
                              1,2,3,4,5,6,7,8,10,
                              3,7,10,
                              1,2,3,4,5,6,7,8,9,10,};

static int posicionxP3[cantP3] = {64,64,64,64,64,64,64,64,64,64,
                               110,110,110,110,
                               156,156,156,156,156,156,
                               202,202,202,202,202,202,202,202,
                               248,248,
                               294,294,294,294,294,294,294,294,294,
                               340,340,340,340,340,
                               386,386,386,
                               432,432,432,
                               478,478,
                               524,524,524,
                               570,570,570,
                               616,616,616,616,616,
                               662,662,662,662,662,662,662,662,662,
                               708,708,
                               754,754,754,754,754,754,754,754,
                               800,800,800,800,800,800,
                               846,846,846,846,
                               892,892,892,892,892,892,892,892,892,892};

static int posicionyP3[cantP3] = {64,110,156,202,248,294,340,386,432,478,
                               64,202,386,478,
                               64,202,294,340,386,478,
                               64,110,156,202,294,340,386,478,
                               202,478,
                               64,110,202,248,294,340,386,432,478,
                               64,110,156,202,478,
                               64,202,478,
                               64,202,478,
                               64,202,
                               64,202,478,
                               64,202,478,
                               64,110,156,202,478,
                               64,110,202,248,294,340,386,432,478,
                               202,478,
                               64,110,156,202,294,340,386,478,
                               64,202,294,340,386,478,
                               64,202,386,478,
                               64,110,156,202,248,294,340,386,432,478};

static int posMxP3[cantP3] = {1,1,1,1,1,1,1,1,1,1,
                              2,2,2,2,
                              3,3,3,3,3,3,
                              4,4,4,4,4,4,4,4,
                              5,5,
                              6,6,6,6,6,6,6,6,6,
                              7,7,7,7,7,
                              8,8,8,
                              9,9,9,
                              10,10,
                              11,11,11,
                              12,12,12,
                              13,13,13,13,13,
                              14,14,14,14,14,14,14,14,
                              15,15,
                              16,16,16,16,16,16,16,16,16,
                              17,17,17,17,17,17,
                              18,18,18,18,
                              19,19,19,19,19,19,19,19,19,19};

static int posMyP3[cantP3] = {1,2,3,4,5,6,7,8,9,10,
                              1,4,8,10,
                              1,4,6,7,8,10,
                              1,2,3,4,6,7,8,10,
                              4,10,
                              1,2,4,5,6,7,8,9,10,
                              1,2,3,4,10,
                              1,4,10,
                              1,4,10,
                              1,4,
                              1,4,10,
                              1,4,10,
                              1,2,3,4,10,
                              1,2,4,5,6,7,8,9,10,
                              4,10,
                              1,2,3,4,6,7,8,10,
                              1,4,6,7,8,10,
                              1,4,8,10,
                              1,2,3,4,5,6,7,8,9,10};

static int posicionxP4[cantP4] = {64,64,64,64,64,64,
                               110,110,110,
                               156,156,156,156,156,
                               202,202,202,202,202,202,202,202,202,202,202,
                               248,248,248,
                               294,294,294,294,294,294,294,294,294,294,
                               340,340,340,340,340,340,340,340,340,340,
                               386,386,386,386,386,386,
                               432,432,432,432,432,432,
                               478,478,
                               524,524,524,524,524,524,
                               570,570,570,570,570,570,
                               616,616,616,616,616,616,616,616,616,616,
                               662,662,662,662,662,662,662,662,662,662,
                               708,708,708,
                               754,754,754,754,754,754,754,754,754,754,754,
                               800,800,800,800,800,
                               846,846,846,
                               892,892,892,892,892,892};

static int posicionyP4[cantP4] = {18,64,248,432,478,524,
                                64,248,432,
                                64,248,432,478,524,
                                18,64,110,156,202,248,294,340,386,432,524,
                                248,432,524,
                                18,64,156,202,248,294,340,386,432,524,
                                64,156,202,248,294,340,386,432,478,524,
                                64,110,156,386,478,524,
                                64,110,156,386,478,524,
                                156,524,
                                64,110,156,386,478,524,
                                64,110,156,386,478,524,
                                64,156,202,248,294,340,386,432,478,524,
                                18,64,156,202,248,294,340,386,432,524,
                                248,432,524,
                                18,64,110,156,202,248,294,340,386,432,524,
                                64,248,432,478,524,
                                64,248,432,
                                18,64,248,432,478,524};

static int posMxP4[cantP4] = {1,1,1,1,1,1,
                              2,2,2,
                              3,3,3,3,3,
                              4,4,4,4,4,4,4,4,4,4,4,
                              5,5,5,
                              6,6,6,6,6,6,6,6,6,6,
                              7,7,7,7,7,7,7,7,7,7,
                              8,8,8,8,8,8,
                              9,9,9,9,9,9,
                              10,10,
                              11,11,11,11,11,11,
                              12,12,12,12,12,12,
                              13,13,13,13,13,13,13,13,13,13,
                              14,14,14,14,14,14,14,14,14,14,
                              15,15,15,
                              16,16,16,16,16,16,16,16,16,16,16,
                              17,17,17,17,17,
                              18,18,18,
                              19,19,19,19,19,19};

static int posMyP4[cantP4] = {1,2,6,10,11,12,
                              2,6,10,
                              2,6,10,11,12,
                              1,2,3,4,5,6,7,8,9,10,12,
                              6,10,12,
                              1,2,4,5,6,7,8,9,10,12,
                              2,4,5,6,7,8,9,10,11,12,
                              2,3,4,9,11,12,
                              2,3,4,9,11,12,
                              4,12,
                              2,3,4,9,11,12,
                              2,3,4,9,11,12,
                              2,4,5,6,7,8,9,10,11,12,
                              1,2,4,5,6,7,8,9,10,12,
                              6,10,12,
                              1,2,3,4,5,6,7,8,9,10,12,
                              2,6,10,11,12,
                              2,6,10,
                              1,2,6,10,11,11
};

static int posMx;
static int posMy;

/**
 * Se cargan las estructuras para el jugador, enemigos y mapas
 */
static Texture2D player;
static Texture2D personaje1U;
static Texture2D personaje1D;
static Texture2D personaje1L;
static Texture2D personaje1R;

static Texture2D personaje2U;
static Texture2D personaje2D;
static Texture2D personaje2L;
static Texture2D personaje2R;

static Texture2D personaje3U;
static Texture2D personaje3D;
static Texture2D personaje3L;
static Texture2D personaje3R;

static Texture2D personaje4U;
static Texture2D personaje4D;
static Texture2D personaje4L;
static Texture2D personaje4R;

static Texture2D personajecheto;

static string perselect;

static Texture2D pacama;
static Texture2D pacamaU;
static Texture2D pacamaD;
static Texture2D pacamaL;
static Texture2D pacamaR;

static Texture2D pacbla;
static Texture2D pacblaU;
static Texture2D pacblaD;
static Texture2D pacblaL;
static Texture2D pacblaR;

static Texture2D pacver;
static Texture2D pacverU;
static Texture2D pacverD;
static Texture2D pacverL;
static Texture2D pacverR;

static Texture2D pacmor;
static Texture2D pacmorU;
static Texture2D pacmorD;
static Texture2D pacmorL;
static Texture2D pacmorR;


static int vidas = 3;
static int puntaje1 = 0;
static int puntos1 = 0;
static int puntaje2 = 0;
static int puntos2 = 0;
static int puntaje3 = 0;
static int puntos3 = 0;
static int puntaje4 = 0;
static int puntos4 = 0;

/**
 * Clase utilizada para la conexión c++ - android studio
 */
class socket;
/**
 * Estructura que representa un nodo en el espacio de búsqueda
 */
struct Node {
    int x, y;           // Coordenadas del nodo en el espacio
    int cost;           // Costo acumulado para llegar a este nodo
    int heuristic;      // Valor heurístico (distancia estimada hasta el objetivo)
    Node* parent;       // Puntero al nodo padre

    Node(int x, int y, int cost, int heuristic, Node* parent)
            : x(x), y(y), cost(cost), heuristic(heuristic), parent(parent) {}
};

/**
 * Función de comparación para el ordenamiento de la lista abierta
 */
struct CompareNode {
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->cost + lhs->heuristic > rhs->cost + rhs->heuristic;
    }
};

// Función para verificar si una posición está dentro de los límites del espacio de búsqueda
bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// Función para calcular la distancia Manhattan entre dos puntos en el espacio
int calculateManhattanDistance(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

/**
 * Función de búsqueda A*
 * @param grid
 * @param start
 * @param goal
 * @param visited
 * @param openList
 * @return
 */
std::vector<std::pair<int, int>> astarSearch(std::vector<std::vector<std::string>>& grid,
                                             std::pair<int, int> start,
                                             std::pair<int, int> goal,
                                             std::vector<Node*>& visited,
                                             std::priority_queue<Node*, std::vector<Node*>, CompareNode>& openList) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Verificar si el punto de inicio o el objetivo están fuera de los límites
    if (!isValid(start.first, start.second, rows, cols) ||
        !isValid(goal.first, goal.second, rows, cols)) {
        return {};
    }

    // Verificar si el punto de inicio o el objetivo están bloqueados
    if (grid[start.first][start.second] == "#" || grid[goal.first][goal.second] == "#") {
        return {};
    }

    // Cola de prioridad para la lista abierta
    // std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

    // Crear el nodo de inicio y añadirlo a la lista abierta
    Node* startNode = new Node(start.first, start.second, 0,
                               calculateManhattanDistance(start.first, start.second,
                                                          goal.first, goal.second),
                               nullptr);
    openList.push(startNode);

    // Bucle principal de búsqueda
    while (!openList.empty()) {
        // Encontrar el nodo con el menor costo acumulado + valor heurístico
        Node* currentNode = openList.top();
        openList.pop();

        // Marcar el nodo actual como visitado
        visited[currentNode->x * cols + currentNode->y] = currentNode;

        // Verificar si el nodo actual es el objetivo
        if (currentNode->x == goal.first && currentNode->y == goal.second) {
            // Reconstruir el camino desde el nodo objetivo hasta el nodo inicial
            std::vector<std::pair<int, int>> path;
            while (currentNode != nullptr) {
                path.push_back(std::make_pair(currentNode->x, currentNode->y));
                currentNode = currentNode->parent;
            }
            std::reverse(path.begin(), path.end());

            return path;
        }

        // Generar sucesores (nodos vecinos)
        std::vector<std::pair<int, int>> neighbors = {
                {currentNode->x - 1, currentNode->y},  // Izquierda
                {currentNode->x + 1, currentNode->y},  // Derecha
                {currentNode->x, currentNode->y - 1},  // Arriba
                {currentNode->x, currentNode->y + 1}   // Abajo
        };

        for (const auto& neighbor : neighbors) {
            int neighborX = neighbor.first;
            int neighborY = neighbor.second;

            // Verificar si el vecino está dentro de los límites y no ha sido visitado
            if (isValid(neighborX, neighborY, rows, cols) &&
                grid[neighborX][neighborY] == "0" &&
                visited[neighborX * cols + neighborY] == nullptr) {
                // Calcular el costo acumulado y el valor heurístico para el vecino
                int neighborCost = currentNode->cost + 1;
                int neighborHeuristic = calculateManhattanDistance(neighborX, neighborY,
                                                                   goal.first, goal.second);

                // Crear el nodo vecino y añadirlo a la lista abierta
                Node* neighborNode = new Node(neighborX, neighborY, neighborCost,
                                              neighborHeuristic, currentNode);
                openList.push(neighborNode);
            }
        }
    }

    // No se encontró un camino válido
    std::cerr << "No se pudo encontrar un camino válido.";

    return {

    };
}
/**
 * Función para encontrar la ruta más rápida utilizando backtracking para el pacman, cuando busca la pastilla
 * @param row
 * @param col
 * @param route
 * @param i
 */
void backtrackE1M1(int row, int col, std::vector<std::pair<int, int>>& route, int i) {
    // Si se llegó al punto de destino, comprobar si esta es la ruta más rápida
    //posMy va a ser la posicion de FILA de la matriz del destino, en este caso el jugador
    //posMx va a ser la posicion de COLUMNA de la matriz del destino, en este caso el jugador
        if (row == superP1[i].MySupP && col == superP1[i].MxSupP) {
            if (fastest_route.empty() || route.size() < fastest_route.size()) {
                fastest_route = route;
                mover = route;
            }
            return;
        }

        // Marcamos la casilla actual como visitada
        //en vez de matriz1 pone el nombre de su matriz
        matriz1[row][col] = "V";

        // Movimiento hacia arriba, los # en nuestro caso son las paredes y obstaculos de la matriz
        if (row > 0 && matriz1[row - 1][col] != "#" && matriz1[row - 1][col] != "V") {
            route.push_back(std::make_pair(row - 1, col));
            backtrackE1M1(row - 1, col, route, i);
            cout<<"("<<row<<","<<col<<")"<<endl;
            route.pop_back();
        }

        // Movimiento hacia abajo
        if (row < ROWS - 1 && matriz1[row + 1][col] != "#" && matriz1[row + 1][col] != "V") {
            route.push_back(std::make_pair(row + 1, col));
            backtrackE1M1(row + 1, col, route, i);
            cout<<"("<<row<<","<<col<<")"<<endl;
            route.pop_back();
        }

        // Movimiento hacia la izquierda
        if (col > 0 && matriz1[row][col - 1] != "#" && matriz1[row][col - 1] != "V") {
            route.push_back(std::make_pair(row, col - 1));
            backtrackE1M1(row, col - 1, route, i);
            cout<<"("<<row<<","<<col<<")"<<endl;
            route.pop_back();
        }

        // Movimiento hacia la derecha
        if (col < COLS - 1 && matriz1[row][col + 1] != "#" && matriz1[row][col + 1] != "V") {
            route.push_back(std::make_pair(row, col + 1));
            backtrackE1M1(row, col + 1, route, i);
            cout<<"("<<row<<","<<col<<")"<<endl;
            route.pop_back();
        }

        // Desmarcamos la casilla actual como visitada,
        // en nuestro caso es "0" porque los espacios libres lo estamos trabajando con "0"
        matriz1[row][col] = "0";
}
/**
 * Función para encontrar la ruta más rápida utilizando backtracking para el pacman, cuando busca la pastilla
 * @param row
 * @param col
 * @param route
 * @param i
 */
void backtrackE1M3(int row, int col, std::vector<std::pair<int, int>>& route, int i) {
    // Si se llegó al punto de destino, comprobar si esta es la ruta más rápida
    //posMy va a ser la posicion de FILA de la matriz del destino, en este caso el jugador
    //posMx va a ser la posicion de COLUMNA de la matriz del destino, en este caso el jugador
    if (row == superP3[i].MySupP && col == superP3[i].MxSupP) {
        if (fastest_route.empty() || route.size() < fastest_route.size()) {
            fastest_route = route;
            mover4 = route;
        }
        return;
    }

    // Marcamos la casilla actual como visitada
    //en vez de matriz1 pone el nombre de su matriz
    matriz3[row][col] = "V";

    // Movimiento hacia arriba, los # en nuestro caso son las paredes y obstaculos de la matriz
    if (row > 0 && matriz3[row - 1][col] != "#" && matriz3[row - 1][col] != "V") {
        route.push_back(std::make_pair(row - 1, col));
        backtrackE1M3(row - 1, col, route, i);
        cout<<"("<<row<<","<<col<<")"<<endl;
        route.pop_back();
    }

    // Movimiento hacia abajo
    if (row < ROWS - 1 && matriz3[row + 1][col] != "#" && matriz3[row + 1][col] != "V") {
        route.push_back(std::make_pair(row + 1, col));
        backtrackE1M3(row + 1, col, route, i);
        cout<<"("<<row<<","<<col<<")"<<endl;
        route.pop_back();
    }

    // Movimiento hacia la izquierda
    if (col > 0 && matriz3[row][col - 1] != "#" && matriz3[row][col - 1] != "V") {
        route.push_back(std::make_pair(row, col - 1));
        backtrackE1M3(row, col - 1, route, i);
        cout<<"("<<row<<","<<col<<")"<<endl;
        route.pop_back();
    }

    // Movimiento hacia la derecha
    if (col < COLS - 1 && matriz3[row][col + 1] != "#" && matriz3[row][col + 1] != "V") {
        route.push_back(std::make_pair(row, col + 1));
        backtrackE1M3(row, col + 1, route, i);
        cout<<"("<<row<<","<<col<<")"<<endl;
        route.pop_back();
    }

    // Desmarcams la casilla actual como visitada,
    // en nuestro caso es "0" porque los espacios libres lo estamos trabajando con "0"
    matriz3[row][col] = "0";
}
/**
 * Función para iniciar y setear el juego.
 */
void juego(void){
    posJugx = 460;
    posJugy = 416;
    posMx = 10;
    posMy = 9;
    jugador1.velocidad.x = 46;
    jugador1.velocidad.y = 46;

    enemigo1.velene.x = 46;
    enemigo1.velene.y = 46;
    enemigo1.posEnex = 460;
    enemigo1.posEney = 234;
    enemigo1.mEnex = 10;
    enemigo1.mEney = 5;


    for(int i = 0; i < cantP1; i++){
        pastillas1[i].rec.width = 10;
        pastillas1[i].rec.height = 10;
        pastillas1[i].rec.x = posicionxP1[i];
        pastillas1[i].rec.y = posicionyP1[i];

        pastillas1[i].active = true;
    }
    for(int i = 0; i < cantP2; i++){
        pastillas2[i].rec.width = 10;
        pastillas2[i].rec.height = 10;
        pastillas2[i].rec.x = posicionxP2[i];
        pastillas2[i].rec.y = posicionyP2[i];
        pastillas2[i].active = true;
    }
    for(int i = 0; i < cantP3; i++){
        pastillas3[i].rec.width = 10;
        pastillas3[i].rec.height = 10;
        pastillas3[i].rec.x = posicionxP3[i];
        pastillas3[i].rec.y = posicionyP3[i];
        pastillas3[i].active = true;
    }
    for(int i = 0; i < cantP4; i++){
        pastillas4[i].rec.width = 10;
        pastillas4[i].rec.height = 10;
        pastillas4[i].rec.x = posicionxP4[i];
        pastillas4[i].rec.y = posicionyP4[i];
        pastillas4[i].active = true;
    }
    for(int i = 0; i < 5; i++){
        superP1[i].rec.width = 20;
        superP1[i].rec.height = 20;
        int num = rand()%(cantP1);
        superP1[i].rec.x = posicionxP1[num]-5;
        superP1[i].rec.y = posicionyP1[num]-5;
        superP1[i].MxSupP = posMxP1[num];
        superP1[i].MySupP = posMyP1[num];
        superP1[i].active = false;
    }
    for(int i = 0; i < 6; i++){
        superP2[i].rec.width = 20;
        superP2[i].rec.height = 20;
        int num = rand()%(cantP2);
        superP2[i].rec.x = posicionxP2[num]-5;
        superP2[i].rec.y = posicionyP2[num]-5;
        superP2[i].MxSupP = posMxP2[num];
        superP2[i].MySupP = posMyP2[num];
        superP2[i].active = false;
    }
    for(int i = 0; i < 5; i++){
        superP3[i].rec.width = 20;
        superP3[i].rec.height = 20;
        int num = rand()%(cantP3);
        superP3[i].rec.x = posicionxP3[num]-5;
        superP3[i].rec.y = posicionyP3[num]-5;
        superP3[i].MxSupP = posMxP3[num];
        superP3[i].MySupP = posMyP3[num];
        superP3[i].active = false;
    }
    for(int i = 0; i < 6; i++){
        superP4[i].rec.width = 20;
        superP4[i].rec.height = 20;
        int num = rand()%(cantP4);
        superP4[i].rec.x = posicionxP4[num]-5;
        superP4[i].rec.y = posicionyP4[num]-5;
        superP4[i].MxSupP = posMxP4[num];
        superP4[i].MySupP = posMyP4[num];
        superP4[i].active = false;
    }
}
/**
 * Funcion que se encuentra dentro del while (!WindowShouldClose())
 * al estar dentro del while el contador va a iniciar; nivel 1.
 */
void actJuego1(void){

    delayTimer += 0.5f;

    if (respawn) {
        spawnTimer += 0.5f;
        enemigo1.posEnex = -50;
        enemigo1.posEney = -50;
        enemigo1.mEnex = 0;
        enemigo1.mEney = 0;
        if (spawnTimer >= spawnTimerMax){
            respawn = false;
            spawnTimer = 0.f;
            enemigo1.posEnex = 460;
            enemigo1.posEney = 234;
            enemigo1.mEnex = 10;
            enemigo1.mEney = 5;
        }
    }
    if (pastilla) {
        pastillaDelay += 0.5f;
        if (pastillaDelay >= pastillaDelayMax) {

            pastilla = false;
            pastillaDelay = 0.f;
            cout << "Pastilla desactivada" << endl;
        }
    }

    if(matriz1[posMy-1][posMx] != "#") {
        if (IsKeyPressed(KEY_UP)) {
            posJugy -= jugador1.velocidad.y;
            posMy--;
            if (!superP1[0].active && !superP1[1].active && !superP1[2].active&& !superP1[3].active && !superP1[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz1A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto &node: path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }

                path.clear();
                visited.clear();
            }
        }
        if (IsKeyPressed(KEY_UP)) {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1U; }
                if (perselect == "2") { player = personaje2U; }
                if (perselect == "3") { player = personaje3U; }
                if (perselect == "4") { player = personaje4U; }
            }

        }
        if (mencel == "1") {
            posJugy -= jugador1.velocidad.y;
            posMy--;
            if (!superP1[0].active && !superP1[1].active && !superP1[2].active&& !superP1[3].active&& !superP1[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz1A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto &node: path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }
        }
        if (mencel == "1") {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1U; }
                if (perselect == "2") { player = personaje2U; }
                if (perselect == "3") { player = personaje3U; }
                if (perselect == "4") { player = personaje4U; }
            }
        }
    }
    if(matriz1[posMy+1][posMx] != "#"){
        if (IsKeyPressed(KEY_DOWN)) {
            posJugy += jugador1.velocidad.y;
            posMy++;
            if (!superP1[0].active && !superP1[1].active && !superP1[2].active&& !superP1[3].active&& !superP1[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz1A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }


        }
        if (IsKeyPressed(KEY_DOWN)) {
            if (pastilla) {player = personajecheto;}
            else{
                if(perselect == "1"){player = personaje1D;}
                if(perselect == "2"){player = personaje2D;}
                if(perselect == "3"){player = personaje3D;}
                if(perselect == "4"){player = personaje4D;}
            }
        }
        if (mencel == "2") {
            posJugy += jugador1.velocidad.y;
            posMy++;
            if (!superP1[0].active && !superP1[1].active && !superP1[2].active&& !superP1[3].active&& !superP1[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz1A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }
        }
        if (mencel == "2") {
            if (pastilla) {player = personajecheto;}
            else{
                if(perselect == "1"){player = personaje1D;}
                if(perselect == "2"){player = personaje2D;}
                if(perselect == "3"){player = personaje3D;}
                if(perselect == "4"){player = personaje4D;}
            }
        }
    }

    if(matriz1[posMy][posMx-1] != "#" ){
        if (IsKeyPressed(KEY_LEFT)) {
            posJugx -= jugador1.velocidad.x;
            posMx--;
            if (!superP1[0].active && !superP1[1].active && !superP1[2].active&& !superP1[3].active&& !superP1[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz1A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }

        }
        if (IsKeyPressed(KEY_LEFT)){
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1L; }
                if (perselect == "2") { player = personaje2L; }
                if (perselect == "3") { player = personaje3L; }
                if (perselect == "4") { player = personaje4L; }
            }
        }
        if (mencel == "3") {
            posJugx -= jugador1.velocidad.x;
            posMx--;
            if (!superP1[0].active && !superP1[1].active && !superP1[2].active && !superP1[3].active && !superP1[4].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz1A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }
        }
        if (mencel == "3") {
            if (pastilla) {player = personajecheto;}
            else{
                if(perselect == "1"){player = personaje1L;}
                if(perselect == "2"){player = personaje2L;}
                if(perselect == "3"){player = personaje3L;}
                if(perselect == "4"){player = personaje4L;}}

        }
    }

    if(matriz1[posMy][posMx+1] != "#"){
        if (IsKeyPressed(KEY_RIGHT)) {
            posJugx += jugador1.velocidad.x;
            posMx++;
            if (!superP1[0].active && !superP1[1].active && !superP1[2].active&& !superP1[3].active&& !superP1[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz1A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }

        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1R; }
                if (perselect == "2") { player = personaje2R; }
                if (perselect == "3") { player = personaje3R; }
                if (perselect == "4") { player = personaje4R; }
            }
        }
        if (mencel == "4") {
            posJugx += jugador1.velocidad.x;
            posMx++;
            if (!superP1[0].active && !superP1[1].active && !superP1[2].active && !superP1[3].active && !superP1[4].active) {

                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz1A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto &node: path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }
        }
        if (mencel == "4") {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1R; }
                if (perselect == "2") { player = personaje2R; }
                if (perselect == "3") { player = personaje3R; }
                if (perselect == "4") { player = personaje4R; }
            }
        }
    }
    for(int i = 0; i < cantP1; i++){
        if(pastillas1[i].active) {
            if (abs(posJugx + 23 - pastillas1[i].rec.x) < 23) {
                if (abs(posJugy + 23 - pastillas1[i].rec.y) < 23) {
                    pastillas1[i].active = false;
                    puntos1++;
                    puntaje1+=10;

                }
            }
        }
    }

    if (delayTimer >=delayTimerMax){
        if (superP1[0].active && p0) {
                std::vector<std::pair<int, int>> route;
                route.push_back(std::make_pair(enemigo1.mEney, enemigo1.mEnex));
                backtrackE1M1(enemigo1.mEney, enemigo1.mEnex, route, 0);
                fastest_route.clear();
                route.clear();
                p0 = false;
        }
        if (superP1[1].active&& p1) {
                std::vector<std::pair<int, int>> route;
                route.push_back(std::make_pair(enemigo1.mEney, enemigo1.mEnex));
                backtrackE1M1(enemigo1.mEney, enemigo1.mEnex, route,  1);
                fastest_route.clear();
                route.clear();
                p1 = false;
        }
        if (superP1[2].active&& p2) {
            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo1.mEney, enemigo1.mEnex));
            backtrackE1M1(enemigo1.mEney, enemigo1.mEnex, route, 2);
            fastest_route.clear();
            route.clear();
            p2 = false;

        }
        if (superP1[3].active&& p3) {
            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo1.mEney, enemigo1.mEnex));
            backtrackE1M1(enemigo1.mEney, enemigo1.mEnex, route, 3);
            fastest_route.clear();
            route.clear();
            p3 = false;

        }
        if (superP1[4].active&& p4) {
            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo1.mEney, enemigo1.mEnex));
            backtrackE1M1(enemigo1.mEney, enemigo1.mEnex, route, 4);
            fastest_route.clear();
            route.clear();
            p4 = false;

        }


        if (!mover.empty()){
            if (mover[1].first < enemigo1.mEney){
                pacama = pacamaU;
                enemigo1.posEney -= enemigo1.velene.y;
                enemigo1.mEney--;
            }
            if (mover[1].first > enemigo1.mEney){
                pacama = pacamaD;
                enemigo1.posEney += enemigo1.velene.y;
                enemigo1.mEney++;
            }
            if (mover[1].second < enemigo1.mEnex) {
                pacama = pacamaL;
                enemigo1.posEnex -= enemigo1.velene.x;
                enemigo1.mEnex--;
            }
            if (mover[1].second > enemigo1.mEnex) {
                pacama = pacamaR;
                enemigo1.posEnex += enemigo1.velene.x;
                enemigo1.mEnex++;
            }
            mover.erase(mover.begin());
        }
        if(!pastilla){
            if(enemigo1.mEnex == posMx && enemigo1.mEney == posMy){
                vidas--;
                fastest_route.clear();
                mover.clear();

                posJugx = 460;
                posJugy = 416;
                posMx = 10;
                posMy = 9;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 234;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 5;
            }
        }
        if(pastilla){
            if(enemigo1.mEnex == posMx && enemigo1.mEney == posMy){
                fastest_route.clear();
                mover.clear();
                puntaje1 += 50;
                respawn = true;

                enemigo1.posEnex = -10;
                enemigo1.posEney = -10;
                enemigo1.mEnex = 0;
                enemigo1.mEney = 0;
            }
        }
        delayTimer = 0.f;
    }
    if(puntaje1 == 200){
        superP1[0].active = true;

    }
    if(puntaje1 == 400){
        superP1[1].active = true;
    }
    if(puntaje1 == 600){
        superP1[2].active = true;
    }
    if(puntaje1 == 800){
        superP1[3].active = true;
    }
    if(puntaje1 == 1000){
        superP1[4].active = true;
    }
    for(int i = 0; i < 5; i++){
        if(superP1[i].active) {
            if (abs(posJugx + 23 - superP1[i].rec.x) < 23) {
                if (abs(posJugy + 23 - superP1[i].rec.y) < 23) {
                    superP1[i].active = false;
                    pastilla = true;
                    cout << "Pastilla activada" << endl;
                }
            }
            if (abs(enemigo1.posEnex + 23 - superP1[i].rec.x) < 23) {
                if (abs(enemigo1.posEney + 23 - superP1[i].rec.y) < 23) {
                    superP1[i].active = false;
                }
            }
        }
    }

}

/**
 * Función que inicia el nivel dos del juego
 */
void actJuego2(void) {
    delayTimer += 0.5f;

    if (respawn) {
        spawnTimer += 0.5f;
        enemigo1.posEnex = -50;
        enemigo1.posEney = -50;
        enemigo1.mEnex = 0;
        enemigo1.mEney = 0;
        if (spawnTimer >= spawnTimerMax){
            respawn = false;
            spawnTimer = 0.f;
            enemigo1.posEnex = 460;
            enemigo1.posEney = 46;
            enemigo1.mEnex = 10;
            enemigo1.mEney = 1;

        }
    }
    if (respawn2) {
        spawnTimer2 += 0.5f;
        enemigo2.posEnex = -50;
        enemigo2.posEney = -50;
        enemigo2.mEnex = 0;
        enemigo2.mEney = 0;

        if (spawnTimer2 >= spawnTimerMax2){
            respawn2 = false;
            spawnTimer2 = 0.f;
            enemigo2.posEnex = 416;
            enemigo2.posEney = 46;
            enemigo2.mEnex = 9;
            enemigo2.mEney = 1;

        }
    }

    if (pastilla) {
        pastillaDelay += 0.5f;
        if (pastillaDelay >= pastillaDelayMax) {
            pastilla = false;
            pastillaDelay = 0.f;
        }
    }

    if(matriz2[posMy-1][posMx] != "#"){
        if (IsKeyPressed(KEY_UP)) {
            posJugy -= jugador1.velocidad.y;
            posMy--;
            if (!superP2[0].active && !superP2[1].active && !superP2[2].active && !superP2[3].active&& !superP2[4].active && !superP2[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }


        }
        if (IsKeyPressed(KEY_UP)) {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1U; }
                if (perselect == "2") { player = personaje2U; }
                if (perselect == "3") { player = personaje3U; }
                if (perselect == "4") { player = personaje4U; }
            }
        }
        if (mencel == "1") {
            posJugy -= jugador1.velocidad.y;
            posMy--;
            if (!superP2[0].active && !superP2[1].active && !superP2[2].active && !superP2[3].active&& !superP2[4].active && !superP2[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;


                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }

        }
        if (mencel == "1") {
            if (pastilla) { player = personajecheto; }
            else {
                if (perselect == "1") { player = personaje1U; }
                if (perselect == "2") { player = personaje2U; }
                if (perselect == "3") { player = personaje3U; }
                if (perselect == "4") { player = personaje4U; }
            }
        }
    }

    if(matriz2[posMy+1][posMx] != "#"){
        if (IsKeyPressed(KEY_DOWN)) {
            posJugy += jugador1.velocidad.y;
            posMy++;
            if (!superP2[0].active && !superP2[1].active && !superP2[2].active && !superP2[3].active&& !superP2[4].active && !superP2[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }


        }
        if (IsKeyPressed(KEY_DOWN)) {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1D; }
                if (perselect == "2") { player = personaje2D; }
                if (perselect == "3") { player = personaje3D; }
                if (perselect == "4") { player = personaje4D; }
            }
        }
        if (mencel == "2") {
            posJugy += jugador1.velocidad.y;
            posMy++;
            if (!superP2[0].active && !superP2[1].active && !superP2[2].active && !superP2[3].active&& !superP2[4].active && !superP2[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);

                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }

        }
        if (mencel == "2") {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1D; }
                if (perselect == "2") { player = personaje2D; }
                if (perselect == "3") { player = personaje3D; }
                if (perselect == "4") { player = personaje4D; }
            }
        };

    }

    if(matriz2[posMy][posMx-1] != "#" ){
        if (IsKeyPressed(KEY_LEFT)) {
            posJugx -= jugador1.velocidad.x;
            posMx--;
            if (!superP2[0].active && !superP2[1].active && !superP2[2].active && !superP2[3].active&& !superP2[4].active && !superP2[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);

                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }


        }
        if (IsKeyPressed(KEY_LEFT)){
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1L; }
                if (perselect == "2") { player = personaje2L; }
                if (perselect == "3") { player = personaje3L; }
                if (perselect == "4") { player = personaje4L; }
            }
        }
        if (mencel == "3") {
            posJugx -= jugador1.velocidad.x;
            posMx--;
            if (!superP2[0].active && !superP2[1].active && !superP2[2].active && !superP2[3].active&& !superP2[4].active && !superP2[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);

                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }

        }
        if (mencel == "3") {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1L; }
                if (perselect == "2") { player = personaje2L; }
                if (perselect == "3") { player = personaje3L; }
                if (perselect == "4") { player = personaje4L; }
            }
        }
    }

    if(matriz2[posMy][posMx+1] != "#"){
        if (IsKeyPressed(KEY_RIGHT)) {
            posJugx += jugador1.velocidad.x;
            posMx++;
            if (!superP2[0].active && !superP2[1].active && !superP2[2].active && !superP2[3].active&& !superP2[4].active && !superP2[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);

                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }

        }
        if (IsKeyPressed(KEY_RIGHT)){
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1R; }
                if (perselect == "2") { player = personaje2R; }
                if (perselect == "3") { player = personaje3R; }
                if (perselect == "4") { player = personaje4R; }
            }
        }
        if (mencel == "4") {
            posJugx += jugador1.velocidad.x;
            posMx++;
            if (!superP2[0].active && !superP2[1].active && !superP2[2].active && !superP2[3].active&& !superP2[4].active && !superP2[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                std::cout << "Lista abierta:";
                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
            }

        }
        if (mencel == "4") {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1R; }
                if (perselect == "2") { player = personaje2R; }
                if (perselect == "3") { player = personaje3R; }
                if (perselect == "4") { player = personaje4R; }
            }
        }
    }
    for (int i = 0; i < cantP2; i++) {
        if (pastillas2[i].active) {
            if (abs(posJugx + 23 - pastillas2[i].rec.x) < 23) {
                if (abs(posJugy + 23 - pastillas2[i].rec.y) < 23) {
                    pastillas2[i].active = false;
                    puntos2++;
                    puntaje2 += 10;
                }
            }
        }
    }
    if (delayTimer >=delayTimerMax){
        if (superP2[0].active && p0) {
            p0 = false;
            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP2[0].MySupP, superP2[0].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            std::cout << "Lista abierta:";
            while (!openList.empty()) {
                Node* node = openList.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada:";
            for (const auto& node : visited) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();


            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            std::cout << "Lista abierta2:";
            while (!openList2.empty()) {
                Node* node = openList2.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList2.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada2:";
            for (const auto& node : visited2) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


        }
        if (superP2[1].active && p1) {
            p1 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP2[1].MySupP, superP2[1].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();



            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


        }
        if (superP2[2].active && p2) {
            p2 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP2[2].MySupP, superP2[2].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();



            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


        }
        if (superP2[3].active && p3) {
            p3 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP2[3].MySupP, superP2[3].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();



            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


        }
        if (superP2[4].active && p4) {
            p4 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP2[4].MySupP, superP2[4].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();



            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


        }
        if (superP2[5].active && p5) {
            p5 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP2[5].MySupP, superP2[5].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();



            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


        }


        if (!mover.empty()) {
            if (mover[1].first < enemigo1.mEney) {
                pacama = pacamaU;
                enemigo1.posEney -= enemigo1.velene.y;
                enemigo1.mEney--;
            }
            if (mover[1].first > enemigo1.mEney) {
                pacama = pacamaD;
                enemigo1.posEney += enemigo1.velene.y;
                enemigo1.mEney++;
            }
            if (mover[1].second < enemigo1.mEnex) {
                pacama = pacamaL;
                enemigo1.posEnex -= enemigo1.velene.x;
                enemigo1.mEnex--;
            }
            if (mover[1].second > enemigo1.mEnex) {
                pacama = pacamaR;
                enemigo1.posEnex += enemigo1.velene.x;
                enemigo1.mEnex++;
            }
            mover.erase(mover.begin());

        }
        if (!mover2.empty()) {

            if (mover2[1].first < enemigo2.mEney) {
                pacbla = pacblaU;
                enemigo2.posEney -= enemigo2.velene.y;
                enemigo2.mEney--;
            }
            if (mover2[1].first > enemigo2.mEney) {
                pacbla = pacblaD;
                enemigo2.posEney += enemigo2.velene.y;
                enemigo2.mEney++;
            }
            if (mover2[1].second < enemigo2.mEnex) {
                pacbla = pacblaL;
                enemigo2.posEnex -= enemigo2.velene.x;
                enemigo2.mEnex--;
            }
            if (mover2[1].second > enemigo2.mEnex) {
                pacbla = pacblaR;
                enemigo2.posEnex += enemigo2.velene.x;
                enemigo2.mEnex++;
            }
            mover2.erase(mover2.begin());
        }
        if (!pastilla) {
            if (enemigo1.mEnex == posMx && enemigo1.mEney == posMy) {
                vidas--;
                fastest_route.clear();
                mover.clear();
                posMx = 10;
                posMy = 10;
                posJugx = 459;
                posJugy = 459;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 46;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 1;
            } else if (enemigo2.mEnex == posMx && enemigo2.mEney == posMy) {
                vidas--;
                posMx = 10;
                posMy = 10;
                posJugx = 459;
                posJugy = 459;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 46;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 1;
                enemigo2.posEnex = 416;
                enemigo2.posEney = 46;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 1;
            }
        }
        if (pastilla) {
            if (enemigo1.mEnex == posMx && enemigo1.mEney == posMy) {
                fastest_route.clear();
                mover.clear();
                puntaje2 += 50;
                respawn = true;

                enemigo1.posEnex = 460;
                enemigo1.posEney = 46;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 1;
            }
            if (enemigo2.mEnex == posMx && enemigo2.mEney == posMy) {
                fastest_route.clear();
                mover2.clear();
                puntaje2 += 50;
                respawn2 = true;

                enemigo2.posEnex = 416;
                enemigo2.posEney = 46;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 1;
            }
        }

        //En caso de que queramos mover el pacman de manera aleatoria

        if (!superP2[0].active && !superP2[1].active && !superP2[2].active && !superP2[3].active&& !superP2[4].active && !superP2[5].active){
            if (random_number == 0) {
                if (matriz2[enemigo2.mEney - 1][enemigo2.mEnex] != "#") {
                    pacbla = pacblaU;
                    enemigo2.posEney -= enemigo2.velene.y;
                    enemigo2.mEney--;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number == 1) {
                if (matriz2[enemigo2.mEney + 1][enemigo2.mEnex] != "#") {
                    pacbla = pacblaD;
                    enemigo2.posEney += enemigo2.velene.y;
                    enemigo2.mEney++;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number == 2) {
                if (matriz2[enemigo2.mEney][enemigo2.mEnex - 1] != "#") {
                    pacbla = pacblaL;
                    enemigo2.posEnex -= enemigo2.velene.x;
                    enemigo2.mEnex--;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number == 3) {
                if (matriz2[enemigo2.mEney][enemigo2.mEnex + 1] != "#") {
                    pacbla = pacblaR;
                    enemigo2.posEnex += enemigo2.velene.x;
                    enemigo2.mEnex++;
                } else {
                    random_number = rand() % 4;
                }
            }
        }
        delayTimer = 0.f;
    }

    if(puntaje2 == 200){
        superP2[0].active = true;
    }
    if(puntaje2 == 400){
        superP2[1].active = true;
    }
    if(puntaje2 == 600){
        superP2[2].active = true;
    }
    if(puntaje2 == 800){
        superP2[3].active = true;
    }
    if(puntaje2 == 1000){
        superP2[4].active = true;
    }
    if(puntaje2 == 1200){
        superP2[5].active = true;
    }
    for(int i = 0; i < 6; i++){
        if(superP2[i].active) {
            //cout << "Posicion enemigo: "<< enemigo2.mEnex<<"," <<enemigo2.mEney << endl;

            //cout << "Posicion pastilla: "<< superP2[i].MxSupP<<"," <<superP2[i].MySupP << endl;
            if (posMy == superP2[i].MySupP && posMx == superP2[i].MxSupP) {
                superP2[i].active = false;
                pastilla = true;
                cout << "Pastilla activada" << endl;
            }
            else if (enemigo2.mEney == superP2[i].MySupP && enemigo2.mEnex == superP2[i].MxSupP) {
                superP2[i].active = false;
                mover.clear();
                mover2.clear();
            }

            else if (enemigo1.mEney == superP2[i].MySupP && enemigo1.mEnex == superP2[i].MxSupP) {
                superP2[i].active = false;
                mover.clear();
                mover2.clear();
            }
        }
    }
}

/**
 *  Funcion que se encuentra dentro del while (!WindowShouldClose())
 *  al estar dentro del while el contador va a iniciar; nivel 3.
 */
void actJuego3(void) {
    delayTimer += 0.5f;
    if (respawn) {
        spawnTimer += 0.5f;
        enemigo1.posEnex = -50;
        enemigo1.posEney = -50;
        enemigo1.mEnex = 0;
        enemigo1.mEney = 0;
        if (spawnTimer >= spawnTimerMax) {
            respawn = false;
            spawnTimer = 0.f;
            enemigo1.posEnex = 460;
            enemigo1.posEney = 284;
            enemigo1.mEnex = 10;
            enemigo1.mEney = 6;

        }
    }
    if (respawn2) {
        spawnTimer2 += 0.5f;
        enemigo2.posEnex = -50;
        enemigo2.posEney = -50;
        enemigo2.mEnex = 0;
        enemigo2.mEney = 0;

        if (spawnTimer2 >= spawnTimerMax2) {
            respawn2 = false;
            spawnTimer2 = 0.f;
            enemigo2.posEnex = 416;
            enemigo2.posEney = 284;
            enemigo2.mEnex = 9;
            enemigo2.mEney = 6;

        }
    }
    if (respawn3) {
        spawnTimer3 += 0.5f;
        enemigo3.posEnex = -50;
        enemigo3.posEney = -50;
        enemigo3.mEnex = 0;
        enemigo3.mEney = 0;

        if (spawnTimer3 >= spawnTimerMax3) {
            respawn3 = false;
            spawnTimer3 = 0.f;
            enemigo3.posEnex = 506;
            enemigo3.posEney = 284;
            enemigo3.mEnex = 11;
            enemigo3.mEney = 6;

        }
    }
    if (pastilla) {
        pastillaDelay += 0.5f;
        if (pastillaDelay >= pastillaDelayMax) {
            pastilla = false;
            pastillaDelay = 0.f;
        }
    }


    if (matriz3[posMy - 1][posMx] != "#") {
        if (IsKeyPressed(KEY_UP)) {
            posJugy -= jugador1.velocidad.y;
            posMy--;
            if (!superP3[0].active && !superP3[1].active && !superP3[2].active && !superP3[3].active &&
                !superP3[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);

                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::vector<std::pair<int, int>> path = astarSearch(matriz3A, start, goal, visited, openList);
                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node *node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto &node: visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;


                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto &node: path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }

        }
        if (IsKeyPressed(KEY_UP)) {
            if (pastilla) { player = personajecheto; }
            else {
                if (perselect == "1") { player = personaje1U; }
                if (perselect == "2") { player = personaje2U; }
                if (perselect == "3") { player = personaje3U; }
                if (perselect == "4") { player = personaje4U; }
            }
        }
        if(mencel == "1")posJugy -= jugador1.velocidad.y;
        posMy--;
        if (!superP3[0].active && !superP3[1].active && !superP3[2].active && !superP3[3].active &&
            !superP3[4].active) {
            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);

            std::pair<int, int> goal = std::make_pair(posMy, posMx);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node *> visited(rows * cols, nullptr);
            std::vector<Node *> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;
            std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            std::cout << "Lista abierta:";

            while (!openList.empty()) {
                Node *node = openList.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada:";
            for (const auto &node: visited) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;


            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado:";
                for (const auto &node: path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();
            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            std::cout << "Lista abierta2:";
            while (!openList2.empty()) {
                Node *node = openList2.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList2.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada2:";
            for (const auto &node: visited2) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto &node: path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();
        }
        if(mencel == "1"){
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1U; }
                if (perselect == "2") { player = personaje2U; }
                if (perselect == "3") { player = personaje3U; }
                if (perselect == "4") { player = personaje4U; }
            }
        }
    }

    if (matriz3[posMy + 1][posMx] != "#") {
        if (IsKeyPressed(KEY_DOWN)) {
            posJugy += jugador1.velocidad.y;
            posMy++;
            if (!superP3[0].active && !superP3[1].active && !superP3[2].active && !superP3[3].active &&
                !superP3[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);

                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node *node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto &node: visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;


                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto &node: path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();
                }

            }
        if (IsKeyPressed(KEY_DOWN)) {
            if (pastilla) { player = personajecheto; }
            else {
                if (perselect == "1") { player = personaje1D; }
                if (perselect == "2") { player = personaje2D; }
                if (perselect == "3") { player = personaje3D; }
                if (perselect == "4") { player = personaje4D; }
            }
        }
        if (mencel == "2") {

            posJugy += jugador1.velocidad.y;
            posMy++;
            if (!superP3[0].active && !superP3[1].active && !superP3[2].active && !superP3[3].active &&
                !superP3[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);

                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node *node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto &node: visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;


                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto &node: path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();
            }
        }
        if (mencel == "2") {
            if (pastilla) { player = personajecheto; }
            else {
                if (perselect == "1") { player = personaje1D; }
                if (perselect == "2") { player = personaje2D; }
                if (perselect == "3") { player = personaje3D; }
                if (perselect == "4") { player = personaje4D; }

            }
        }
    }

    if (matriz3[posMy][posMx - 1] != "#") {
        if (IsKeyPressed(KEY_LEFT)) {
            posJugx -= jugador1.velocidad.x;
            posMx--;
            if (!superP3[0].active && !superP3[1].active && !superP3[2].active && !superP3[3].active &&
                !superP3[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);

                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                            std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node *node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto &node: visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;


                            if (!path.empty()) {
                                mover = path;
                                std::cout << "Camino encontrado:";
                                for (const auto &node: path) {
                                    std::cout << " (" << node.first << ", " << node.second << ")";
                                }
                                std::cout << std::endl;
                            }
                            path.clear();
                            visited.clear();
                            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
                            std::cout << "Lista abierta2:";
                            while (!openList2.empty()) {
                                Node *node = openList2.top();
                                std::cout << " (" << node->x << ", " << node->y << ")";
                                openList2.pop();
                            }
                            std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }
        }
        if (IsKeyPressed(KEY_LEFT)) {
            if (pastilla) { player = personajecheto; }
            else {
                if (perselect == "1") { player = personaje1L; }
                if (perselect == "2") { player = personaje2L; }
                if (perselect == "3") { player = personaje3L; }
                if (perselect == "4") { player = personaje4L; }
            }
        }
        if(mencel == "3"){
            posJugx -= jugador1.velocidad.x;
            posMx--;
            if (!superP3[0].active && !superP3[1].active && !superP3[2].active && !superP3[3].active &&
                !superP3[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);

                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node *node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto &node: visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;


                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto &node: path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }
        }
        if(mencel == "3"){
            if (pastilla) {player = personajecheto;}
            else{
                if(perselect == "1"){player = personaje1L;}
                if(perselect == "2"){player = personaje2L;}
                if(perselect == "3"){player = personaje3L;}
                if(perselect == "4"){player = personaje4L;}}

        }
    }

    if (matriz3[posMy][posMx + 1] != "#") {
        if (IsKeyPressed(KEY_RIGHT)) {
            posJugx += jugador1.velocidad.x;
            posMx++;
            if (!superP3[0].active && !superP3[1].active && !superP3[2].active && !superP3[3].active &&
                !superP3[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);

                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node *node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto &node: visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;


                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto &node: path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();
            }


        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (pastilla) { player = personajecheto; }
            else {
                if (perselect == "1") { player = personaje1R; }
                if (perselect == "2") { player = personaje2R; }
                if (perselect == "3") { player = personaje3R; }
                if (perselect == "4") { player = personaje4R; }
            }
        }
        if(mencel == "4"){
            posJugx += jugador1.velocidad.x;
            posMx++;
            if (!superP3[0].active && !superP3[1].active && !superP3[2].active && !superP3[3].active &&
                !superP3[4].active) {
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);

                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 12;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node *> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node *node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto &node: visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;


                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto &node: path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();
            }


        }


        if (mencel == "4") {
            if (pastilla) { player = personajecheto; }
            else {
                if (perselect == "1") { player = personaje1R; }
                if (perselect == "2") { player = personaje2R; }
                if (perselect == "3") { player = personaje3R; }
                if (perselect == "4") { player = personaje4R; }
            }
        }
    }
    for (int i = 0; i < cantP3; i++) {
        if (pastillas3[i].active) {
            if (abs(posJugx + 23 - pastillas3[i].rec.x) < 23) {
                if (abs(posJugy + 23 - pastillas3[i].rec.y) < 23) {
                    pastillas3[i].active = false;
                    puntos3++;
                    puntaje3 += 10;
                }
            }
        }
    }
    if (delayTimer >= delayTimerMax) {
        if (superP3[0].active && p0) {
            p0 = false;
            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP3[0].MySupP, superP3[0].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            std::cout << "Lista abierta:";
            while (!openList.empty()) {
                Node* node = openList.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada:";
            for (const auto& node : visited) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();


            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            std::cout << "Lista abierta2:";
            while (!openList2.empty()) {
                Node* node = openList2.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList2.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada2:";
            for (const auto& node : visited2) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();

            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo3.mEney, enemigo3.mEnex));
            backtrackE1M3(enemigo3.mEney, enemigo3.mEnex, route, 0);
            fastest_route.clear();
            route.clear();

        }
        if (superP3[1].active && p1) {
            p1 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP3[1].MySupP, superP3[1].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();



            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();
            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo3.mEney, enemigo3.mEnex));
            backtrackE1M3(enemigo3.mEney, enemigo3.mEnex, route, 1);
            fastest_route.clear();
            route.clear();

        }
        if (superP3[2].active && p2) {
            p2 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP3[2].MySupP, superP3[2].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();



            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();

            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo3.mEney, enemigo3.mEnex));
            backtrackE1M3(enemigo3.mEney, enemigo3.mEnex, route, 2);
            fastest_route.clear();
            route.clear();

        }
        if (superP3[3].active && p3) {
            p3 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP3[3].MySupP, superP3[3].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz2A, start, goal, visited, openList);
            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();



            std::vector<std::pair<int, int>> path2 = astarSearch(matriz2A, start2, goal, visited2, openList2);
            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();

            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo3.mEney, enemigo3.mEnex));
            backtrackE1M3(enemigo3.mEney, enemigo3.mEnex, route, 3);
            fastest_route.clear();
            route.clear();

        }
        if (superP3[4].active && p4) {
            p4 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> goal = std::make_pair(superP3[4].MySupP, superP3[4].MxSupP);

            int rows = 12;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;

            std::vector<std::pair<int, int>> path = astarSearch(matriz3A, start, goal, visited, openList);
            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();



            std::vector<std::pair<int, int>> path2 = astarSearch(matriz3A, start2, goal, visited2, openList2);
            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();

            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo3.mEney, enemigo3.mEnex));
            backtrackE1M3(enemigo3.mEney, enemigo3.mEnex, route, 4);
            fastest_route.clear();
            route.clear();

        }

        if (!mover.empty()) {
            if (mover[1].first < enemigo1.mEney) {
                pacama = pacamaU;
                enemigo1.posEney -= enemigo1.velene.y;
                enemigo1.mEney--;
            }
            if (mover[1].first > enemigo1.mEney) {
                pacama = pacamaD;
                enemigo1.posEney += enemigo1.velene.y;
                enemigo1.mEney++;
            }
            if (mover[1].second < enemigo1.mEnex) {
                pacama = pacamaL;
                enemigo1.posEnex -= enemigo1.velene.x;
                enemigo1.mEnex--;
            }
            if (mover[1].second > enemigo1.mEnex) {
                pacama = pacamaR;
                enemigo1.posEnex += enemigo1.velene.x;
                enemigo1.mEnex++;
            }
            mover.erase(mover.begin());
        }
        if (!mover2.empty()) {

            if (mover2[1].first < enemigo2.mEney) {
                pacbla = pacblaU;
                enemigo2.posEney -= enemigo2.velene.y;
                enemigo2.mEney--;
            }
            if (mover2[1].first > enemigo2.mEney) {
                pacbla = pacblaD;
                enemigo2.posEney += enemigo2.velene.y;
                enemigo2.mEney++;
            }
            if (mover2[1].second < enemigo2.mEnex) {
                pacbla = pacblaL;
                enemigo2.posEnex -= enemigo2.velene.x;
                enemigo2.mEnex--;
            }
            if (mover2[1].second > enemigo2.mEnex) {
                pacbla = pacblaR;
                enemigo2.posEnex += enemigo2.velene.x;
                enemigo2.mEnex++;
            }
            mover2.erase(mover2.begin());
        }
        if (!mover3.empty()) {

            if (mover3[1].first < enemigo3.mEney) {
                pacver = pacverU;
                enemigo3.posEney -= enemigo3.velene.y;
                enemigo3.mEney--;
            }
            if (mover3[1].first > enemigo3.mEney) {
                pacver = pacverD;
                enemigo3.posEney += enemigo3.velene.y;
                enemigo3.mEney++;
            }
            if (mover3[1].second < enemigo3.mEnex) {
                pacver = pacverL;
                enemigo3.posEnex -= enemigo3.velene.x;
                enemigo3.mEnex--;
            }
            if (mover3[1].second > enemigo3.mEnex) {
                pacver = pacverR;
                enemigo3.posEnex += enemigo3.velene.x;
                enemigo3.mEnex++;
            }
            mover3.erase(mover3.begin());
        }

        if (!pastilla) {
            if (enemigo1.mEnex == posMx && enemigo1.mEney == posMy) {
                vidas--;
                fastest_route.clear();
                mover.clear();
                mover2.clear();
                mover3.clear();
                posMx = 10;
                posMy = 10;
                posJugx = 459;
                posJugy = 459;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 284;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 6;
                enemigo2.posEnex = 416;
                enemigo2.posEney = 284;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 6;
                enemigo3.posEnex = 506;
                enemigo3.posEney = 284;
                enemigo3.mEnex = 11;
                enemigo3.mEney = 6;

            } else if (enemigo2.mEnex == posMx && enemigo2.mEney == posMy) {
                vidas--;
                fastest_route.clear();
                mover.clear();
                mover2.clear();
                mover3.clear();

                posMx = 10;
                posMy = 10;
                posJugx = 459;
                posJugy = 459;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 284;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 6;
                enemigo2.posEnex = 416;
                enemigo2.posEney = 284;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 6;
                enemigo3.posEnex = 506;
                enemigo3.posEney = 284;
                enemigo3.mEnex = 11;
                enemigo3.mEney = 6;
            } else if (enemigo3.mEnex == posMx && enemigo3.mEney == posMy) {
                vidas--;
                fastest_route.clear();
                mover.clear();
                mover2.clear();
                mover3.clear();

                posMx = 10;
                posMy = 10;
                posJugx = 459;
                posJugy = 459;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 284;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 6;
                enemigo2.posEnex = 416;
                enemigo2.posEney = 284;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 6;
                enemigo3.posEnex = 506;
                enemigo3.posEney = 284;
                enemigo3.mEnex = 11;
                enemigo3.mEney = 6;
            }
        }
        if (pastilla) {
            if (enemigo1.mEnex == posMx && enemigo1.mEney == posMy) {
                fastest_route.clear();
                mover.clear();
                puntaje2 += 50;
                respawn = true;

                enemigo1.posEnex = 460;
                enemigo1.posEney = 46;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 1;
            }
            if (enemigo2.mEnex == posMx && enemigo2.mEney == posMy) {
                fastest_route.clear();
                mover2.clear();
                puntaje2 += 50;
                respawn2 = true;

                enemigo2.posEnex = 416;
                enemigo2.posEney = 46;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 1;
            }
            if (enemigo3.mEnex == posMx && enemigo3.mEney == posMy) {
                fastest_route.clear();
                mover3.clear();

                puntaje2 += 50;
                respawn3 = true;

                enemigo3.posEnex = 506;
                enemigo3.posEney = 284;
                enemigo3.mEnex = 11;
                enemigo3.mEney = 6;

            }

        }
        //En caso de que queramos mover el pacman de manera aleatoria

        if (!superP3[0].active && !superP3[1].active && !superP3[2].active && !superP3[3].active&& !superP3[4].active){
            if (random_number == 0) {
                if (matriz3[enemigo3.mEney - 1][enemigo3.mEnex] != "#") {
                    pacver = pacverU;
                    enemigo3.posEney -= enemigo3.velene.y;
                    enemigo3.mEney--;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number == 1) {
                if (matriz3[enemigo3.mEney + 1][enemigo3.mEnex] != "#") {
                    pacver = pacverD;
                    enemigo3.posEney += enemigo3.velene.y;
                    enemigo3.mEney++;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number == 2) {
                if (matriz3[enemigo3.mEney][enemigo3.mEnex - 1] != "#") {
                    pacver = pacverL;
                    enemigo3.posEnex -= enemigo2.velene.x;
                    enemigo3.mEnex--;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number == 3) {
                if (matriz3[enemigo3.mEney][enemigo3.mEnex + 1] != "#") {
                    pacver = pacverR;
                    enemigo3.posEnex += enemigo3.velene.x;
                    enemigo3.mEnex++;
                } else {
                    random_number = rand() % 4;
                }
            }
        }

        delayTimer = 0.f;
    }
    if (puntaje3 == 200) {
        superP3[0].active = true;

    }
    if (puntaje3 == 400) {
        superP3[1].active = true;
    }
    if (puntaje3 == 600) {
        superP3[2].active = true;
    }
    if (puntaje3 == 800) {
        superP3[3].active = true;
    }
    if (puntaje3 == 1000) {
        superP3[4].active = true;
    }
    for (int i = 0; i < 5; i++) {
        if(superP3[i].active) {
            //cout << "Posicion enemigo: "<< enemigo2.mEnex<<"," <<enemigo2.mEney << endl;

            //cout << "Posicion pastilla: "<< superP2[i].MxSupP<<"," <<superP2[i].MySupP << endl;
            if (posMy == superP3[i].MySupP && posMx == superP3[i].MxSupP) {
                superP3[i].active = false;
                pastilla = true;
                cout << "Pastilla activada" << endl;
            }
            else if (enemigo2.mEney == superP3[i].MySupP && enemigo2.mEnex == superP3[i].MxSupP) {
                superP3[i].active = false;
                mover.clear();
                mover2.clear();
                mover3.clear();
            }

            else if (enemigo1.mEney == superP3[i].MySupP && enemigo1.mEnex == superP3[i].MxSupP) {
                superP3[i].active = false;
                mover.clear();
                mover2.clear();
                mover3.clear();

            }
            else if (enemigo3.mEney == superP3[i].MySupP && enemigo3.mEnex == superP3[i].MxSupP) {
                superP3[i].active = false;
                mover.clear();
                mover2.clear();
                mover3.clear();

            }
        }
    }
}

/**
 *  Funcion que se encuentra dentro del while (!WindowShouldClose())
 *  al estar dentro del while el contador va a iniciar; nivel 4.
 */
void actJuego4(void) {
    delayTimer += 0.5f;
    if (respawn) {
        spawnTimer += 0.5f;
        enemigo1.posEnex = -50;
        enemigo1.posEney = -50;
        enemigo1.mEnex = 0;
        enemigo1.mEney = 0;
        if (spawnTimer >= spawnTimerMax) {
            respawn = false;
            spawnTimer = 0.f;
            enemigo1.posEnex = 460;
            enemigo1.posEney = 284;
            enemigo1.mEnex = 10;
            enemigo1.mEney = 6;

        }
    }
    if (respawn2) {
        spawnTimer2 += 0.5f;
        enemigo2.posEnex = -50;
        enemigo2.posEney = -50;
        enemigo2.mEnex = 0;
        enemigo2.mEney = 0;

        if (spawnTimer2 >= spawnTimerMax2) {
            respawn2 = false;
            spawnTimer2 = 0.f;
            enemigo2.posEnex = 416;
            enemigo2.posEney = 284;
            enemigo2.mEnex = 9;
            enemigo2.mEney = 6;

        }
    }
    if (respawn3) {
        spawnTimer3 += 0.5f;
        enemigo3.posEnex = -50;
        enemigo3.posEney = -50;
        enemigo3.mEnex = 0;
        enemigo3.mEney = 0;

        if (spawnTimer3 >= spawnTimerMax3) {
            respawn3 = false;
            spawnTimer3 = 0.f;
            enemigo3.posEnex = 506;
            enemigo3.posEney = 284;
            enemigo3.mEnex = 11;
            enemigo3.mEney = 6;

        }
    }
    if (respawn4) {
        spawnTimer4 += 0.5f;
        enemigo4.posEnex = -50;
        enemigo4.posEney = -50;
        enemigo4.mEnex = 0;
        enemigo4.mEney = 0;

        if (spawnTimer4 >= spawnTimerMax4) {
            respawn4 = false;
            spawnTimer4 = 0.f;
            enemigo4.posEnex = 460;
            enemigo4.posEney = 284;
            enemigo4.mEnex = 10;
            enemigo4.mEney = 7;
        }
    }


    if (pastilla) {
        pastillaDelay += 0.5f;
        if (pastillaDelay >= pastillaDelayMax) {
            pastilla = false;
            pastillaDelay = 0.f;
        }
    }


    if (matriz4[posMy - 1][posMx] != "#") {
        if (IsKeyPressed(KEY_UP)) {
            posJugy -= jugador1.velocidad.y;
            posMy--;
            if (!superP4[0].active && !superP4[1].active && !superP4[2].active && !superP4[3].active&& !superP4[4].active && !superP4[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 14;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }

        }
        if (IsKeyPressed(KEY_UP)) {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1U; }
                if (perselect == "2") { player = personaje2U; }
                if (perselect == "3") { player = personaje3U; }
                if (perselect == "4") { player = personaje4U; }
            }
        }
        if (mencel == "1") {
            posJugy -= jugador1.velocidad.y;
            posMy--;
            if (!superP4[0].active && !superP4[1].active && !superP4[2].active && !superP4[3].active&& !superP4[4].active && !superP4[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 14;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }

        }
        if (mencel == "1") {
            if (pastilla) { player = personajecheto; }
            else {
                if (perselect == "1") { player = personaje1U; }
                if (perselect == "2") { player = personaje2U; }
                if (perselect == "3") { player = personaje3U; }
                if (perselect == "4") { player = personaje4U; }
            }
        }
    }

    if (matriz4[posMy + 1][posMx] != "#") {
        if (IsKeyPressed(KEY_DOWN)) {
            posJugy += jugador1.velocidad.y;
            posMy++;
            if (!superP4[0].active && !superP4[1].active && !superP4[2].active && !superP4[3].active&& !superP4[4].active && !superP4[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 14;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }
        }
        if (IsKeyPressed(KEY_DOWN)){
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1D; }
                if (perselect == "2") { player = personaje2D; }
                if (perselect == "3") { player = personaje3D; }
                if (perselect == "4") { player = personaje4D; }
            }
        }
        if (mencel == "2") {
            posJugy += jugador1.velocidad.y;
            posMy++;
            if (!superP4[0].active && !superP4[1].active && !superP4[2].active && !superP4[3].active&& !superP4[4].active && !superP4[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 14;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }

        }
        if (mencel == "2") {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1D; }
                if (perselect == "2") { player = personaje2D; }
                if (perselect == "3") { player = personaje3D; }
                if (perselect == "4") { player = personaje4D; }
            }
        };
    }

    if (matriz4[posMy][posMx - 1] != "#") {
        if (IsKeyPressed(KEY_LEFT)) {
            posJugx -= jugador1.velocidad.x;
            posMx--;
            if (!superP4[0].active && !superP4[1].active && !superP4[2].active && !superP4[3].active&& !superP4[4].active && !superP4[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 14;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }

        }
        if (IsKeyPressed(KEY_LEFT)) {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1L; }
                if (perselect == "2") { player = personaje2L; }
                if (perselect == "3") { player = personaje3L; }
                if (perselect == "4") { player = personaje4L; }
            }
        }
        if (mencel == "3") {
            posJugx -= jugador1.velocidad.x;
            posMx--;
            if (!superP4[0].active && !superP4[1].active && !superP4[2].active && !superP4[3].active&& !superP4[4].active && !superP4[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 14;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }
        }
        if (mencel == "3") {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1L; }
                if (perselect == "2") { player = personaje2L; }
                if (perselect == "3") { player = personaje3L; }
                if (perselect == "4") { player = personaje4L; }
            }
        }
    }

    if (matriz4[posMy][posMx + 1] != "#") {
        if (IsKeyPressed(KEY_RIGHT)) {
            posJugx += jugador1.velocidad.x;
            posMx++;
            if (!superP4[0].active && !superP4[1].active && !superP4[2].active && !superP4[3].active&& !superP4[4].active && !superP4[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 14;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1R; }
                if (perselect == "2") { player = personaje2R; }
                if (perselect == "3") { player = personaje3R; }
                if (perselect == "4") { player = personaje4R; }
            }
        }
        if (mencel == "4") {
            posJugx += jugador1.velocidad.x;
            posMx++;
            if (!superP4[0].active && !superP4[1].active && !superP4[2].active && !superP4[3].active&& !superP4[4].active && !superP4[5].active){
                std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
                std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
                std::pair<int, int> goal = std::make_pair(posMy, posMx);

                int rows = 14;
                int cols = 21;

                // Vector para almacenar los nodos visitados
                std::vector<Node*> visited(rows * cols, nullptr);
                std::vector<Node *> visited2(rows * cols, nullptr);

                // Cola de prioridad para la lista abierta
                std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
                std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList2;

                std::cout << "Lista abierta:";

                while (!openList.empty()) {
                    Node* node = openList.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada:";
                for (const auto& node : visited) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;
                std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
                if (!path.empty()) {
                    mover = path;
                    std::cout << "Camino encontrado:";
                    for (const auto& node : path) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path.clear();
                visited.clear();
                std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
                std::cout << "Lista abierta2:";
                while (!openList2.empty()) {
                    Node *node = openList2.top();
                    std::cout << " (" << node->x << ", " << node->y << ")";
                    openList2.pop();
                }
                std::cout << std::endl;

                std::cout << "Lista cerrada2:";
                for (const auto &node: visited2) {
                    if (node != nullptr) {
                        std::cout << " (" << node->x << ", " << node->y << ")";
                    }
                }
                std::cout << std::endl;

                if (!path2.empty()) {
                    mover2 = path2;
                    std::cout << "Camino encontrado2:";
                    for (const auto &node: path2) {
                        std::cout << " (" << node.first << ", " << node.second << ")";
                    }
                    std::cout << std::endl;
                }
                path2.clear();
                visited2.clear();

            }
        }
        if (mencel == "4") {
            if (pastilla) {player = personajecheto;}
            else {
                if (perselect == "1") { player = personaje1R; }
                if (perselect == "2") { player = personaje2R; }
                if (perselect == "3") { player = personaje3R; }
                if (perselect == "4") { player = personaje4R; }
            }
        }
    }
    for (int i = 0; i < cantP4; i++) {
        if (pastillas4[i].active) {
            if (abs(posJugx + 23 - pastillas4[i].rec.x) < 23) {
                if (abs(posJugy + 23 - pastillas4[i].rec.y) < 23) {
                    pastillas4[i].active = false;
                    puntos4++;
                    puntaje4 += 10;
                }
            }
        }
    }


    if (delayTimer >= delayTimerMax) {
        if (superP4[0].active && p0) {
            p0 = false;
            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> start3 = std::make_pair(enemigo3.mEney, enemigo3.mEnex);
            std::pair<int, int> start4 = std::make_pair(enemigo4.mEney, enemigo4.mEnex);

            std::pair<int, int> goal = std::make_pair(superP4[0].MySupP, superP4[0].MxSupP);

            int rows = 14;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);
            std::vector<Node*> visited3(rows * cols, nullptr);
            std::vector<Node*> visited4(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList3;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList4;

            std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
            std::cout << "Lista abierta:";
            while (!openList.empty()) {
                Node* node = openList.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada:";
            for (const auto& node : visited) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();


            std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
            std::cout << "Lista abierta2:";
            while (!openList2.empty()) {
                Node* node = openList2.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList2.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada2:";
            for (const auto& node : visited2) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


            std::vector<std::pair<int, int>> path3 = astarSearch(matriz4A, start3, goal, visited3, openList3);
            std::cout << "Lista abierta3:";
            while (!openList3.empty()) {
                Node* node = openList3.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList3.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada3:";
            for (const auto& node : visited3) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path3.empty()) {
                mover3 = path3;
                std::cout << "Camino encontrado3:";
                for (const auto& node : path3) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path3.clear();
            visited3.clear();

            std::vector<std::pair<int, int>> path4 = astarSearch(matriz4A, start4, goal, visited4, openList4);
            std::cout << "Lista abierta4:";
            while (!openList4.empty()) {
                Node* node = openList4.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList4.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada4:";
            for (const auto& node : visited4) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path4.empty()) {
                mover4 = path4;
                std::cout << "Camino encontrado4:";
                for (const auto& node : path4) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path4.clear();
            visited4.clear();
        }
        if (superP4[1].active && p1) {
            p1 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> start3 = std::make_pair(enemigo3.mEney, enemigo3.mEnex);
            std::pair<int, int> start4 = std::make_pair(enemigo4.mEney, enemigo4.mEnex);

            std::pair<int, int> goal = std::make_pair(superP4[1].MySupP, superP4[1].MxSupP);

            int rows = 14;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);
            std::vector<Node*> visited3(rows * cols, nullptr);
            std::vector<Node*> visited4(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList3;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList4;

            std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
            std::cout << "Lista abierta:";
            while (!openList.empty()) {
                Node* node = openList.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada:";
            for (const auto& node : visited) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();


            std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
            std::cout << "Lista abierta2:";
            while (!openList2.empty()) {
                Node* node = openList2.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList2.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada2:";
            for (const auto& node : visited2) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


            std::vector<std::pair<int, int>> path3 = astarSearch(matriz4A, start3, goal, visited3, openList3);
            std::cout << "Lista abierta3:";
            while (!openList3.empty()) {
                Node* node = openList3.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList3.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada3:";
            for (const auto& node : visited3) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path3.empty()) {
                mover3 = path3;
                std::cout << "Camino encontrado3:";
                for (const auto& node : path3) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path3.clear();
            visited3.clear();

            std::vector<std::pair<int, int>> path4 = astarSearch(matriz4A, start4, goal, visited4, openList4);
            std::cout << "Lista abierta4:";
            while (!openList4.empty()) {
                Node* node = openList4.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList4.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada4:";
            for (const auto& node : visited4) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path4.empty()) {
                mover4 = path4;
                std::cout << "Camino encontrado4:";
                for (const auto& node : path4) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path4.clear();
            visited4.clear();

        }
        if (superP4[2].active && p2) {
            p2 = false;
            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> start3 = std::make_pair(enemigo3.mEney, enemigo3.mEnex);
            std::pair<int, int> start4 = std::make_pair(enemigo4.mEney, enemigo4.mEnex);

            std::pair<int, int> goal = std::make_pair(superP4[2].MySupP, superP4[2].MxSupP);

            int rows = 14;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);
            std::vector<Node*> visited3(rows * cols, nullptr);
            std::vector<Node*> visited4(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList3;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList4;

            std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
            std::cout << "Lista abierta:";
            while (!openList.empty()) {
                Node* node = openList.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada:";
            for (const auto& node : visited) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();


            std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
            std::cout << "Lista abierta2:";
            while (!openList2.empty()) {
                Node* node = openList2.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList2.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada2:";
            for (const auto& node : visited2) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


            std::vector<std::pair<int, int>> path3 = astarSearch(matriz4A, start3, goal, visited3, openList3);
            std::cout << "Lista abierta3:";
            while (!openList3.empty()) {
                Node* node = openList3.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList3.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada3:";
            for (const auto& node : visited3) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path3.empty()) {
                mover3 = path3;
                std::cout << "Camino encontrado3:";
                for (const auto& node : path3) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path3.clear();
            visited3.clear();

            std::vector<std::pair<int, int>> path4 = astarSearch(matriz4A, start4, goal, visited4, openList4);
            std::cout << "Lista abierta4:";
            while (!openList4.empty()) {
                Node* node = openList4.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList4.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada4:";
            for (const auto& node : visited4) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path4.empty()) {
                mover4 = path4;
                std::cout << "Camino encontrado4:";
                for (const auto& node : path4) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path4.clear();
            visited4.clear();

        }
        if (superP4[3].active && p3) {
            p3 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> start3 = std::make_pair(enemigo3.mEney, enemigo3.mEnex);
            std::pair<int, int> start4 = std::make_pair(enemigo4.mEney, enemigo4.mEnex);

            std::pair<int, int> goal = std::make_pair(superP4[3].MySupP, superP4[3].MxSupP);

            int rows = 14;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);
            std::vector<Node*> visited3(rows * cols, nullptr);
            std::vector<Node*> visited4(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList3;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList4;

            std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
            std::cout << "Lista abierta:";
            while (!openList.empty()) {
                Node* node = openList.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada:";
            for (const auto& node : visited) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();


            std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
            std::cout << "Lista abierta2:";
            while (!openList2.empty()) {
                Node* node = openList2.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList2.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada2:";
            for (const auto& node : visited2) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


            std::vector<std::pair<int, int>> path3 = astarSearch(matriz4A, start3, goal, visited3, openList3);
            std::cout << "Lista abierta3:";
            while (!openList3.empty()) {
                Node* node = openList3.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList3.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada3:";
            for (const auto& node : visited3) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path3.empty()) {
                mover3 = path3;
                std::cout << "Camino encontrado3:";
                for (const auto& node : path3) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path3.clear();
            visited3.clear();

            std::vector<std::pair<int, int>> path4 = astarSearch(matriz4A, start4, goal, visited4, openList4);
            std::cout << "Lista abierta4:";
            while (!openList4.empty()) {
                Node* node = openList4.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList4.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada4:";
            for (const auto& node : visited4) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path4.empty()) {
                mover4 = path4;
                std::cout << "Camino encontrado4:";
                for (const auto& node : path4) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path4.clear();
            visited4.clear();

        }
        if (superP4[4].active && p4) {
            p4 = false;
            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> start3 = std::make_pair(enemigo3.mEney, enemigo3.mEnex);
            std::pair<int, int> start4 = std::make_pair(enemigo4.mEney, enemigo4.mEnex);

            std::pair<int, int> goal = std::make_pair(superP4[4].MySupP, superP4[4].MxSupP);

            int rows = 14;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);
            std::vector<Node*> visited3(rows * cols, nullptr);
            std::vector<Node*> visited4(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList3;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList4;

            std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
            std::cout << "Lista abierta:";
            while (!openList.empty()) {
                Node* node = openList.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada:";
            for (const auto& node : visited) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();


            std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
            std::cout << "Lista abierta2:";
            while (!openList2.empty()) {
                Node* node = openList2.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList2.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada2:";
            for (const auto& node : visited2) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


            std::vector<std::pair<int, int>> path3 = astarSearch(matriz4A, start3, goal, visited3, openList3);
            std::cout << "Lista abierta3:";
            while (!openList3.empty()) {
                Node* node = openList3.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList3.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada3:";
            for (const auto& node : visited3) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path3.empty()) {
                mover3 = path3;
                std::cout << "Camino encontrado3:";
                for (const auto& node : path3) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path3.clear();
            visited3.clear();

            std::vector<std::pair<int, int>> path4 = astarSearch(matriz4A, start4, goal, visited4, openList4);
            std::cout << "Lista abierta4:";
            while (!openList4.empty()) {
                Node* node = openList4.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList4.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada4:";
            for (const auto& node : visited4) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path4.empty()) {
                mover4 = path4;
                std::cout << "Camino encontrado4:";
                for (const auto& node : path4) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path4.clear();
            visited4.clear();
        }
        if (superP4[5].active && p5) {
            p5 = false;

            std::pair<int, int> start = std::make_pair(enemigo1.mEney, enemigo1.mEnex);
            std::pair<int, int> start2 = std::make_pair(enemigo2.mEney, enemigo2.mEnex);
            std::pair<int, int> start3 = std::make_pair(enemigo3.mEney, enemigo3.mEnex);
            std::pair<int, int> start4 = std::make_pair(enemigo4.mEney, enemigo4.mEnex);

            std::pair<int, int> goal = std::make_pair(superP4[5].MySupP, superP4[5].MxSupP);

            int rows = 14;
            int cols = 21;

            // Vector para almacenar los nodos visitados
            std::vector<Node*> visited(rows * cols, nullptr);
            std::vector<Node*> visited2(rows * cols, nullptr);
            std::vector<Node*> visited3(rows * cols, nullptr);
            std::vector<Node*> visited4(rows * cols, nullptr);

            // Cola de prioridad para la lista abierta
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList2;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList3;
            std::priority_queue<Node*, std::vector<Node*>, CompareNode> openList4;

            std::vector<std::pair<int, int>> path = astarSearch(matriz4A, start, goal, visited, openList);
            std::cout << "Lista abierta:";
            while (!openList.empty()) {
                Node* node = openList.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada:";
            for (const auto& node : visited) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path.empty()) {
                mover = path;
                std::cout << "Camino encontrado1:";
                for (const auto& node : path) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path.clear();
            visited.clear();


            std::vector<std::pair<int, int>> path2 = astarSearch(matriz4A, start2, goal, visited2, openList2);
            std::cout << "Lista abierta2:";
            while (!openList2.empty()) {
                Node* node = openList2.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList2.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada2:";
            for (const auto& node : visited2) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path2.empty()) {
                mover2 = path2;
                std::cout << "Camino encontrado2:";
                for (const auto& node : path2) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path2.clear();
            visited2.clear();


            std::vector<std::pair<int, int>> path3 = astarSearch(matriz4A, start3, goal, visited3, openList3);
            std::cout << "Lista abierta3:";
            while (!openList3.empty()) {
                Node* node = openList3.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList3.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada3:";
            for (const auto& node : visited3) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path3.empty()) {
                mover3 = path3;
                std::cout << "Camino encontrado3:";
                for (const auto& node : path3) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path3.clear();
            visited3.clear();

            std::vector<std::pair<int, int>> path4 = astarSearch(matriz4A, start4, goal, visited4, openList4);
            std::cout << "Lista abierta4:";
            while (!openList4.empty()) {
                Node* node = openList4.top();
                std::cout << " (" << node->x << ", " << node->y << ")";
                openList4.pop();
            }
            std::cout << std::endl;

            std::cout << "Lista cerrada4:";
            for (const auto& node : visited4) {
                if (node != nullptr) {
                    std::cout << " (" << node->x << ", " << node->y << ")";
                }
            }
            std::cout << std::endl;

            if (!path4.empty()) {
                mover4 = path4;
                std::cout << "Camino encontrado4:";
                for (const auto& node : path4) {
                    std::cout << " (" << node.first << ", " << node.second << ")";
                }
                std::cout << std::endl;
            }
            path4.clear();
            visited4.clear();
        }

        if (!mover.empty()) {
            if (mover[1].first < enemigo1.mEney) {
                pacama = pacamaU;
                enemigo1.posEney -= enemigo1.velene.y;
                enemigo1.mEney--;
            }
            if (mover[1].first > enemigo1.mEney) {
                pacama = pacamaD;
                enemigo1.posEney += enemigo1.velene.y;
                enemigo1.mEney++;
            }
            if (mover[1].second < enemigo1.mEnex) {
                pacama = pacamaL;
                enemigo1.posEnex -= enemigo1.velene.x;
                enemigo1.mEnex--;
            }
            if (mover[1].second > enemigo1.mEnex) {
                pacama = pacamaR;
                enemigo1.posEnex += enemigo1.velene.x;
                enemigo1.mEnex++;
            }
            mover.erase(mover.begin());
        }
        if (!mover2.empty()) {

            if (mover2[1].first < enemigo2.mEney) {
                pacbla = pacblaU;
                enemigo2.posEney -= enemigo2.velene.y;
                enemigo2.mEney--;
            }
            if (mover2[1].first > enemigo2.mEney) {
                pacbla = pacblaD;
                enemigo2.posEney += enemigo2.velene.y;
                enemigo2.mEney++;
            }
            if (mover2[1].second < enemigo2.mEnex) {
                pacbla = pacblaL;
                enemigo2.posEnex -= enemigo2.velene.x;
                enemigo2.mEnex--;
            }
            if (mover2[1].second > enemigo2.mEnex) {
                pacbla = pacblaR;
                enemigo2.posEnex += enemigo2.velene.x;
                enemigo2.mEnex++;
            }
            mover2.erase(mover2.begin());
        }
        if (!mover3.empty()) {

            if (mover3[1].first < enemigo3.mEney) {
                pacver = pacverU;
                enemigo3.posEney -= enemigo3.velene.y;
                enemigo3.mEney--;
            }
            if (mover3[1].first > enemigo3.mEney) {
                pacver = pacverD;
                enemigo3.posEney += enemigo3.velene.y;
                enemigo3.mEney++;
            }
            if (mover3[1].second < enemigo3.mEnex) {
                pacver = pacverL;
                enemigo3.posEnex -= enemigo3.velene.x;
                enemigo3.mEnex--;
            }
            if (mover3[1].second > enemigo3.mEnex) {
                pacver = pacverR;
                enemigo3.posEnex += enemigo3.velene.x;
                enemigo3.mEnex++;
            }
            mover3.erase(mover3.begin());
        }
        if (!mover4.empty()) {

            if (mover4[1].first < enemigo4.mEney) {
                pacmor = pacmorU;
                enemigo4.posEney -= enemigo4.velene.y;
                enemigo4.mEney--;
            }
            if (mover4[1].first > enemigo4.mEney) {
                pacmor = pacmorD;
                enemigo4.posEney += enemigo4.velene.y;
                enemigo4.mEney++;
            }
            if (mover4[1].second < enemigo4.mEnex) {
                pacmor = pacmorL;
                enemigo4.posEnex -= enemigo4.velene.x;
                enemigo4.mEnex--;
            }
            if (mover4[1].second > enemigo4.mEnex) {
                pacmor = pacmorR;
                enemigo4.posEnex += enemigo4.velene.x;
                enemigo4.mEnex++;
            }
            mover4.erase(mover4.begin());
        }




        if (!pastilla) {
            if (enemigo1.mEnex == posMx && enemigo1.mEney == posMy) {
                vidas--;
                fastest_route.clear();
                fastest_route2.clear();

                mover.clear();
                mover2.clear();
                mover3.clear();
                mover4.clear();

                posMx = 10;
                posMy = 9;
                posJugx = 459;
                posJugy = 375;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 234;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 6;
                enemigo2.posEnex = 416;
                enemigo2.posEney = 284;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 7;
                enemigo3.posEnex = 506;
                enemigo3.posEney = 284;
                enemigo3.mEnex = 11;
                enemigo3.mEney = 7;
                enemigo4.posEnex = 460;
                enemigo4.posEney = 284;
                enemigo4.mEnex = 10;
                enemigo4.mEney = 7;
            } else if (enemigo2.mEnex == posMx && enemigo2.mEney == posMy) {
                vidas--;
                fastest_route.clear();
                fastest_route2.clear();

                mover.clear();
                mover2.clear();
                mover3.clear();
                mover4.clear();

                posMx = 10;
                posMy = 9;
                posJugx = 459;
                posJugy = 375;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 234;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 6;
                enemigo2.posEnex = 416;
                enemigo2.posEney = 284;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 7;
                enemigo3.posEnex = 506;
                enemigo3.posEney = 284;
                enemigo3.mEnex = 11;
                enemigo3.mEney = 7;
                enemigo4.posEnex = 460;
                enemigo4.posEney = 284;
                enemigo4.mEnex = 10;
                enemigo4.mEney = 7;
            } else if (enemigo3.mEnex == posMx && enemigo3.mEney == posMy) {
                vidas--;
                fastest_route.clear();
                fastest_route2.clear();

                mover.clear();
                mover2.clear();
                mover3.clear();
                mover4.clear();

                posMx = 10;
                posMy = 9;
                posJugx = 459;
                posJugy = 375;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 234;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 6;
                enemigo2.posEnex = 416;
                enemigo2.posEney = 284;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 7;
                enemigo3.posEnex = 506;
                enemigo3.posEney = 284;
                enemigo3.mEnex = 11;
                enemigo3.mEney = 7;
                enemigo4.posEnex = 460;
                enemigo4.posEney = 284;
                enemigo4.mEnex = 10;
                enemigo4.mEney = 7;
            } else if (enemigo4.mEnex == posMx && enemigo4.mEney == posMy) {
                vidas--;
                fastest_route.clear();
                fastest_route2.clear();

                mover.clear();
                mover2.clear();
                mover3.clear();
                mover4.clear();

                posMx = 10;
                posMy = 9;
                posJugx = 459;
                posJugy = 375;
                enemigo1.posEnex = 460;
                enemigo1.posEney = 234;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 6;
                enemigo2.posEnex = 416;
                enemigo2.posEney = 284;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 7;
                enemigo3.posEnex = 506;
                enemigo3.posEney = 284;
                enemigo3.mEnex = 11;
                enemigo3.mEney = 7;
                enemigo4.posEnex = 460;
                enemigo4.posEney = 284;
                enemigo4.mEnex = 10;
                enemigo4.mEney = 7;
            }
        }
        if (pastilla) {
            if (enemigo1.mEnex == posMx && enemigo1.mEney == posMy) {
                fastest_route.clear();
                mover.clear();
                respawn = true;

                puntaje4 += 50;

                enemigo1.posEnex = 460;
                enemigo1.posEney = 234;
                enemigo1.mEnex = 10;
                enemigo1.mEney = 6;
            }
            if (enemigo2.mEnex == posMx && enemigo2.mEney == posMy) {
                fastest_route.clear();
                mover2.clear();
                respawn2 = true;

                puntaje4 += 50;
                enemigo2.posEnex = 416;
                enemigo2.posEney = 284;
                enemigo2.mEnex = 9;
                enemigo2.mEney = 7;
            }
            if (enemigo3.mEnex == posMx && enemigo3.mEney == posMy) {
                fastest_route.clear();
                mover3.clear();
                respawn3 = true;

                puntaje4 += 50;
                enemigo3.posEnex = 506;
                enemigo3.posEney = 284;
                enemigo3.mEnex = 11;
                enemigo3.mEney = 7;
            }
            if (enemigo4.mEnex == posMx && enemigo4.mEney == posMy) {
                fastest_route.clear();
                mover4.clear();
                respawn4 = true;

                puntaje4 += 50;
                enemigo4.posEnex = 460;
                enemigo4.posEney = 284;
                enemigo4.mEnex = 10;
                enemigo4.mEney = 7;
            }
        }
        //En caso de que queramos mover el pacman de manera aleatoria

        if (!superP4[0].active && !superP4[1].active && !superP4[2].active && !superP4[3].active&& !superP4[4].active&& !superP4[5].active){
            if (random_number == 0) {
                if (matriz4[enemigo3.mEney - 1][enemigo3.mEnex] != "#") {
                    pacver = pacverU;
                    enemigo3.posEney -= enemigo3.velene.y;
                    enemigo3.mEney--;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number == 1) {
                if (matriz4[enemigo3.mEney + 1][enemigo3.mEnex] != "#") {
                    pacver = pacverD;
                    enemigo3.posEney += enemigo3.velene.y;
                    enemigo3.mEney++;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number == 2) {
                if (matriz4[enemigo3.mEney][enemigo3.mEnex - 1] != "#") {
                    pacver = pacverL;
                    enemigo3.posEnex -= enemigo2.velene.x;
                    enemigo3.mEnex--;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number == 3) {
                if (matriz4[enemigo3.mEney][enemigo3.mEnex + 1] != "#") {
                    pacver = pacverR;
                    enemigo3.posEnex += enemigo3.velene.x;
                    enemigo3.mEnex++;
                } else {
                    random_number = rand() % 4;
                }
            }
            if (random_number2 == 0) {
                if (matriz4[enemigo4.mEney - 1][enemigo4.mEnex] != "#") {
                    pacmor = pacmorU;
                    enemigo4.posEney -= enemigo4.velene.y;
                    enemigo4.mEney--;
                } else {
                    random_number2 = rand() % 4;
                }
            }
            if (random_number2 == 1) {
                if (matriz4[enemigo4.mEney + 1][enemigo4.mEnex] != "#") {
                    pacmor = pacmorD;
                    enemigo4.posEney += enemigo4.velene.y;
                    enemigo4.mEney++;
                } else {
                    random_number2 = rand() % 4;
                }
            }
            if (random_number2 == 2) {
                if (matriz4[enemigo4.mEney][enemigo4.mEnex - 1] != "#") {
                    pacmor = pacmorL;
                    enemigo4.posEnex -= enemigo4.velene.x;
                    enemigo4.mEnex--;
                } else {
                    random_number2 = rand() % 4;
                }
            }
            if (random_number2 == 3) {
                if (matriz4[enemigo4.mEney][enemigo4.mEnex + 1] != "#") {
                    pacmor = pacmorR;
                    enemigo4.posEnex += enemigo4.velene.x;
                    enemigo4.mEnex++;
                } else {
                    random_number2 = rand() % 4;
                }
            }
            if (random_number3 == 0) {
                if (matriz4[enemigo2.mEney - 1][enemigo2.mEnex] != "#") {
                    pacbla = pacblaU;
                    enemigo2.posEney -= enemigo2.velene.y;
                    enemigo2.mEney--;
                } else {
                    random_number3 = rand() % 4;
                }
            }
            if (random_number3 == 1) {
                if (matriz4[enemigo2.mEney + 1][enemigo2.mEnex] != "#") {
                    pacbla = pacblaD;
                    enemigo2.posEney += enemigo2.velene.y;
                    enemigo2.mEney++;
                } else {
                    random_number3 = rand() % 4;
                }
            }
            if (random_number3 == 2) {
                if (matriz4[enemigo2.mEney][enemigo2.mEnex - 1] != "#") {
                    pacbla = pacblaL;
                    enemigo2.posEnex -= enemigo2.velene.x;
                    enemigo2.mEnex--;
                } else {
                    random_number3 = rand() % 4;
                }
            }
            if (random_number3 == 3) {
                if (matriz4[enemigo2.mEney][enemigo2.mEnex + 1] != "#") {
                    pacbla = pacblaR;
                    enemigo2.posEnex += enemigo2.velene.x;
                    enemigo2.mEnex++;
                } else {
                    random_number3 = rand() % 4;
                }
            }

        }

        delayTimer = 0.f;

    }
    if(puntaje4 == 200){
        superP4[0].active = true;
    }
    if(puntaje4 == 400){
        superP4[1].active = true;
    }
    if(puntaje4 == 600){
        superP4[2].active = true;
    }
    if(puntaje4 == 800){
        superP4[3].active = true;
    }
    if(puntaje4 == 1000){
        superP4[4].active = true;
    }
    if(puntaje4 == 1200){
        superP4[5].active = true;
    }
    for(int i = 0; i < 6; i++){
        if(superP4[i].active) {
            //cout << "Posicion enemigo: "<< enemigo2.mEnex<<"," <<enemigo2.mEney << endl;

            //cout << "Posicion pastilla: "<< superP2[i].MxSupP<<"," <<superP2[i].MySupP << endl;
            if (posMy == superP4[i].MySupP && posMx == superP4[i].MxSupP) {
                superP4[i].active = false;
                pastilla = true;
                cout << "Pastilla activada" << endl;
            }
            else if (enemigo2.mEney == superP4[i].MySupP && enemigo2.mEnex == superP4[i].MxSupP) {
                superP4[i].active = false;
                mover.clear();
                mover2.clear();
                mover3.clear();
                mover4.clear();
            }

            else if (enemigo1.mEney == superP4[i].MySupP && enemigo1.mEnex == superP4[i].MxSupP) {
                superP4[i].active = false;
                mover.clear();
                mover2.clear();
                mover3.clear();
                mover4.clear();

            }
            else if (enemigo3.mEney == superP4[i].MySupP && enemigo3.mEnex == superP4[i].MxSupP) {
                superP4[i].active = false;
                mover.clear();
                mover2.clear();
                mover3.clear();
                mover4.clear();

            }
            else if (enemigo4.mEney == superP4[i].MySupP && enemigo4.mEnex == superP4[i].MxSupP) {
                superP4[i].active = false;
                mover.clear();
                mover2.clear();
                mover3.clear();
                mover4.clear();

            }

        }
    }
}
/**
 * Función que inicializa los sockets para iniciar la conexión con la app del celular y controlar al jugador con el giroscopio
 */
/*
void celular() {
    try {

        while (true) {

            boost::asio::read_until(socket, buffer, "\n", error);

            if (error == boost::asio::error::eof) {
                break; // Connection closed cleanly by peer.
            } else if (error) {
                throw boost::system::system_error(error); // Some other error.
            }

            string message(boost::asio::buffer_cast<const char *>(buffer.data()), buffer.size());
            string response = to_string(puntaje1);
//            cout<<"posicion m x: " <<posMx<<endl;
//            cout<<"posicion m y: " <<posMy<<endl;
            //size_t received_len = read_socket(received, sizeof(received));
            int startind = message.find("\n");
            if (startind != std::string::npos) {
                message.erase(startind, message.length() - startind);
            }
            //boost::asio::write(socket, boost::asio::buffer("Hola\n"));

            std::cout << "Received message: " << message << std::endl;
            std::cout << "Mensaje enviado: " << response << std::endl;

            mencel = message;

            buffer.consume(buffer.size());

        }
    }
    catch (std::exception & e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
}
*/

/**
 * Función Main, utilizada para iniciar el código, abrir la ventana inicial del juego, dibujar texturas, actualizar y
 * llevar la comunicación de ventanas, actualización de sprites, etc.
 * @return
 */
int main() {
/*
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor(io_context,
                                            boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("192.168.100.213"), 5001));

    std::cout << "Waiting for incoming connection..." << std::endl;

    boost::asio::ip::tcp::socket socket(io_context);
    acceptor.accept(socket);

    std::cout << "Connection accepted." << std::endl;

    boost::asio::streambuf buffer;
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer("hola\n"));
*/

    srand(time(0));
    random_number = rand() % 4;
    random_number2 = rand() % 4;
    random_number3 = rand() % 4;

    const int screenWidth = 960;
    const int screenHeight = 540;
    //thread cel(celular);
    InitWindow(screenWidth, screenHeight, "-APACALYPSE-");
    GameScreen currentScreen = INICIO;

    Rectangle boton1 = {380, 330, 200, 70};
    Rectangle per1 = {55, 285, 130, 142};
    Rectangle per2 = {300, 285, 130, 142};
    Rectangle per3 = {535, 285, 130, 142};
    Rectangle per4 = {780, 285, 130, 142};
    Rectangle salir = {830,15,90,50};


    Texture2D fondoI = LoadTexture("Imagenes/Apacalypse.png");

    Texture2D fondoS = LoadTexture("Imagenes/fondo0.png");
    Texture2D fondo0 = LoadTexture("Imagenes/fondo0.png");
    Texture2D fondo1 = LoadTexture("Imagenes/fondo1.png");
    Texture2D fondo2 = LoadTexture("Imagenes/fondo2.png");
    Texture2D fondo3 = LoadTexture("Imagenes/fondo3.png");
    Texture2D fondo4 = LoadTexture("Imagenes/fondo4.png");

    Texture2D fondoN1 = LoadTexture("Imagenes/fondoj1.png");
    Texture2D fondoN2 = LoadTexture("Imagenes/fondoj2.png");
    Texture2D fondoN3 = LoadTexture("Imagenes/fondoj3.png");
    Texture2D fondoN4 = LoadTexture("Imagenes/fondoj4.png");

    Texture2D corazon = LoadTexture("Imagenes/heart.png");

    personaje1U = LoadTexture("ImagenesPer/Per1U.png");
    personaje1D = LoadTexture("ImagenesPer/Per1D.png");
    personaje1L = LoadTexture("ImagenesPer/Per1L.png");
    personaje1R = LoadTexture("ImagenesPer/Per1R.png");

    personaje2U = LoadTexture("ImagenesPer/Per2U.png");
    personaje2D = LoadTexture("ImagenesPer/Per2D.png");
    personaje2L = LoadTexture("ImagenesPer/Per2L.png");
    personaje2R = LoadTexture("ImagenesPer/Per2R.png");

    personaje3U = LoadTexture("ImagenesPer/Per3U.png");
    personaje3D = LoadTexture("ImagenesPer/Per3D.png");
    personaje3L = LoadTexture("ImagenesPer/Per3L.png");
    personaje3R = LoadTexture("ImagenesPer/Per3R.png");

    personaje4U = LoadTexture("ImagenesPer/Per4U.png");
    personaje4D = LoadTexture("ImagenesPer/Per4D.png");
    personaje4L = LoadTexture("ImagenesPer/Per4L.png");
    personaje4R = LoadTexture("ImagenesPer/Per4R.png");

    personajecheto = LoadTexture("ImagenesPer/PerPastilla.png");

    pacama = LoadTexture("ImagenesPac/PacD11.png");
    pacamaU = LoadTexture("ImagenesPac/PacU11.png");
    pacamaD = LoadTexture("ImagenesPac/PacD11.png");
    pacamaL = LoadTexture("ImagenesPac/PacL11.png");
    pacamaR = LoadTexture("ImagenesPac/PacR11.png");

    pacbla = LoadTexture("ImagenesPac/PacD21.png");
    pacblaU = LoadTexture("ImagenesPac/PacU21.png");
    pacblaD = LoadTexture("ImagenesPac/PacD21.png");
    pacblaL = LoadTexture("ImagenesPac/PacL21.png");
    pacblaR = LoadTexture("ImagenesPac/PacR21.png");

    pacver = LoadTexture("ImagenesPac/PacD31.png");
    pacverU = LoadTexture("ImagenesPac/PacU31.png");
    pacverD = LoadTexture("ImagenesPac/PacD31.png");
    pacverL = LoadTexture("ImagenesPac/PacL31.png");
    pacverR = LoadTexture("ImagenesPac/PacR31.png");

    pacmor = LoadTexture("ImagenesPac/PacD41.png");
    pacmorU = LoadTexture("ImagenesPac/PacU41.png");
    pacmorD = LoadTexture("ImagenesPac/PacD41.png");
    pacmorL = LoadTexture("ImagenesPac/PacL41.png");
    pacmorR = LoadTexture("ImagenesPac/PacR41.png");

    Texture2D win = LoadTexture("Imagenes/win.png");
    Texture2D lose = LoadTexture("Imagenes/lose.png");


    SetTargetFPS(120);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
/*
        if (socket.available() > 0) {
            boost::asio::read_until(socket, buffer, "\n", error);

            if (error == boost::asio::error::eof) {
                break; // Connection closed cleanly by peer.
            } else if (error) {
                throw boost::system::system_error(error); // Some other error.
            }

            string message(boost::asio::buffer_cast<const char *>(buffer.data()), buffer.size());
            //string response = to_string(puntaje1);
            int startind = message.find("\n");
            if (startind != std::string::npos) {
                message.erase(startind, message.length() - startind);
            }

           // std::cout << "Received message: " << message << std::endl;
            //std::cout << "Mensaje enviado: " << response << std::endl;

            mencel = message;

            buffer.consume(buffer.size());
        }
*/


        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case INICIO:
            {
                if (CheckCollisionPointRec(GetMousePosition(), boton1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = SELECCION;
                }
            } break;
            case SELECCION:
            {
                if (CheckCollisionPointRec(GetMousePosition(), per1)){
                    fondoS = fondo1;
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = NIVEL1;
                        perselect = "1";
                        player = LoadTexture("ImagenesPer/Per1D.png");
                        juego();
                    }
                }
                else if (CheckCollisionPointRec(GetMousePosition(), per2)){
                    fondoS = fondo2;
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = NIVEL1;
                        perselect = "2";
                        player = LoadTexture("ImagenesPer/Per2D.png");
                        juego();
                    }
                }
                else if (CheckCollisionPointRec(GetMousePosition(), per3)){
                    fondoS = fondo3;
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = NIVEL1;
                        perselect = "3";
                        player = LoadTexture("ImagenesPer/Per3D.png");
                        juego();
                    }
                }
                else if (CheckCollisionPointRec(GetMousePosition(), per4)){
                    fondoS = fondo4;
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = NIVEL1;
                        perselect = "4";
                        player = LoadTexture("ImagenesPer/Per4D.png");
                        juego();
                    }
                }
                else{
                    fondoS = fondo0;
                }
            } break;
            case NIVEL1:
            {
                actJuego1();

                if(puntos1 == 1){
                     p0 = true;
                     p1 = true;
                     p2 = true;
                     p3 = true;
                     p4 = true;
                     p5 = true;

                    currentScreen = NIVEL2;
                    fastest_route.clear();
                    mover.clear();

                    posMx = 10;
                    posMy = 10;
                    posJugx = 459;
                    posJugy = 459;

                    enemigo1.velene.x = 46;
                    enemigo1.velene.y = 46;
                    enemigo1.posEnex = 460;
                    enemigo1.posEney = 52;
                    enemigo1.mEnex = 10;
                    enemigo1.mEney = 1;
                    enemigo2.velene.x = 46;
                    enemigo2.velene.y = 46;
                    enemigo2.posEnex = 416;
                    enemigo2.posEney = 52;
                    enemigo2.mEnex = 9;
                    enemigo2.mEney = 1;
                }
                if(vidas == 0){
                    currentScreen = PERDER;
                }
            } break;
            case NIVEL2:
            {
                actJuego2();
                if(puntos2 == 1){
                    p0 = true;
                    p1 = true;
                    p2 = true;
                    p3 = true;
                    p4 = true;
                    p5 = true;

                    currentScreen = NIVEL3;
                    fastest_route.clear();
                    mover.clear();

                    posMx = 10;
                    posMy = 10;
                    posJugx = 459;
                    posJugy = 459;

                    enemigo1.velene.x = 46;
                    enemigo1.velene.y = 46;
                    enemigo1.posEnex = 460;
                    enemigo1.posEney = 284;
                    enemigo1.mEnex = 10;
                    enemigo1.mEney = 6;
                    enemigo2.velene.x = 46;
                    enemigo2.velene.y = 46;
                    enemigo2.posEnex = 416;
                    enemigo2.posEney = 284;
                    enemigo2.mEnex = 9;
                    enemigo2.mEney = 6;
                    enemigo3.velene.x = 46;
                    enemigo3.velene.y = 46;
                    enemigo3.posEnex = 506;
                    enemigo3.posEney = 284;
                    enemigo3.mEnex = 11;
                    enemigo3.mEney = 6;
                }
                if(vidas == 0){
                    currentScreen = PERDER;
                }
            } break;
            case NIVEL3:
            {
                actJuego3();
                if(puntos3 == 1){
                    p0 = true;
                    p1 = true;
                    p2 = true;
                    p3 = true;
                    p4 = true;
                    p5 = true;

                    currentScreen = NIVEL4;
                    posMx = 10;
                    posMy = 9;
                    posJugx = 459;
                    posJugy = 375;

                    enemigo1.velene.x = 46;
                    enemigo1.velene.y = 46;
                    enemigo1.posEnex = 460;
                    enemigo1.posEney = 234;
                    enemigo1.mEnex = 10;
                    enemigo1.mEney = 6;
                    enemigo2.velene.x = 46;
                    enemigo2.velene.y = 46;
                    enemigo2.posEnex = 416;
                    enemigo2.posEney = 284;
                    enemigo2.mEnex = 9;
                    enemigo2.mEney = 7;
                    enemigo3.velene.x = 46;
                    enemigo3.velene.y = 46;
                    enemigo3.posEnex = 506;
                    enemigo3.posEney = 284;
                    enemigo3.mEnex = 11;
                    enemigo3.mEney = 7;
                    enemigo4.velene.x = 46;
                    enemigo4.velene.y = 46;
                    enemigo4.posEnex = 460;
                    enemigo4.posEney = 284;
                    enemigo4.mEnex = 10;
                    enemigo4.mEney = 7;
                }
                if(vidas == 0){
                    currentScreen = PERDER;
                }
            } break;
            case NIVEL4:
            {
                actJuego4();
                if(puntos4 == cantP4){
                    currentScreen = GANAR;
                }
                if(vidas == 0){
                    currentScreen = PERDER;
                }
            } break;
            case GANAR:
            {
                if (CheckCollisionPointRec(GetMousePosition(), salir) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    CloseWindow();
                }
            } break;
            case PERDER:
            {
                if (CheckCollisionPointRec(GetMousePosition(), salir) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    CloseWindow();
                }
            } break;
            default: break;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case INICIO:
            {
                DrawRectangle(boton1.x,boton1.y,boton1.width,boton1.height, WHITE);
                DrawTexture(fondoI, 0, 0, RAYWHITE);

            } break;
            case SELECCION:
            {
                DrawRectangle(per1.x,per1.y,per1.width,per1.height, WHITE);
                DrawRectangle(per2.x,per2.y,per2.width,per2.height, WHITE);
                DrawRectangle(per3.x,per3.y,per3.width,per3.height, WHITE);
                DrawRectangle(per4.x,per4.y,per4.width,per4.height, WHITE);
                DrawTexture(fondoS, 0, 0, RAYWHITE);

            } break;

            case NIVEL1:
            {
                DrawTexture(fondoN1, 0, 0, RAYWHITE);
                DrawRectangle(280,10,400,30,BLACK);
                DrawText("SCORE:",300,15,20,GRAY);
                DrawText(TextFormat("%04i", puntaje1), 380, 15, 20, GRAY);
                DrawTexture(corazon, 450,10,RAYWHITE);
                DrawText("x",483,13,20,GRAY);
                DrawText(TextFormat("%02i",vidas),500,15,20,GRAY);
                DrawText("LEVEL 1",570,15,20,GRAY);
                DrawTexture(player, posJugx,posJugy, RAYWHITE);
                for(int i = 0; i < cantP1; i++) {
                    if(pastillas1[i].active){
                        DrawRectangleRec(pastillas1[i].rec,YELLOW);
                    }
                }
                for(int i = 0; i < 5; i++) {
                    if(superP1[i].active){
                        DrawRectangleRec(superP1[i].rec,RED);
                    }
                }
                DrawTexture(pacama, enemigo1.posEnex, enemigo1.posEney, RAYWHITE);
            } break;
            case NIVEL2:
            {
                DrawTexture(fondoN2, 0, 0, LIGHTGRAY);
                DrawRectangle(280,10,400,30,GRAY);
                DrawText("SCORE:",300,15,20,WHITE);
                DrawText(TextFormat("%04i", puntaje2), 380, 15, 20, WHITE);
                DrawTexture(corazon, 450,10,RAYWHITE);
                DrawText("x",483,13,20,WHITE);
                DrawText(TextFormat("%02i",vidas),500,15,20,WHITE);
                DrawText("LEVEL 2",570,15,20,WHITE);
                DrawTexture(player, posJugx,posJugy, RAYWHITE);

                for(int i = 0; i < cantP2; i++) {
                    if(pastillas2[i].active){
                        DrawRectangleRec(pastillas2[i].rec,YELLOW);
                    }
                }
                for(int i = 0; i < 6; i++) {
                    if(superP2[i].active){
                        DrawRectangleRec(superP2[i].rec,RED);
                    }
                }
                DrawTexture(pacama, enemigo1.posEnex, enemigo1.posEney, RAYWHITE);
                DrawTexture(pacbla, enemigo2.posEnex, enemigo2.posEney, RAYWHITE);

            } break;
            case NIVEL3:
            {
                DrawTexture(fondoN3, 0, 0, RAYWHITE);
                DrawRectangle(280,10,400,30,GRAY);
                DrawText("SCORE:",300,15,20,WHITE);
                DrawText(TextFormat("%04i", puntaje3), 380, 15, 20, WHITE);
                DrawTexture(corazon, 450,10,RAYWHITE);
                DrawText("x",483,13,20,WHITE);
                DrawText(TextFormat("%02i",vidas),500,15,20,WHITE);
                DrawText("LEVEL 3",570,15,20,WHITE);
                DrawTexture(player, posJugx,posJugy, RAYWHITE);

                for(int i = 0; i < cantP3; i++) {
                    if(pastillas3[i].active){
                        DrawRectangleRec(pastillas3[i].rec,YELLOW);
                    }
                }
                for(int i = 0; i < 5; i++) {
                    if(superP3[i].active){
                        DrawRectangleRec(superP3[i].rec,RED);
                    }
                }
                DrawTexture(pacama, enemigo1.posEnex, enemigo1.posEney, RAYWHITE);
                DrawTexture(pacbla, enemigo2.posEnex, enemigo2.posEney, RAYWHITE);
                DrawTexture(pacver, enemigo3.posEnex, enemigo3.posEney, RAYWHITE);

            } break;
            case NIVEL4:
            {
                DrawTexture(fondoN4, 0, 0, RAYWHITE);
                DrawRectangle(280,10,400,30,BLACK);
                DrawText("SCORE:",300,15,20,GRAY);
                DrawText(TextFormat("%04i", puntaje4), 380, 15, 20, GRAY);
                DrawTexture(corazon, 450,10,RAYWHITE);
                DrawText("x",483,13,20,GRAY);
                DrawText(TextFormat("%02i",vidas),500,15,20,GRAY);
                DrawText("LEVEL 4",570,15,20,GRAY);
                DrawTexture(player, posJugx,posJugy, RAYWHITE);

                for(int i = 0; i < cantP4; i++) {
                    if(pastillas4[i].active){
                        DrawRectangleRec(pastillas4[i].rec,YELLOW);
                    }
                }for(int i = 0; i < 6; i++) {
                    if(superP4[i].active){
                        DrawRectangleRec(superP4[i].rec,RED);
                    }
                }
                DrawTexture(pacama, enemigo1.posEnex, enemigo1.posEney, RAYWHITE);
                DrawTexture(pacbla, enemigo2.posEnex, enemigo2.posEney, RAYWHITE);
                DrawTexture(pacver, enemigo3.posEnex, enemigo3.posEney, RAYWHITE);
                DrawTexture(pacmor, enemigo4.posEnex, enemigo4.posEney, RAYWHITE);

            } break;
            case GANAR:
            {
                DrawRectangle(salir.x,salir.y,salir.width,salir.height, WHITE);
                DrawTexture(win, 0, 0, RAYWHITE);
                DrawText(TextFormat("%05i", puntaje1+puntaje2+puntaje3+puntaje4),480,468,40,SKYBLUE);

            } break;
            case PERDER:
            {
                DrawRectangle(salir.x,salir.y,salir.width,salir.height, WHITE);
                DrawTexture(lose, 0, 0, RAYWHITE);
                DrawText(TextFormat("%05i", puntaje1+puntaje2+puntaje3+puntaje4),480,468,40,PINK);

            } break;
            default: break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    /**
     * Unload de todos los sprites, musica, arreglos para los xml, etc.
     */
    // De-Initialization
    //--------------------------------------------------------------------------------------
        UnloadTexture(fondoI);
        UnloadTexture(fondoS);
        UnloadTexture(fondo0);
        UnloadTexture(fondo1);
        UnloadTexture(fondo2);
        UnloadTexture(fondo3);
        UnloadTexture(fondo4);
        UnloadTexture(fondoN1);
        UnloadTexture(fondoN2);
        UnloadTexture(fondoN3);
        UnloadTexture(fondoN4);

        UnloadTexture(corazon);
        UnloadTexture(pacama);


    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}