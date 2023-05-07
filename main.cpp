#include <iostream>
#include <raylib.h>
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


static string matriz1[12][21] = {
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

static int posJugx;
static int posJugy;
static Jugador jugador1 = { 0 };
static int posMx;
static int posMy;
static Texture2D player;
static Texture2D personajeU;
static Texture2D personajeD;
static Texture2D personajeL;
static Texture2D personajeR;



void juego(void){
    posJugx = 460;
    posJugy = 416;
    posMx = 10;
    posMy = 9;
    jugador1.velocidad.x = 46;
    jugador1.velocidad.y = 46;
}

void actJuego(void){
    if(matriz1[posMy-1][posMx] != "#"){
        if (IsKeyPressed(KEY_UP)) {
            posJugy -= jugador1.velocidad.y;
            posMy--;
        }
        if (IsKeyPressed(KEY_UP)) player = personajeU;
    }
    if(matriz1[posMy+1][posMx] != "#"){
        if (IsKeyPressed(KEY_DOWN)) {
            posJugy += jugador1.velocidad.y;
            posMy++;
        }
        if (IsKeyPressed(KEY_DOWN)) player = personajeD;
    }
    if(matriz1[posMy][posMx-1] != "#"){
        if (IsKeyPressed(KEY_LEFT)) {
            posJugx -= jugador1.velocidad.x;
            posMx--;
        }
        if (IsKeyPressed(KEY_LEFT)) player = personajeL;
    }
    if(matriz1[posMy][posMx+1] != "#"){
        if (IsKeyPressed(KEY_RIGHT)) {
            posJugx += jugador1.velocidad.x;
            posMx++;
        }
        if (IsKeyPressed(KEY_RIGHT)) player = personajeR;
    }
}




void actualizar(void){
    actJuego();
}
/**
 * Función Main, utilizada para iniciar el código, abrir la ventana inicial del juego, dibujar texturas, actualizar y
 * llevar la comunicación de ventanas, actualización de sprites, etc.
 * @return
 */
int main() {
    const int screenWidth = 960;
    const int screenHeight = 540;

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
                actualizar();
            } break;
            case NIVEL2:
            {

            } break;
            case NIVEL3:
            {

            } break;
            case NIVEL4:
            {

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
                DrawTexture(player, posJugx,posJugy, RAYWHITE);
            } break;
            case NIVEL2:
            {

            } break;
            case NIVEL3:
            {
            } break;
            case NIVEL4:
            {

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



    CloseWindow();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}