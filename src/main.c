#include <genesis.h>
#include <resourses.h>

const u16 LEFT_EDGE = 0;
const u16 RIGHT_EDGE = 320;
const u16 TOP_EDGE = 0;
const u16 BOTTOM_EDGE = 224;

s16 player_x_pos = 200;
const s16 PLYER_Y_POS = 200;
const u16 PLAYER_SPEED = 10;
s16 player_x_velocity = 0;
u16 PLAYER_WIDTH = 48;
u16 PLAYER_HEIGHT = 16;

s16 ball_x_pos = 200;
s16 ball_y_pos = 200;
const u16 BALL_SPEED = 5;
s16 ball_x_vellocity = 0;
s16 ball_y_vellocity = 0;
const u16 BALL_WIDTH = 16;
const u16 BALL_HEIGHT = 16;

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
        if(!XGM2_isPlaying())
        {
            XGM2_play(music);
        }
        VDP_drawImage(BG_B, &logo_image, 10, 10);
        break;
    case MainMenuState:
        PAL_setColor(15, RGB24_TO_VDPCOLOR(0xff0000));
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
        ball_x_vellocity = BALL_SPEED;
        ball_y_vellocity = -BALL_SPEED;
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
    //VDP_clearPlane(BG_B, TRUE);
    //VDP_clearPlane(BG_A, TRUE);
    //VDP_clearPlane(WINDOW, TRUE);
    //VDP_clearSprites();
    //SPR_clear();
    //VDP_releaseAllSprites();
    //VDP_releaseSprites(0, SAT_MAX_SIZE);
    //VRAM_clearRegion(0);
    SPR_reset();
    VDP_resetScreen();

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
        if(player)
        {
            //SPR_releaseSprite(player->sprite);
            free(player);
            player = NULL;
        }
        if(ball)
        {
            //SPR_releaseSprite(ball->sprite);
            free(ball);
            ball = NULL;
        }
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
        waitMs(3000);
        gameStates = MainMenuState;
        changeState(gameStates);
        break;
    case MainMenuState:
        waitMs(3000);
        gameStates = GamePlayState;
        changeState(gameStates);
        break;
    case OptionMenuState:
        break;
    case GamePlayState:
        movePlayer();
        moveBall();
        //waitMs(3000);
        //gameStates = SplashState;
        //changeState(gameStates);
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

	if(player_x_pos + PLAYER_WIDTH > RIGHT_EDGE)
    {
        player_x_pos = RIGHT_EDGE - PLAYER_WIDTH + 8;
    }
    SPR_setPosition(&player->sprite, player_x_pos, PLYER_Y_POS);
    //KLog_S1("player y pos = ", player_x_pos);
}

void moveBall()
{
    //s16 x_pos = SPR_getPositionX(ball->sprite);
    //s16 y_pos = SPR_getPositionY(ball->sprite);

    ball_x_pos += ball_x_vellocity;
    ball_y_pos += ball_y_vellocity;

    SPR_setPosition(ball->sprite, ball_x_pos, ball_y_pos);

    if(ball_x_pos <= LEFT_EDGE)
    {
        SPR_setPosition(ball->sprite, LEFT_EDGE + 2, ball_y_pos);
        ball_x_vellocity = BALL_SPEED;
    }
    else if(ball_x_pos >= RIGHT_EDGE)
    {
        SPR_setPosition(ball->sprite, RIGHT_EDGE - BALL_WIDTH - 2, ball_y_pos);
        ball_x_vellocity = -BALL_SPEED;
    }

    if(ball_y_pos <= TOP_EDGE)
    {
        SPR_setPosition(ball->sprite, ball_x_pos, TOP_EDGE + 2);
        ball_y_vellocity = BALL_SPEED;
    }
    else if(ball_y_pos >= BOTTOM_EDGE)
    {
        SPR_setPosition(ball->sprite, ball_x_pos, BOTTOM_EDGE - BALL_HEIGHT - 2);
        ball_y_vellocity = -BALL_SPEED;
    }

    KLog_S1("ball velocity x = ", ball_x_vellocity);
    KLog_S1("ball velocity y = ", ball_y_vellocity);
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
    //SPR_setPosition(paddlePlayer, player_x_pos, PLYER_Y_POS);

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
