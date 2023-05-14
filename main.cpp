#include <iostream>
#include <raylib.h>
#include <boost/asio.hpp>
#include <thread>
#include <ctime>
#include <cstdlib>

using boost::asio::ip::tcp;

static boost::asio::io_context io_context;

using namespace std;

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
typedef struct Enemigos{
    Vector2 velene;
    int posEnex;
    int posEney;
    int mEnex;
    int mEney;
    bool activex;
    bool activey;
}Enemigos;

typedef struct Pastillas{
    Rectangle rec;
    bool active;
    int numero;
}Pastillas;

static float delayTimerMax = 15.f;
static float delayTimer = 0.f;

float MoverTimerMax = 20.f;
float MoverTimer = 0.f;
static int cont1 = 0;
static int random_number;
const int ROWS = 12;
const int COLS = 21;

// Definimos una estructura para representar las coordenadas en la matriz.
struct Coord {
    int row;
    int col;
};

// Vector para almacenar la ruta más rápida
std::vector<std::pair<int, int>> fastest_route;
std::vector<std::pair<int, int>> mover;


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

static string matriz2[12][21] = {
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

static string matriz3[12][21] = {
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

static string matriz4[14][21] = {
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


static string mencel;

static int posJugx;
static int posJugy;
static Jugador jugador1 = { 0 };
static Enemigos enemigo1 = { 0 };
static int cont = 0;
static int cont2 = 0;


static const int cantP1 = 116;
static const int cantP2 = 121;
static const int cantP3 = 102;
static const int cantP4 = 130;
static Pastillas pastillas1[cantP1] = {0};
static Pastillas pastillas2[cantP2] = {0};
static Pastillas pastillas3[cantP3] = {0};
static Pastillas pastillas4[cantP4] = {0};
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

static int posicionxP4[cantP4] = {64,64,64,64,64,64,
                               110,110,110,
                               156,156,156,156,156,
                               202,202,202,202,202,202,202,202,202,202,202,
                               248,248,248,
                               294,294,294,294,294,294,294,294,294,294,
                               340,340,340,340,340,340,340,340,340,340,
                               386,386,386,386,386,386,
                               432,432,432,432,432,432,432,432,
                               478,478,478,478,478,478,
                               524,524,524,524,524,524,524,524,
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
                                64,110,156,248,294,386,478,524,
                                156,202,248,294,386,524,
                                64,110,156,248,294,386,478,524,
                                64,110,156,386,478,524,
                                64,156,202,248,294,340,386,432,478,524,
                                18,64,156,202,248,294,340,386,432,524,
                                248,432,524,
                                18,64,110,156,202,248,294,340,386,432,524,
                                64,248,432,478,524,
                                64,248,432,
                                18,64,248,432,478,524};


static int posMx;
static int posMy;
static Texture2D player;
static Texture2D personajeU;
static Texture2D personajeD;
static Texture2D personajeL;
static Texture2D personajeR;

static int vidas = 3;
static int puntaje1 = 0;
static int puntos1 = 0;
static int puntaje2 = 0;
static int puntos2 = 0;
static int puntaje3 = 0;
static int puntos3 = 0;
static int puntaje4 = 0;
static int puntos4 = 0;

// Función para encontrar la ruta más rápida utilizando backtracking
void backtrack(int row, int col, std::vector<std::pair<int, int>>& route) {
    // Si se llegó al punto de destino, comprobar si esta es la ruta más rápida
    //if (row == end_row && col == end_col)
    if (row == posMy && col == posMx) {
        if (fastest_route.empty() || route.size() < fastest_route.size()) {
            fastest_route = route;
            mover = route;
        }
        return;
    }

    // Marcamos la casilla actual como visitada
    matriz1[row][col] = "V";

    // Movimiento hacia arriba
    if (row > 0 && matriz1[row-1][col] != "#" && matriz1[row-1][col] != "V") {
        route.push_back(std::make_pair(row-1, col));
        backtrack(row-1, col, route);
        route.pop_back();
    }

    // Movimiento hacia abajo
    if (row < ROWS-1 && matriz1[row+1][col] != "#" && matriz1[row+1][col] != "V") {
        route.push_back(std::make_pair(row+1, col));
        backtrack(row+1, col, route);
        route.pop_back();
    }

    // Movimiento hacia la izquierda
    if (col > 0 && matriz1[row][col-1] != "#" && matriz1[row][col-1] != "V") {
        route.push_back(std::make_pair(row, col-1));
        backtrack(row, col-1, route);
        route.pop_back();
    }

    // Movimiento hacia la derecha
    if (col < COLS-1 && matriz1[row][col+1] != "#" && matriz1[row][col+1] != "V") {
        route.push_back(std::make_pair(row, col+1));
        backtrack(row, col+1, route);
        route.pop_back();
    }

    // Desmarcamos la casilla actual como visitada
    matriz1[row][col] = "0";
}
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
    enemigo1.activex = false;
    enemigo1.activey = true;
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
}

void actJuego1(void){
    delayTimer += 0.5f;

    if(matriz1[posMy-1][posMx] != "#"){
        if (IsKeyPressed(KEY_UP)) {
            posJugy -= jugador1.velocidad.y;
            //matriz1[posMy][posMx] = "0";
            posMy--;
            // Vector para almacenar la ruta
            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo1.mEney, enemigo1.mEnex));

            // Buscamos la ruta más rápida
            backtrack(enemigo1.mEney, enemigo1.mEnex, route);
            fastest_route.clear();
            route.clear();

            //matriz1[posMy][posMx] = "1";
        }
        if (IsKeyPressed(KEY_UP)) player = personajeU;
        if (mencel == "1") {
            posJugy -= jugador1.velocidad.y;
            //matriz1[posMy][posMx] = "0";
            posMy--;

            //matriz1[posMy][posMx] = "1";
        }
        if (mencel == "1") player = personajeU;
    }

    if(matriz1[posMy+1][posMx] != "#"){
        if (IsKeyPressed(KEY_DOWN)) {
            posJugy += jugador1.velocidad.y;
            //matriz1[posMy][posMx] = "0";
            posMy++;
            // Vector para almacenar la ruta
            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo1.mEney, enemigo1.mEnex));

            // Buscamos la ruta más rápida
            backtrack(enemigo1.mEney, enemigo1.mEnex, route);

            fastest_route.clear();
            route.clear();


            //matriz1[posMy][posMx] = "1";
        }
        if (IsKeyPressed(KEY_DOWN)) player = personajeD;
        if (mencel == "2") {
            posJugy += jugador1.velocidad.y;
            //matriz1[posMy][posMx] = "0";
            posMy++;

            //matriz1[posMy][posMx] = "1";
        }
        if (mencel == "2") player = personajeD;

    }

    if(matriz1[posMy][posMx-1] != "#" ){
        if (IsKeyPressed(KEY_LEFT)) {
            posJugx -= jugador1.velocidad.x;
            //matriz1[posMy][posMx] = "0";
            posMx--;
            // Vector para almacenar la ruta
            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo1.mEney, enemigo1.mEnex));

            // Buscamos la ruta más rápida
            backtrack(enemigo1.mEney, enemigo1.mEnex, route);

            fastest_route.clear();
            route.clear();

            //matriz1[posMy][posMx] = "1";
        }
        if (IsKeyPressed(KEY_LEFT)) player = personajeL;
        if (mencel == "3") {
            posJugx -= jugador1.velocidad.x;
            //matriz1[posMy][posMx] = "0";
            posMx--;

            //matriz1[posMy][posMx] = "1";
        }
        if (mencel == "3") player = personajeL;
    }

    if(matriz1[posMy][posMx+1] != "#"){
        if (IsKeyPressed(KEY_RIGHT)) {
            posJugx += jugador1.velocidad.x;
            //matriz1[posMy][posMx] = "0";
            posMx++;
            // Vector para almacenar la ruta
            std::vector<std::pair<int, int>> route;
            route.push_back(std::make_pair(enemigo1.mEney, enemigo1.mEnex));

            // Buscamos la ruta más rápida
            backtrack(enemigo1.mEney, enemigo1.mEnex, route);

            fastest_route.clear();
            route.clear();

            //matriz1[posMy][posMx] = "1";
        }
        if (IsKeyPressed(KEY_RIGHT)) player = personajeR;
        if (mencel == "4") {
            posJugx += jugador1.velocidad.x;
            //matriz1[posMy][posMx] = "0";
            posMx++;

            //matriz1[posMy][posMx] = "1";
        }
        if (mencel == "4") player = personajeR;
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
        if (!mover.empty()){
            if (mover[1].first < enemigo1.mEney){
                enemigo1.posEney -= enemigo1.velene.y;
                enemigo1.mEney--;
            }
            if (mover[1].first > enemigo1.mEney){
                enemigo1.posEney += enemigo1.velene.y;
                enemigo1.mEney++;
            }
            if (mover[1].second < enemigo1.mEnex) {
                enemigo1.posEnex -= enemigo1.velene.x;
                enemigo1.mEnex--;
            }
            if (mover[1].second > enemigo1.mEnex) {
                enemigo1.posEnex += enemigo1.velene.x;
                enemigo1.mEnex++;
            }
            mover.erase(mover.begin());
        }
        //En caso de que queramos mover el pacman de manera aleatoria
        /**
        if (random_number == 0){
            if(matriz1[enemigo1.mEney-1][enemigo1.mEnex] != "#"){
                enemigo1.posEney -= enemigo1.velene.y;
                enemigo1.mEney--;
            }
            else {
                random_number = rand() % 4;
            }
        }
        if (random_number == 1){
            if(matriz1[enemigo1.mEney+1][enemigo1.mEnex] != "#"){
                        enemigo1.posEney += enemigo1.velene.y;
                        enemigo1.mEney++;
            }
            else {
                random_number = rand() % 4;
            }
        }
        if (random_number == 2){
            if(matriz1[enemigo1.mEney][enemigo1.mEnex-1] != "#"){
                        enemigo1.posEnex -= enemigo1.velene.x;
                        enemigo1.mEnex--;
            }
            else {
                random_number = rand() % 4;
            }
        }
        if (random_number == 3){
            if(matriz1[enemigo1.mEney][enemigo1.mEnex+1] != "#"){
                        enemigo1.posEnex += enemigo1.velene.x;
                        enemigo1.mEnex++;
            }
            else {
                random_number = rand() % 4;
            }
        }
         */
        delayTimer = 0.f;
    }

}
void actJuego2(void) {
    if(matriz2[posMy-1][posMx] != "#"){
        if (IsKeyPressed(KEY_UP)) {
            posJugy -= jugador1.velocidad.y;
            posMy--;
        }
        if (IsKeyPressed(KEY_UP)) player = personajeU;
        if (mencel == "1") {
            posJugy -= jugador1.velocidad.y;
            posMy--;
        }
        if (mencel == "1") player = personajeU;
    }

    if(matriz2[posMy+1][posMx] != "#"){
        if (IsKeyPressed(KEY_DOWN)) {
            posJugy += jugador1.velocidad.y;
            posMy++;
        }
        if (IsKeyPressed(KEY_DOWN)) player = personajeD;
        if (mencel == "2") {
            posJugy += jugador1.velocidad.y;
            posMy++;
        }
        if (mencel == "2") player = personajeD;

    }

    if(matriz2[posMy][posMx-1] != "#" ){
        if (IsKeyPressed(KEY_LEFT)) {
            posJugx -= jugador1.velocidad.x;
            posMx--;
        }
        if (IsKeyPressed(KEY_LEFT)) player = personajeL;
        if (mencel == "3") {
            posJugx -= jugador1.velocidad.x;
            posMx--;
        }
        if (mencel == "3") player = personajeL;
    }

    if(matriz2[posMy][posMx+1] != "#"){
        if (IsKeyPressed(KEY_RIGHT)) {
            posJugx += jugador1.velocidad.x;
            matriz2[posMy][posMx] = "0";
            posMx++;
            matriz2[posMy][posMx] = "1";
        }
        if (IsKeyPressed(KEY_RIGHT)) player = personajeR;
        if (mencel == "4") {
            posJugx += jugador1.velocidad.x;
            matriz2[posMy][posMx] = "0";
            posMx++;
            matriz2[posMy][posMx] = "1";
        }
        if (mencel == "4") player = personajeR;
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
}

void actJuego3(void) {
    if (matriz3[posMy - 1][posMx] != "#") {
        if (IsKeyPressed(KEY_UP)) {
            posJugy -= jugador1.velocidad.y;
            posMy--;
        }
        if (IsKeyPressed(KEY_UP)) player = personajeU;
    }

    if (matriz3[posMy + 1][posMx] != "#") {
        if (IsKeyPressed(KEY_DOWN)) {
            posJugy += jugador1.velocidad.y;
            posMy++;
        }
        if (IsKeyPressed(KEY_DOWN)) player = personajeD;
    }

    if (matriz3[posMy][posMx - 1] != "#") {
        if (IsKeyPressed(KEY_LEFT)) {
            posJugx -= jugador1.velocidad.x;
            posMx--;
        }
        if (IsKeyPressed(KEY_LEFT)) player = personajeL;
    }

    if (matriz3[posMy][posMx + 1] != "#") {
        if (IsKeyPressed(KEY_RIGHT)) {
            posJugx += jugador1.velocidad.x;
            posMx++;
        }
        if (IsKeyPressed(KEY_RIGHT)) player = personajeR;
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
}
void actJuego4(void) {
    if (matriz4[posMy - 1][posMx] != "#") {
        if (IsKeyPressed(KEY_UP)) {
            posJugy -= jugador1.velocidad.y;
            posMy--;
        }
        if (IsKeyPressed(KEY_UP)) player = personajeU;
    }

    if (matriz4[posMy + 1][posMx] != "#") {
        if (IsKeyPressed(KEY_DOWN)) {
            posJugy += jugador1.velocidad.y;
            posMy++;
        }
        if (IsKeyPressed(KEY_DOWN)) player = personajeD;
    }

    if (matriz4[posMy][posMx - 1] != "#") {
        if (IsKeyPressed(KEY_LEFT)) {
            posJugx -= jugador1.velocidad.x;
            posMx--;
        }
        if (IsKeyPressed(KEY_LEFT)) player = personajeL;
    }

    if (matriz4[posMy][posMx + 1] != "#") {
        if (IsKeyPressed(KEY_RIGHT)) {
            posJugx += jugador1.velocidad.x;
            posMx++;
        }
        if (IsKeyPressed(KEY_RIGHT)) player = personajeR;
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
}
void celular() {
    char received[1024];
    try {
        tcp::acceptor acceptor(io_context,
                               tcp::endpoint(boost::asio::ip::address::from_string("192.168.100.195"), 5001));

        std::cout << "Waiting for incoming connection..." << std::endl;

        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "Connection accepted." << std::endl;

        boost::asio::streambuf buffer;
        boost::system::error_code error;

        while (true) {

            boost::asio::read_until(socket, buffer, "\n", error);

            if (error == boost::asio::error::eof) {
                break; // Connection closed cleanly by peer.
            } else if (error) {
                throw boost::system::system_error(error); // Some other error.
            }

            string message(boost::asio::buffer_cast<const char *>(buffer.data()), buffer.size());
//            cout<<"posicion m x: " <<posMx<<endl;
//            cout<<"posicion m y: " <<posMy<<endl;
            //size_t received_len = read_socket(received, sizeof(received));
            int startind = message.find("\n");
            if (startind != std::string::npos) {
                message.erase(startind, message.length() - startind);
            }
            std::cout << "Received message: " << message << std::endl;

            mencel = message;

            buffer.consume(buffer.size());

        }
    }
    catch (std::exception & e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
}

/**
 * Función Main, utilizada para iniciar el código, abrir la ventana inicial del juego, dibujar texturas, actualizar y
 * llevar la comunicación de ventanas, actualización de sprites, etc.
 * @return
 */
int main() {
    srand(time(0));
    random_number = rand() % 4;
    const int screenWidth = 960;
    const int screenHeight = 540;
    thread cel(celular);
    InitWindow(screenWidth, screenHeight, "-APACALYPSE-");
    GameScreen currentScreen = INICIO;

    Rectangle boton1 = {380, 330, 200, 70};
    Rectangle per1 = {55, 285, 130, 142};
    Rectangle per2 = {300, 285, 130, 142};
    Rectangle per3 = {535, 285, 130, 142};
    Rectangle per4 = {780, 285, 130, 142};


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

    Texture2D pacama = LoadTexture("ImagenesPac/PacD11.png");


    SetTargetFPS(120);               // Set desired framerate (frames-per-second)
    //--------------------------------------------------------------------------------------


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {



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
                        player = LoadTexture("ImagenesPer/Per1D.png");
                        personajeU = LoadTexture("ImagenesPer/Per1U.png");
                        personajeD = LoadTexture("ImagenesPer/Per1D.png");
                        personajeL = LoadTexture("ImagenesPer/Per1L.png");
                        personajeR = LoadTexture("ImagenesPer/Per1R.png");
                        juego();
                    }
                }
                else if (CheckCollisionPointRec(GetMousePosition(), per2)){
                    fondoS = fondo2;
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = NIVEL1;
                        player = LoadTexture("ImagenesPer/Per2D.png");
                        personajeU = LoadTexture("ImagenesPer/Per2U.png");
                        personajeD = LoadTexture("ImagenesPer/Per2D.png");
                        personajeL = LoadTexture("ImagenesPer/Per2L.png");
                        personajeR = LoadTexture("ImagenesPer/Per2R.png");
                        juego();
                    }
                }
                else if (CheckCollisionPointRec(GetMousePosition(), per3)){
                    fondoS = fondo3;
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = NIVEL1;
                        player = LoadTexture("ImagenesPer/Per3D.png");
                        personajeU = LoadTexture("ImagenesPer/Per3U.png");
                        personajeD = LoadTexture("ImagenesPer/Per3D.png");
                        personajeL = LoadTexture("ImagenesPer/Per3L.png");
                        personajeR = LoadTexture("ImagenesPer/Per3R.png");
                        juego();
                    }
                }
                else if (CheckCollisionPointRec(GetMousePosition(), per4)){
                    fondoS = fondo4;
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        currentScreen = NIVEL1;
                        player = LoadTexture("ImagenesPer/Per4D.png");
                        personajeU = LoadTexture("ImagenesPer/Per4U.png");
                        personajeD = LoadTexture("ImagenesPer/Per4D.png");
                        personajeL = LoadTexture("ImagenesPer/Per4L.png");
                        personajeR = LoadTexture("ImagenesPer/Per4R.png");
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
                if(puntos1 == cantP1){
                    currentScreen = NIVEL2;
                    posMx = 10;
                    posMy = 10;
                    posJugx = 459;
                    posJugy = 459;
                }
            } break;
            case NIVEL2:
            {
                actJuego2();
                if(puntos2 == cantP2){
                    currentScreen = NIVEL3;
                    posMx = 10;
                    posMy = 10;
                    posJugx = 459;
                    posJugy = 459;
                }
            } break;
            case NIVEL3:
            {
                actJuego3();
                if(puntos3 == cantP3){
                    currentScreen = NIVEL4;
                    posMx = 10;
                    posMy = 9;
                    posJugx = 459;
                    posJugy = 375;
                }
            } break;
            case NIVEL4:
            {
                actJuego4();
                if(puntos4 == cantP4){
                    currentScreen = GANAR;
                }
            } break;
            case GANAR:
            {

            } break;
            case PERDER:
            {

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
                DrawTexture(pacama, enemigo1.posEnex, enemigo1.posEney, RAYWHITE);
                for(int i = 0; i < 116; i++) {
                    if(pastillas1[i].active){
                        DrawRectangleRec(pastillas1[i].rec,YELLOW);
                    }
                }
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
                DrawTexture(pacama, enemigo1.posEnex, enemigo1.posEney, RAYWHITE);
                for(int i = 0; i < cantP2; i++) {
                    if(pastillas2[i].active){
                        DrawRectangleRec(pastillas2[i].rec,YELLOW);
                    }
                }
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
                DrawTexture(pacama, enemigo1.posEnex, enemigo1.posEney, RAYWHITE);
                for(int i = 0; i < cantP3; i++) {
                    if(pastillas3[i].active){
                        DrawRectangleRec(pastillas3[i].rec,YELLOW);
                    }
                }
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
                DrawTexture(pacama, enemigo1.posEnex, enemigo1.posEney, RAYWHITE);
                for(int i = 0; i < cantP4; i++) {
                    if(pastillas4[i].active){
                        DrawRectangleRec(pastillas4[i].rec,YELLOW);
                    }
                }
            } break;
            case GANAR:
            {

            } break;
            case PERDER:
            {

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