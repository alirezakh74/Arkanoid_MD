
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
Sprite* paddlePlayer;


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

	if(player_y_pos + player_width > RIGHT_EDGE)
    {
        player_x_pos = RIGHT_EDGE - player_width;
    }

    SPR_setPosition(paddlePlayer, player_x_pos, player_y_pos);
    
}

int main()
{

    JOY_init();
    JOY_setEventHandler(&joyEvent);

    PAL_setColors(0, palette_black, 64, DMA);

    SPR_init();

    u16 pallete_full[64] = {0};

    u16 pallete_0_full[16];
    memcpy(&pallete_0_full[0], logo.palette->data, 16 * 2);
    //PAL_setPalette(PAL0, logo.palette->data, DMA);
    //PAL_setColors(0, palette_black, 16, DMA);

    u16 pallete_2_full[16];
    memcpy(&pallete_2_full[0], paddle.palette->data, 16 * 2);

    //PAL_setColors(32, palette_black, 16, DMA);
    //PAL_setPalette(PAL2, paddle.palette->data, DMA);
    paddlePlayer = SPR_addSprite(&paddle, player_x_pos, player_x_pos, TILE_ATTR(PAL2, false, false, false));
    //SPR_setPosition(paddlePlayer, player_x_pos, player_y_pos);

    for(u16 i = 0; i < sizeof(pallete_full) / sizeof(pallete_full[0]); ++i)
    {
        //pallete_full[i] = ( (i <= 15) ? pallete_0_full[i] : pallete_2_full[32 + i] );

        if(i <= 15)
        {
            pallete_full[i] = pallete_0_full[i];
            //KLog("BBB");
        }
        else if(i >= 32 && i <= 47)
        {
            pallete_full[i] = pallete_2_full[i - 32];
            //KLog_S1("p2 ", pallete_2_full[i]);
            //KLog_S1("p2 ", pallete_2_full[i - 32]);
        }
        else
        {
            pallete_full[i] = palette_black;
        }

        //KLog_S2("i = ", i, " pallete_full ", pallete_full[i]);
    }

    VDP_drawImage(BG_B, &logo, 10, 10);
    SPR_update();

    PAL_setColors(0, palette_black, 64, DMA);
    PAL_fadeIn(0, 63, pallete_full, 100, FALSE);
    //waitMs(1000);
    //PAL_fadeOut(0, 63, 100, false);

    //XGM2_playPCM(&story_music, sizeof(story_music), SOUND_PCM_CH1);

    //XGM2_play(music);
    //XGM2_fadeIn(120);

    XGM2_play(music);

    while(1)
    {
        // if(!XGM2_isPlayingPCM(SOUND_PCM_CH1_MSK))
        // {
        //     XGM2_playPCM(&story_music, sizeof(story_music), SOUND_PCM_CH1);
        // }
        movePlayer();
        SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}
