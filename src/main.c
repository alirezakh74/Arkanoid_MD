#include <genesis.h>
#include <resourses.h>

const u16 LEFT_EDGE = 0;
const int RIGHT_EDGE = 320;
const int TOP_EDGE = 0;
const int BOTTOM_EDGE = 224;

s16 player_x_pos = 200;
const s16 player_y_pos = 200;
const u16 PLAYER_SPEED = 10;
u16 player_x_velocity = 0;
u16 player_width = 48;
u16 player_hight = 16;

u16 ball_x_pos = 200;
u16 ball_y_pos = 200;
u16 ball_x_vellocity = 0;
u16 ball_y_vellocity = 0;

typedef struct
{
    Sprite* sprite;

    Vect2D_s16 position;
    Vect2D_s16 velocity;
    bool active;
} GameObject;

GameObject* player;
GameObject* ball;

enum GameStates
{
    SplashState = 1,
    MainMenuState,
    OptionMenuState,
    GamePlayState,
    GameOverState,
    CounterStates
};

enum GameStates* gameStates;

typedef struct
{
    enum GameStates* currentState;
} State;

State* state;

static void clearState(State* requestedState);
static void initState(State* requestedState);

static void changeState(enum GameStates* requestedState)
{
    clearState(&state);
    state->currentState = requestedState;
    initState(&state);
}

static void initState(State* requestedState)
{
    switch (*(requestedState->currentState))
    {
    case SplashState:
        VDP_drawImage(BG_B, &logo, 10, 10);
        XGM2_play(music);
        break;
    case MainMenuState:
        VDP_drawText("Play", 10, 10);
        VDP_drawText("Option", 10, 12);
        break;
    case OptionMenuState:
        VDP_drawText("Options", 10, 4);
        break;
    case GamePlayState:
        // add player sprite
        player = (GameObject*)malloc(sizeof(GameObject));
        PAL_setPalette(PAL2, paddle_image.palette->data, DMA);
        player = SPR_addSprite(&paddle_image, player_x_pos, player_x_pos, TILE_ATTR(PAL2, false, false, false));
        player->active = TRUE;
        // add ball sprite
        ball = (GameObject*)malloc(sizeof(GameObject));
        ball->sprite = SPR_addSprite(&ball_image, player_x_pos-48, player_x_pos-48, TILE_ATTR(PAL2, false, false, false));
        ball->active = TRUE;
        break;
    case GameOverState:
        VDP_drawText("Game Over!", 10, 10);
        break;
    
    default:
        break;
    }
}

static void clearState(State* requestedState)
{
    //clear everyting in screen
    VDP_clearPlane(BG_B, TRUE);
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearSprites();

    switch (*(requestedState->currentState))
    {
    case SplashState:
        break;
    case MainMenuState:
        break;
    case OptionMenuState:
        break;
    case GamePlayState:
        // clear pointers of sprites
        free(player);
        free(ball);
        break;
    case GameOverState:
        break;
    default:
        break;
    }
}

static void updateState(State* requestedState)
{
    switch (*(requestedState->currentState))
    {
    case SplashState:
        waitMs(5000);
        gameStates = MainMenuState;
        changeState(gameStates);
        break;
    case MainMenuState:
        break;
    case OptionMenuState:
        break;
    case GamePlayState:
        movePlayer();
        moveBall();
        break;
    case GameOverState:
        break;
    default:
        break;
    }
}

// void update(struct GameObject* gameObject)
// {
    //gameObject->position.x += gameObject->velocity.x;
    //gameObject->position.y += gameObject->velocity.y;
// }


static void joyEvent(u16 joy, u16 changed, u16 state)
{
    if(joy == JOY_1)
    {
        if(state & BUTTON_LEFT)
        {
            player_x_velocity = -PLAYER_SPEED;
        }
        else if(state & BUTTON_RIGHT)
        {
            player_x_velocity = PLAYER_SPEED;
        }
        else
        {
            if( (changed & BUTTON_LEFT) | (changed & BUTTON_RIGHT) )
            {
                player_x_velocity = 0;
            }
        }
    }
}

void movePlayer()
{
    player_x_pos += player_x_velocity;

    if(player_x_pos < LEFT_EDGE)
    {
        player_x_pos = LEFT_EDGE;
    }

	if(player_x_pos + player_width > RIGHT_EDGE)
    {
        player_x_pos = RIGHT_EDGE - player_width + 8;
    }
    SPR_setPosition(&player->sprite, player_x_pos, player_y_pos);
    //KLog_S1("player y pos = ", player_x_pos);
}

void moveBall()
{

}

int main()
{
    state = (State*)malloc(sizeof(State));

    JOY_init();
    JOY_setEventHandler(&joyEvent);

    //PAL_setColors(0, palette_black, 64, DMA);

    SPR_init();

    //u16 pallete_full[64] = {0};

    //u16 pallete_0_full[16];
    //memcpy(&pallete_0_full[0], logo.palette->data, 16 * 2);
    //PAL_setPalette(PAL0, logo.palette->data, DMA);
    //PAL_setColors(0, palette_black, 16, DMA);

    //u16 pallete_2_full[16];
    //memcpy(&pallete_2_full[0], paddle_image.palette->data, 16 * 2);

    //PAL_setColors(32, palette_black, 16, DMA);
    //PAL_setPalette(PAL2, paddle.palette->data, DMA);
    //SPR_setPosition(paddlePlayer, player_x_pos, player_y_pos);

    // for(u16 i = 0; i < sizeof(pallete_full) / sizeof(pallete_full[0]); ++i)
    // {
    //     //pallete_full[i] = ( (i <= 15) ? pallete_0_full[i] : pallete_2_full[32 + i] );

    //     if(i <= 15)
    //     {
    //         pallete_full[i] = pallete_0_full[i];
    //         //KLog("BBB");
    //     }
    //     else if(i >= 32 && i <= 47)
    //     {
    //         pallete_full[i] = pallete_2_full[i - 32];
    //         //KLog_S1("p2 ", pallete_2_full[i]);
    //         //KLog_S1("p2 ", pallete_2_full[i - 32]);
    //     }
    //     else
    //     {
    //         pallete_full[i] = palette_black;
    //     }

    //     //KLog_S2("i = ", i, " pallete_full ", pallete_full[i]);
    // }

    //VDP_drawImage(BG_B, &logo, 10, 10);

    //SPR_update();

    //PAL_setColors(0, palette_black, 64, DMA);
    //PAL_fadeIn(0, 63, pallete_full, 100, FALSE);
    //waitMs(1000);
    //PAL_fadeOut(0, 63, 100, false);

    //XGM2_playPCM(&story_music, sizeof(story_music), SOUND_PCM_CH1);

    //XGM2_play(music);
    //XGM2_fadeIn(120);

    state->currentState = SplashState;
    initState(&state);

    while(1)
    {
        // if(!XGM2_isPlayingPCM(SOUND_PCM_CH1_MSK))
        // {
        //     XGM2_playPCM(&story_music, sizeof(story_music), SOUND_PCM_CH1);
        // }
        updateState(&state);
        SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}
