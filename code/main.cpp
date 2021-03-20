#include <stdio.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <string>
#define GAME_TERMINATE -1
#define ScreenHeight 240


ALLEGRO_TIMER* timer =  NULL;
ALLEGRO_TIMER* timer2 =  NULL;
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_FONT* font = NULL;
ALLEGRO_FONT* font2 = NULL;
ALLEGRO_SAMPLE* song = NULL;
ALLEGRO_SAMPLE* footsteps = NULL;
ALLEGRO_SAMPLE* crunchsound = NULL;
ALLEGRO_SAMPLE* doorsound = NULL;
ALLEGRO_SAMPLE* laughter = NULL;
ALLEGRO_SAMPLE* bass = NULL;
ALLEGRO_SAMPLE_INSTANCE* instance1 = NULL;
ALLEGRO_SAMPLE_INSTANCE* instance2 = NULL;
ALLEGRO_SAMPLE_INSTANCE* instance3 = NULL;
ALLEGRO_SAMPLE_INSTANCE* instance4 = NULL;
ALLEGRO_SAMPLE_INSTANCE* instance5 = NULL;
ALLEGRO_BITMAP* player = NULL;
ALLEGRO_BITMAP* bg = NULL;
ALLEGRO_BITMAP* title = NULL;
ALLEGRO_BITMAP* papa = NULL;
ALLEGRO_BITMAP* shadow = NULL;
ALLEGRO_BITMAP* titlejohnny = NULL;
ALLEGRO_BITMAP* textbox = NULL;
ALLEGRO_BITMAP* textbox2 = NULL;


//new bitmaps for window 2 here
ALLEGRO_BITMAP* papa_1 = NULL;
ALLEGRO_BITMAP* door = NULL;
ALLEGRO_BITMAP* frame = NULL;
ALLEGRO_BITMAP* family = NULL;

//bitmaps for STAGE 2
ALLEGRO_BITMAP* youdied = NULL;
ALLEGRO_BITMAP* enemyx1 = NULL;
ALLEGRO_BITMAP* enemyx2 = NULL;
ALLEGRO_BITMAP* enemyx3 = NULL;
ALLEGRO_BITMAP* enemyx4 = NULL;
ALLEGRO_BITMAP* enemyi = NULL;
ALLEGRO_BITMAP* enemys = NULL;

//bitmaps for STAGE_3
ALLEGRO_BITMAP* long_bg = NULL;
ALLEGRO_BITMAP* fox = NULL;
ALLEGRO_BITMAP* paperimg = NULL;
ALLEGRO_TRANSFORM camera;

//bitmaps for STAGE_4
ALLEGRO_BITMAP* end_bg = NULL;
ALLEGRO_BITMAP* mama = NULL;

ALLEGRO_EVENT_QUEUE *event_queue = NULL;

enum KEYS { DOWN, LEFT, RIGHT, UP};
enum STATE {MOVE, IDLE, FRAME, DOOR1};
enum SCENE {START, STAGE_1, STAGE_2, STAGE_3, ENDING, LOSE, FINISH};

const char *gametitle = "johnny johnny";
const int width = 1280;
const int height = 720;
const float DEAD_TIME = 1;
int sign = 0;
int counter = 0;
int active_scene;
int state;
const int pos_x = width / 2 - 50; //starting pos_x
const int pos_y = height / 2 + 200; //starting pos_y
int dir = DOWN, sourcex = 0, sourcey = 0;
bool startmove = false;
bool startboss = false;
bool draw = true;
bool active = false;
bool hitframe=false;
bool hitdoor1=false;
bool wronganswer=false;
bool hitdoor2=false;
bool stop_enemy = false;
bool stop1 = false, stop2 = false, stop3 = false;
bool hitpaper = false;
bool showpaper = false;
bool hitdoor3=false;
float movespeed = 5;
float cameraPos_y = 0;
double enemy1_die_timestamp;
double enemy2_die_timestamp;
double enemy3_die_timestamp;
double enemy4_die_timestamp;


void show_err_msg(int msg);
void game_init();
void game_draw();
void game_change_scene(int next_scene);
void game_begin();
void game_destroy();
bool Collision(float x, float y, float ex, float ey, int width, int height);
int game_run();
int process_event();
void stopmoving();

//dialogue stuff
int dialogueindex = 0;
char* dialogue1[12] = {"papa: johnny, johnny...","johnny: yes papa?","papa: eating sugar?","johnny: no, papa...","papa: telling lies?","johnny: no, papa...","papa: well, then. prove it.","johnny: 'i really did not eat sugar.", "what should i say to convince him?'","johnny: 'oh, i know... i will give","papa's favorite letters.'","hint: find papa's favorite letters."};
char* dialogue2a[6] = {"johnny: mama's room is just 2 doors ahead.", "johnny: i have to be quick!","sugarcube monsters appeared!", "johnny: What are those creatures?", "johnny:i need to find mama soon...", "use your keyboard to eliminate enemies"};
char* door2text[8] = {"","","","","","","hm? there is a password in this door again.", "'enter the last two letters of this stage'."};
char* dialogue3[7] = {"the next room is mama's room!", "i finally can have proof.", "i don't know why papa hates sugar,", "mama loves it a lot.","i love mama.","that's why i gave her all of the sugar.","there's a piece of paper under the door"};
char* dialogue4[19] = {"papa: johnny, johnny...", "johnny: why is papa here?", "papa: I see,", "you have figured it out.", "johnny: ...?", "m a m a  i s  d e a d !", "that is what you have answered.", "johnny: the passwords...", "that's what they're for?", "papa: now tell me johnny,", "where is the sugar?", "johnny: ...?", "papa: why did you give sugar to mama?!", "johnny: ...?",
                       "papa: mama died,", "by sugar overdose, johnny.", "johnny: ...?", "papa: open your mouth johnny.", "johnny: ha! ha! ha!"
                      };
std::string keyboardString="";
const char *inputString = keyboardString.c_str();
int maxWord[3] = {4,2,4};



typedef struct M_obj
{
    float x;
    float y;
    float vx;
    float vy;
    float w;
    float h;
    bool hidden;
    bool is_alive = true;
    ALLEGRO_BITMAP *image_path;

} M_obj;

M_obj character1;
M_obj enemy[4];
M_obj boss[2];
M_obj paper;


void cameraUpdate (float cameraPos, float y, int charheight)
{
    cameraPos_y = -(ScreenHeight) + (y+charheight/2);
    if(cameraPos_y > 1100)
        cameraPos_y = 1100;
}

void moveInside(int scrwidth, int scrheight)
{
    if(character1.x - character1.w / 2 < 0)
        character1.x = character1.w/2;
    else if(character1.x + character1.w / 2 > scrwidth - 100)
        character1.x = (scrwidth - 100) - character1.w/2;
    else if(character1.y - character1.h / 2 < 0)
        character1.y = character1.h/2;
    else if(character1.y + character1.h / 2 > scrheight - 100)
        character1.y = (scrheight - 100) - character1.h/2;
}

void draw_movable_object(M_obj obj)
{
    if (obj.hidden)
        return;
    al_draw_bitmap(obj.image_path, obj.x, obj.y, 0);
}

void game_draw()
{
    //start
    if(active_scene == START)
    {
        al_draw_bitmap(player,0, 0, 0); //for character
        al_draw_bitmap(bg, 0, 0, 0);
        al_draw_bitmap(shadow, 55, 10, 0);
        al_draw_bitmap(papa, 150, 0, 0);
        al_draw_bitmap(title, 500, 250, 0);
        al_draw_bitmap(titlejohnny, 220, 350, 0);
        al_draw_text(font,al_map_rgb(255,255,255),800,500,0,"press enter to start");
    }

    //stage_1
    else if(active_scene == STAGE_1)
    {
        al_draw_bitmap(bg, 0,0,0);
        al_draw_bitmap(papa_1,950,250,0);
        al_draw_bitmap(frame,290,220,0);
        al_draw_bitmap(door,550,0,0);

        // Change Image for animation
        if(draw)
        {
            al_draw_bitmap_region(character1.image_path, sourcex, sourcey * al_get_bitmap_height(character1.image_path) / 4, 100, 100, character1.x, character1.y, 0);
        }

        int sz = sizeof(dialogue1)/sizeof(dialogue1[0]);
        //interact
        if(dialogueindex<sz)
        {
            state = IDLE;
            al_draw_bitmap(textbox,300,200,0);
            al_draw_text(font,al_map_rgb(255,255,255),600,250,ALLEGRO_ALIGN_CENTER,dialogue1[dialogueindex]);
        }
        else
        {
            state = MOVE;
        }

        if(hitframe)
        {

            al_draw_bitmap(family,300,0,0);
        }

        if(hitdoor1)
        {
            al_draw_bitmap(textbox,300,200,0);
            al_draw_text(font,al_map_rgb(255,255,255),600,230,ALLEGRO_ALIGN_CENTER,"what's papa's favorite word?");
            al_draw_text(font,al_map_rgb(255,0,0),600,270,ALLEGRO_ALIGN_CENTER,inputString);
        }
        if(wronganswer)
        {
            al_draw_bitmap(textbox,300,200,0);
            al_draw_text(font,al_map_rgb(255,255,255),600,250,ALLEGRO_ALIGN_CENTER,"wrong answer!");
        }

    }

    //stage 2
    else if(active_scene == STAGE_2)
    {
        al_draw_bitmap(bg, 0,0,0);
        al_draw_bitmap(door,550,0,0);

        for(int i=0; i<4; i++)
            draw_movable_object(enemy[i]);
        for(int i=0; i<2; i++)
            draw_movable_object(boss[i]);

        if(draw)
        {
            al_draw_bitmap_region(character1.image_path, sourcex, sourcey * al_get_bitmap_height(character1.image_path) / 4, 100, 100, character1.x, character1.y, 0);
        }

        int sz = sizeof(dialogue2a)/sizeof(dialogue2a[0]);
        //interact
        if(!hitdoor2)
        {
            if(dialogueindex<sz - 3)
            {
                state = IDLE;
                al_draw_bitmap(textbox2,200,300,0);
                al_draw_text(font,al_map_rgb(255,255,255),600,350,ALLEGRO_ALIGN_CENTER,dialogue2a[dialogueindex]);
            }
            else if(sz-3 == dialogueindex)

            {
                state = IDLE;
                al_draw_bitmap(textbox,300,400,0);
                al_draw_text(font,al_map_rgb(255,255,255),600,450,ALLEGRO_ALIGN_CENTER,"press z to continue");
            }
            else if(sz-3 < dialogueindex && dialogueindex < sz)
            {
                state = IDLE;
                al_draw_bitmap(textbox2,200,300,0);
                al_draw_text(font,al_map_rgb(255,255,255),600,350,ALLEGRO_ALIGN_CENTER,dialogue2a[dialogueindex]);
            }
            else
                state = MOVE;
        }

        if(hitdoor2)
        {
            al_draw_bitmap(textbox,300,200,0);
            al_draw_text(font,al_map_rgb(255,255,255),620,230,ALLEGRO_ALIGN_CENTER,door2text[dialogueindex]);
            al_draw_text(font,al_map_rgb(255,0,0),620,270,ALLEGRO_ALIGN_CENTER,inputString);
        }

        if(wronganswer)
        {
            al_draw_bitmap(textbox,300,200,0);
            al_draw_text(font,al_map_rgb(255,255,255),620,250,ALLEGRO_ALIGN_CENTER,"wrong answer!");
        }
    }

    else if(active_scene == LOSE)
    {
        al_draw_bitmap(bg, 0,0,0);
        al_draw_bitmap(youdied,200,200,0);
        al_draw_text(font,al_map_rgb(255,255,255),650,550,ALLEGRO_ALIGN_CENTER,"press space to continue");
    }

    else if(active_scene == STAGE_3)
    {
        al_draw_bitmap(long_bg, 0,-1020,0);
        //al_draw_bitmap(paper,600,100,0);
        draw_movable_object(paper);
        al_draw_bitmap(door,550,0,0);

        if(draw)
        {
            al_draw_bitmap_region(character1.image_path, sourcex, sourcey * al_get_bitmap_height(character1.image_path) / 4, 100, 100, character1.x, character1.y, 0);
        }

        if(stop1)
        {
            state = IDLE;
            al_draw_bitmap(textbox,300,cameraPos_y+250,0);
            al_draw_text(font,al_map_rgb(255,255,255),620,cameraPos_y+300,ALLEGRO_ALIGN_CENTER,dialogue3[dialogueindex]);
        }
        else if(stop2)
        {
            state = IDLE;
            al_draw_bitmap(textbox,300,cameraPos_y+250,0);
            al_draw_text(font,al_map_rgb(255,255,255),620,cameraPos_y+300,ALLEGRO_ALIGN_CENTER,dialogue3[dialogueindex]);
        }
        else if(stop3)
        {
            state = IDLE;
            al_draw_bitmap(textbox,300,cameraPos_y+250,0);
            al_draw_text(font,al_map_rgb(255,255,255),620,cameraPos_y+300,ALLEGRO_ALIGN_CENTER,dialogue3[dialogueindex]);
        }
        else
            state = MOVE;


        if(showpaper)
        {
            al_draw_bitmap(fox,370,cameraPos_y,0);
        }

        if(hitpaper)
        {
            state = IDLE;
            al_draw_bitmap(textbox,300,cameraPos_y+250,0);
            al_draw_text(font,al_map_rgb(255,255,255),620,cameraPos_y+300,ALLEGRO_ALIGN_CENTER,dialogue3[dialogueindex]);
            al_draw_text(font2,al_map_rgb(255,255,255),620,cameraPos_y+350,ALLEGRO_ALIGN_CENTER,"press z to pick it up");
        }

        if(hitdoor3)
        {
            state = IDLE;
            al_draw_bitmap(textbox,300,350,0);
            al_draw_text(font,al_map_rgb(255,255,255),620,380,ALLEGRO_ALIGN_CENTER,"what's the answer?");
            al_draw_text(font2,al_map_rgb(255,255,255),620,cameraPos_y+580,ALLEGRO_ALIGN_CENTER,"hold space to see the paper");
            al_draw_text(font,al_map_rgb(255,0,0),620,410,ALLEGRO_ALIGN_CENTER,inputString);
        }

        if(wronganswer)
        {
            al_draw_bitmap(textbox,300,300,0);
            al_draw_text(font,al_map_rgb(255,255,255),620,350,ALLEGRO_ALIGN_CENTER,"wrong answer!");
        }

    }

    else if(active_scene == ENDING)
    {
        al_draw_bitmap(end_bg, 0,-125,0);
        al_draw_bitmap(mama, 530,-125,0);
        al_draw_bitmap(papa_1,580,170,0);
        if(draw)
        {
            al_draw_bitmap_region(character1.image_path, sourcex, sourcey * al_get_bitmap_height(character1.image_path) / 4, 100, 100, character1.x, character1.y, 0);
        }

        int sz = sizeof(dialogue4)/sizeof(dialogue4[0]);
        //interact
        if(dialogueindex<sz)
        {
            state = IDLE;
            al_draw_bitmap(textbox,300,0,0);
            al_draw_text(font,al_map_rgb(255,255,255),620,50,ALLEGRO_ALIGN_CENTER,dialogue4[dialogueindex]);
        }

    }

    else if(active_scene == FINISH)
    {
        al_draw_bitmap(bg, 0,-125,0);
        al_draw_text(font,al_map_rgb(255,255,255),620,250,ALLEGRO_ALIGN_CENTER,"thanks for playing!");
        al_draw_text(font2,al_map_rgb(255,255,255),620,300,ALLEGRO_ALIGN_CENTER,"made by alice and bryan");
    }

    al_flip_display();
}

bool Collision(float x, float y, float ex, float ey, int width, int height)
{
    if(x + width < ex || x > ex + width || y + height < ey || y > ey + height)
    {
        return false;
    }
    return true;
}

void stopmoving()
{
    if(dir == DOWN)
        character1.y-= movespeed;
    if (dir == LEFT)
        character1.x+=movespeed;
    if(dir == RIGHT)
        character1.x-=movespeed;
    if(dir == UP)
        character1.y+=movespeed;
}

void game_change_scene(int next_scene)
{
    //stop lagu


    //start game
    active_scene = next_scene;

    if(active_scene == START)
    {
        //music

    }

    if(active_scene == STAGE_1)
    {
        dir = UP;
        character1.x = pos_x;
        character1.y = pos_y;
        character1.w = al_get_bitmap_width(player) / 3;
        character1.h = al_get_bitmap_height(player) / 4;
        character1.image_path = player;
    }

    if(active_scene == STAGE_2)
    {
        //for restart
        al_play_sample_instance(instance3);
        al_rest(1.4);
        character1.x = pos_x;
        character1.y = pos_y;
        dir = UP;
        dialogueindex = 0;
        keyboardString="";

        //enemyx1
        enemy[0].x = 150;
        enemy[0].y = 100;
        enemy[0].vx = 0;
        enemy[0].vy = 2;
        enemy[0].w = al_get_bitmap_width(enemyx1);
        enemy[0].h = al_get_bitmap_height(enemyx1);
        enemy[0].image_path = enemyx1;
        enemy[0].hidden = true;

        //enemyx2
        enemy[1].x = 450;
        enemy[1].y = 100;
        enemy[1].vx = 0;
        enemy[1].vy = 2;
        enemy[1].w = al_get_bitmap_width(enemyx2);
        enemy[1].h = al_get_bitmap_height(enemyx2);
        enemy[1].image_path = enemyx2;
        enemy[1].hidden = true;

        //enemyx3
        enemy[2].x = 750;
        enemy[2].y = 100;
        enemy[2].vx = 0;
        enemy[2].vy = 2;
        enemy[2].w = al_get_bitmap_width(enemyx3);
        enemy[2].h = al_get_bitmap_height(enemyx3);
        enemy[2].image_path = enemyx3;
        enemy[2].hidden = true;

        //enemyx4
        enemy[3].x = 1050;
        enemy[3].y = 100;
        enemy[3].vx = 0;
        enemy[3].vy = 2;
        enemy[3].w = al_get_bitmap_width(enemyx4);
        enemy[3].h = al_get_bitmap_height(enemyx4);
        enemy[3].image_path = enemyx4;
        enemy[3].hidden = true;

        //boss 1
        boss[0].x = 250;
        boss[0].y = 0;
        boss[0].vx = 0;
        boss[0].vy = 3;
        boss[0].w = al_get_bitmap_width(enemyi);
        boss[0].h = al_get_bitmap_height(enemyi);
        boss[0].image_path = enemyi;
        boss[0].hidden = true;

        //boss 2
        boss[1].x = 950;
        boss[1].y = 0;
        boss[1].vx = 0;
        boss[1].vy = 3;
        boss[1].w = al_get_bitmap_width(enemys);
        boss[1].h = al_get_bitmap_height(enemys);
        boss[1].image_path = enemys;
        boss[1].hidden = true;
    }

    if(active_scene == STAGE_3)
    {
        al_play_sample_instance(instance3);
        al_rest(1.4);
        character1.x = pos_x;
        character1.y = 1650;
        character1.image_path = player;
        dir = UP;
        dialogueindex = 0;
        keyboardString="";

        paper.x = 600;
        paper.y = 100;
        paper.vx = 0;
        paper.vy = 0;
        paper.w = al_get_bitmap_width(paperimg);
        paper.h = al_get_bitmap_height(paperimg);
        paper.image_path = paperimg;
        paper.hidden = false;
    }

    if(active_scene == ENDING)
    {
        al_play_sample_instance(instance3);
        al_rest(1.4);
        character1.x = pos_x;
        character1.y = pos_y - 100;
        dir = UP;
        dialogueindex = 0;
    }

    if(active_scene == LOSE)
    {
        counter = 0;
    }

}

int main(int argc, char *argv[])
{
    int msg = 0;
    printf("Loading...\n");
    game_init(); // allegro init
    game_begin(); // game init

    while (msg != GAME_TERMINATE)
    {
        msg = game_run();
        game_draw();
        if (msg == GAME_TERMINATE)
            printf("Bye\n");
    }
    game_destroy();
    return 0;
}


void show_err_msg(int msg)
{
    fprintf(stderr, "unexpected msg: %d", msg);
    game_destroy();
    exit(9);
}

void game_init()
{
    if (!al_init())
        show_err_msg(-1);

    //Initialize Timer
    float FPS = 60.0;
    timer  = al_create_timer(1.0/FPS);
    al_start_timer(timer);


    event_queue = al_create_event_queue();
    display = al_create_display(width,height);

    al_set_window_position(display, 0, 0);
    al_set_window_title(display, gametitle);
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_acodec_addon();
    al_install_audio();
    al_install_keyboard();

    al_register_event_source(event_queue,al_get_display_event_source(display));
    al_register_event_source(event_queue,al_get_keyboard_event_source());
    al_register_event_source(event_queue,al_get_timer_event_source(timer));
}


void game_begin()
{

    ALLEGRO_COLOR al_map_rgb(unsigned char r, unsigned char g, unsigned char b);
    font = al_load_ttf_font("Wildy-Sans.ttf", 40, 0);
    font2 = al_load_ttf_font("Wildy-Sans.ttf", 20, 0);
    player = al_load_bitmap("spritejohnny.png");
    bg = al_load_bitmap("title_bg.png");
    title = al_load_bitmap("title.png");
    shadow = al_load_bitmap("shadow.png");
    papa = al_load_bitmap("papa.png");
    titlejohnny = al_load_bitmap("title_johnny.png");

    //for stage1
    papa_1 = al_load_bitmap("papa_1.png");
    textbox = al_load_bitmap("textbox.png");
    family = al_load_bitmap("family.png");
    frame = al_load_bitmap("frame.png");
    door = al_load_bitmap("door.png");
    bg = al_load_bitmap("title_bg.png");

    //for stage2
    textbox2 = al_load_bitmap("textbox2.png");
    enemyx1 = al_load_bitmap("x1.png");
    enemyx2 = al_load_bitmap("x2.png");
    enemyx3 = al_load_bitmap("x3.png");
    enemyx4 = al_load_bitmap("x4.png");
    enemyi = al_load_bitmap("i.png");
    enemys = al_load_bitmap("s.png");

    //for STAGE_3
    long_bg = al_load_bitmap("long_bg.bmp");
    fox = al_load_bitmap("fox.png");
    paperimg = al_load_bitmap("paper.png");

    //for STAGE_4
    end_bg = al_load_bitmap("end_bg.png");
    mama = al_load_bitmap("mama.png");

    //if player died
    youdied = al_load_bitmap("youdied.png");

    song = al_load_sample("liar.wav");
    footsteps = al_load_sample("footsteps.wav");
    crunchsound = al_load_sample("crunchsound.wav");
    doorsound = al_load_sample("doorsound.wav");
    laughter = al_load_sample("laughter.wav");
    bass = al_load_sample("bass.wav");
    al_reserve_samples(6);

    instance1 = al_create_sample_instance(footsteps);
    instance2 = al_create_sample_instance(crunchsound);
    instance3 = al_create_sample_instance(doorsound);
    instance4 = al_create_sample_instance(laughter);
    instance5 = al_create_sample_instance(bass);
    al_attach_sample_instance_to_mixer(instance1, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance2, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance3, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance4, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance5, al_get_default_mixer());

    if (!song)
    {
        printf( "Audio clip sample not loaded!\n" );
        show_err_msg(-6);
    }

    //Loop the song until the display closes
    al_play_sample(song, 1,0,1,ALLEGRO_PLAYMODE_LOOP, NULL);

    game_change_scene(START);
}

int process_event()
{
    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE keystate;
    al_wait_for_event(event_queue, &event);
    al_get_keyboard_state(&keystate);
    al_get_keyboard_event_source();
    al_get_timer_event_source(timer);

    if(event.type == ALLEGRO_EVENT_TIMER)
    {
        //close game
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            return GAME_TERMINATE;
        if(al_key_down(&keystate, ALLEGRO_KEY_ESCAPE))
            return GAME_TERMINATE;

        //start
        if(active_scene == START)
        {
            if(al_key_down(&keystate, ALLEGRO_KEY_ENTER))
            {
                game_change_scene(STAGE_1);
            }
        }
    }

    //stage1
    if(active_scene == STAGE_1)
    {
        moveInside(width, height);

        if(al_key_down(&keystate, ALLEGRO_KEY_Z))
        {
            if(hitframe)
                hitframe = false;
            if(hitdoor1)
                hitdoor1 = false;
            if(wronganswer)
                wronganswer = false;
        }
        if(Collision(character1.x, character1.y,260,160, al_get_bitmap_width(frame)/2+10, al_get_bitmap_height(frame)/2))
        {
            stopmoving();
            hitframe = true;
        }

        if(Collision(character1.x, character1.y,585,0, al_get_bitmap_width(door)/2-10, al_get_bitmap_height(door)/2))
        {
            hitdoor1 = true;
            stopmoving();
        }

        if(event.type == ALLEGRO_EVENT_KEY_CHAR && hitdoor1)
        {
            const int inputChar = event.keyboard.unichar;
            if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
            {
                if( keyboardString.length() > 0)
                {
                    keyboardString = keyboardString.substr( 0, keyboardString.length() - 1);
                    maxWord[0]++;
                }
            }
            else if (inputChar >= 97 && inputChar <= 122 && maxWord[0]>0) //is a lower-case letter
            {
                keyboardString += inputChar;
                maxWord[0]--;
            }
        }

        if(maxWord[0]==0 && al_key_down(&keystate, ALLEGRO_KEY_ENTER))
        {
            if(strcmp(inputString,"mama")==0)
            {
                game_change_scene(STAGE_2);
                hitdoor1=false;
            }

            else
            {
                hitdoor1 = false;
                wronganswer = true;
                keyboardString = "";
                maxWord[0]=4;
            }
        }
        if(al_key_down(&keystate, ALLEGRO_KEY_2))
            game_change_scene(STAGE_2);
        if(al_key_down(&keystate, ALLEGRO_KEY_3))
            game_change_scene(STAGE_3);
        if(al_key_down(&keystate, ALLEGRO_KEY_4))
            game_change_scene(ENDING);


        if(al_key_down(&keystate, ALLEGRO_KEY_DOWN) || al_key_down(&keystate, ALLEGRO_KEY_UP) || al_key_down(&keystate, ALLEGRO_KEY_RIGHT) || al_key_down(&keystate, ALLEGRO_KEY_LEFT))
            wronganswer = false;

    }

    //stage2
    if(active_scene == STAGE_2)
    {
        moveInside(width, height);

        if(Collision(character1.x, character1.y,585,0, al_get_bitmap_width(door)/2-10, al_get_bitmap_height(door)/2))
        {
            hitdoor2 = true;
            state=IDLE;
            stopmoving();
        }

        if(al_key_down(&keystate, ALLEGRO_KEY_Z))
        {
            if(hitdoor2 && dialogueindex==8)
            {
                hitdoor2=false;
                dialogueindex = 6;
            }
            if(wronganswer)
                wronganswer = false;
        }

        int sz = sizeof(dialogue2a)/sizeof(dialogue2a[0]);
        if(sz-3 == dialogueindex)
        {
            enemy[0].hidden = false;
            enemy[1].hidden = false;
            enemy[2].hidden = false;
            enemy[3].hidden = false;
        }

        if(dialogueindex >= 6 && startboss == false)
            startmove = true;

        if(stop_enemy)
        {
            boss[0].hidden = true;
            boss[1].hidden = true;
            enemy[0].hidden = true;
            enemy[1].hidden = true;
            enemy[2].hidden = true;
            enemy[3].hidden = true;
            startboss = false;
            startmove = false;
        }
        //8 enemies have died
        if(startboss)
        {
            enemy[0].hidden = true;
            enemy[1].hidden = true;
            enemy[2].hidden = true;
            enemy[3].hidden = true;
            startmove = false;

            if(counter == 100)
                stop_enemy = true;

            int i;
            for (i = 0; i < 2; i++)
            {
                if (boss[i].hidden == true)
                    continue;

                boss[i].x += boss[i].vx;
                boss[i].y += boss[i].vy;

                if (boss[i].y > height)
                    boss[i].hidden = true;
            }
            for (i = 0; i < 2; i++)
                if (boss[i].hidden == true)
                    break;



            for(i = 0; i < 2; i++)
            {
                if(!character1.hidden)
                {
                    if(!boss[i].hidden)
                    {
                        if(boss[i].x - boss[i].w/2 < character1.x + character1.w/2 && boss[i].x + boss[i].w/2 > character1.x - character1.w/2
                                && boss[i].y - boss[i].h/2 < character1.y + character1.h/2 && boss[i].y + boss[i].h/2 > character1.y - character1.h/2)
                        {
                            game_change_scene(LOSE);
                            break;
                        }
                    }
                }
            }
            //to eliminate boss
            if(al_key_down(&keystate, ALLEGRO_KEY_I) && state!=IDLE)
            {
                al_play_sample_instance(instance2);
                boss[0].hidden = true;
                counter++;
            }
            if(al_key_down(&keystate, ALLEGRO_KEY_S) && state!=IDLE)
            {
                al_play_sample_instance(instance2);
                boss[1].hidden = true;
                counter++;
            }
        }
        //for enemy
        if(startmove)
        {
            //to eliminate enemies
            double now_dead1 = al_get_time();
            if(al_key_down(&keystate, ALLEGRO_KEY_X))
            {
                al_play_sample_instance(instance2);
                enemy1_die_timestamp = al_get_time();
                enemy[0].hidden = true;
                counter++;
            }

            double now_dead2 = al_get_time();
            if(al_key_down(&keystate, ALLEGRO_KEY_C))
            {
                enemy2_die_timestamp = al_get_time();
                al_play_sample_instance(instance2);
                enemy[1].hidden = true;
                counter++;
            }

            double now_dead3 = al_get_time();
            if(al_key_down(&keystate, ALLEGRO_KEY_M))
            {
                enemy3_die_timestamp = al_get_time();
                al_play_sample_instance(instance2);
                enemy[2].hidden = true;
                counter++;
            }

            double now_dead4 = al_get_time();
            if(al_key_down(&keystate, ALLEGRO_KEY_P))
            {
                enemy4_die_timestamp = al_get_time();
                al_play_sample_instance(instance2);
                enemy[3].hidden = true;
                counter++;
            }

            int i;
            for (i = 0; i < 4; i++)
            {
                if (enemy[i].hidden == true)
                    continue;

                enemy[i].x += enemy[i].vx;
                enemy[i].y += enemy[i].vy;

                if (enemy[i].y > height)
                    enemy[i].hidden = true;
            }

            for (i = 0; i < 4; i++)
                if (enemy[i].hidden == true)
                    break;

            if(i == 0 && enemy[i].hidden == true)
            {
                if(now_dead1 - enemy1_die_timestamp >= DEAD_TIME)
                {
                    enemy[i].y = 0;
                    enemy[i].hidden = false;
                    enemy[i].x = enemy[i].w / 2 + (float)rand() / RAND_MAX * (width - enemy[i].w);
                }
            }
            else if(i == 1 && enemy[i].hidden == true)
            {
                if(now_dead2 - enemy2_die_timestamp >= DEAD_TIME)
                {
                    enemy[i].y = 0;
                    enemy[i].hidden = false;
                    enemy[i].x = enemy[i].w / 2 + (float)rand() / RAND_MAX * (width - enemy[i].w);
                }
            }
            else if(i == 2 && enemy[i].hidden == true)
            {
                if(now_dead3 - enemy3_die_timestamp >= DEAD_TIME)
                {
                    enemy[i].y = 0;
                    enemy[i].hidden = false;
                    enemy[i].x = enemy[i].w / 2 + (float)rand() / RAND_MAX * (width - enemy[i].w);
                }
            }
            else if(i == 3 && enemy[i].hidden == true)
            {
                if(now_dead4 - enemy4_die_timestamp >= DEAD_TIME)
                {
                    enemy[i].y = 0;
                    enemy[i].hidden = false;
                    enemy[i].x = enemy[i].w / 2 + (float)rand() / RAND_MAX * (width - enemy[i].w);
                }
            }



            if(counter >= 70)
            {
                startboss = true;
                boss[0].hidden = false;
                boss[1].hidden = false;
            }

            //for enemy collision
            for(i = 0; i < 4; i++)
            {
                if(!character1.hidden)
                {
                    if(!enemy[i].hidden)
                    {
                        if(enemy[i].x - enemy[i].w/2 < character1.x + character1.w/2 && enemy[i].x + enemy[i].w/2 > character1.x - character1.w/2
                                && enemy[i].y - enemy[i].h/2 < character1.y + character1.h/2 && enemy[i].y + enemy[i].h/2 > character1.y - character1.h/2)
                        {
                            game_change_scene(LOSE);
                            break;
                        }
                    }
                }
            }

        }

        if(event.type == ALLEGRO_EVENT_KEY_CHAR && hitdoor2 && dialogueindex==7)
        {
            const int inputChar = event.keyboard.unichar;
            if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
            {
                if( keyboardString.length() > 0)
                {
                    keyboardString = keyboardString.substr( 0, keyboardString.length() - 1);
                    maxWord[1]++;
                }
            }
            else if (inputChar >= 97 && inputChar <= 121 && maxWord[1]>0)
            {
                keyboardString += inputChar;
                maxWord[1]--;
            }
        }

        if(maxWord[1]==0 && al_key_down(&keystate, ALLEGRO_KEY_ENTER))
        {
            if(strcmp(keyboardString.c_str(),"is")==0)
            {
                game_change_scene(STAGE_3);
                hitdoor2=false;
            }
            else
            {
                hitdoor2 = false;
                wronganswer = true;
                keyboardString = "";
                dialogueindex=6;
                maxWord[1]=2;
            }
        }

        if(al_key_down(&keystate, ALLEGRO_KEY_DOWN) || al_key_down(&keystate, ALLEGRO_KEY_UP) || al_key_down(&keystate, ALLEGRO_KEY_RIGHT) || al_key_down(&keystate, ALLEGRO_KEY_LEFT))
            wronganswer = false;
        }

    if(active_scene == LOSE)
    {
        if(al_key_down(&keystate, ALLEGRO_KEY_SPACE))
        {
            startmove = false;
            game_change_scene(STAGE_2);
        }
    }

    if(active_scene == STAGE_3)
    {

        moveInside(width, 2000);
        cameraUpdate(cameraPos_y, character1.y, 100);
        al_identity_transform(&camera);
        al_translate_transform(&camera,0,-cameraPos_y);
        al_use_transform(&camera);


        int sz = sizeof(dialogue3)/sizeof(dialogue3[0]);

        if(dialogueindex>=sz)
            dialogueindex == sz;

        if(character1.y == 1500)
            stop1 = true;
        if(dialogueindex==2)
            stop1 = false;
        if(character1.y == 1000)
            stop2 = true;
        if(dialogueindex==4)
            stop2 = false;
        if(character1.y == 500)
            stop3 = true;
        if(dialogueindex==6)
            stop3 = false;

        if(Collision(character1.x, character1.y,550,0, al_get_bitmap_width(door)/2-10, al_get_bitmap_height(door)/2))
        {
            stopmoving();
            if(sign==0)
            {
                hitpaper = true;
                hitdoor3 = false;
                printf("sign : %d\n",sign);
            }
            else if (sign==1)
            {
                hitdoor3 = true;
                hitpaper = false;
                printf("sign : %d\n",sign);
            }
        }

        if(dialogueindex == 7 && al_key_down(&keystate, ALLEGRO_KEY_Z))
        {
            if(paper.hidden == false)
            {
                paper.hidden = true;
                sign=1;
            }
        }

        if(paper.hidden == true && al_key_down(&keystate, ALLEGRO_KEY_SPACE))
        {
            showpaper = true;
        }
        else
            showpaper = false;

        if(event.type == ALLEGRO_EVENT_KEY_CHAR && hitdoor3)
        {
            const int inputChar = event.keyboard.unichar;
            if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
            {
                if( keyboardString.length() > 0)
                {
                    keyboardString = keyboardString.substr( 0, keyboardString.length() - 1);
                    maxWord[2]++;
                }
            }
            else if (inputChar >= 97 && inputChar <= 121 && maxWord[2]>0) //is a lower-case letter
            {
                keyboardString += inputChar;
                maxWord[2]--;
            }
        }

        if(maxWord[2]==0 && al_key_down(&keystate, ALLEGRO_KEY_ENTER))
        {
            if(strcmp(inputString,"dead")==0)
            {
                game_change_scene(ENDING);
                hitdoor3=false;
            }

            else
            {
                hitdoor3 = false;
                wronganswer = true;
                keyboardString = "";
                maxWord[2]=4;
            }
        }

        if(al_key_down(&keystate, ALLEGRO_KEY_DOWN) || al_key_down(&keystate, ALLEGRO_KEY_UP) || al_key_down(&keystate, ALLEGRO_KEY_RIGHT) || al_key_down(&keystate, ALLEGRO_KEY_LEFT))
            wronganswer = false;
    }

    if(active_scene == ENDING)
    {
        int sz = sizeof(dialogue4) / sizeof(dialogue4[0]);
        if(dialogueindex == sz && al_key_down(&keystate, ALLEGRO_KEY_Z))
        {
            game_change_scene(FINISH);
        }

        if(dialogueindex == sz-1)
            al_play_sample_instance(instance4);

        if(dialogueindex == 5)
            al_play_sample_instance(instance5);

        if(al_key_down(&keystate, ALLEGRO_KEY_ESCAPE))
            return GAME_TERMINATE;
    }



    if(state != IDLE && event.type == ALLEGRO_EVENT_TIMER)
    {
        active = true;
        if(al_key_down(&keystate, ALLEGRO_KEY_DOWN))
        {
            character1.y += movespeed;
            dir = DOWN;
        }
        else if(al_key_down(&keystate, ALLEGRO_KEY_UP))
        {
            character1.y -= movespeed;
            dir = UP;
        }
        else if(al_key_down(&keystate, ALLEGRO_KEY_RIGHT))
        {
            character1.x += movespeed;
            dir = RIGHT;
        }
        else if(al_key_down(&keystate, ALLEGRO_KEY_LEFT))
        {
            character1.x -= movespeed;
            dir = LEFT;
        }
        else
            active = false;

        if(active)
        {
            sourcex += al_get_bitmap_width(player) / 3;
            if((al_get_timer_count(timer) % 12))
                al_play_sample_instance(instance1);
        }
        else
            sourcex = 100;
        if(sourcex >= al_get_bitmap_width(player))
            sourcex = 0;
        sourcey = dir;
        draw = true;
    }

    /*if(event.type == ALLEGRO_EVENT_TIMER && (!al_get_timer_count(timer) % 12))
    {
        if(al_key_down(&keystate, ALLEGRO_KEY_UP) || al_key_down(&keystate, ALLEGRO_KEY_DOWN) || al_key_down(&keystate, ALLEGRO_KEY_RIGHT) || al_key_down(&keystate, ALLEGRO_KEY_LEFT))
        {
            al_play_sample_instance(instance1);
        }
    }*/


    while(Collision(character1.x, character1.y,950,280, al_get_bitmap_width(papa_1)/2+10, al_get_bitmap_height(papa_1)/2))
    {
        stopmoving();
    }

    if(event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        if(event.keyboard.keycode == ALLEGRO_KEY_Z)
            dialogueindex++;
        if(hitpaper)
            hitpaper=false;
    }


    if(al_key_down(&keystate, ALLEGRO_KEY_P))
        printf("x %f, y %f", character1.x, character1.y);

    return 0;
}



int game_run()
{
    int error = 0;

    //start
    if(active_scene == START)
    {
        if (!al_is_event_queue_empty(event_queue))
            error = process_event();
    }
    //stage 1
    else if(active_scene == STAGE_1)
    {
        if (!al_is_event_queue_empty(event_queue))
            error = process_event();
    }
    //stage 2
    else if(active_scene == STAGE_2)
    {
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
        }
    }
    //stage 3
    else if(active_scene == STAGE_3)
    {
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
        }
    }
    //ending
    else if(active_scene == ENDING)
    {
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
        }
    }
    //LOSE
    else if(active_scene == LOSE)
    {
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
        }
    }

    else if(active_scene == FINISH)
    {
        if (!al_is_event_queue_empty(event_queue))
        {
            error = process_event();
        }
    }


    return error;
}

void game_destroy()
{
    al_destroy_display(display);
    al_destroy_font(font);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_bitmap(player);
    al_destroy_bitmap(papa);
    al_destroy_bitmap(enemyx1);
    al_destroy_bitmap(enemyx2);
    al_destroy_bitmap(enemyx3);
    al_destroy_bitmap(enemyx4);
    al_destroy_bitmap(enemyi);
    al_destroy_bitmap(enemys);
    al_destroy_bitmap(title);
    al_destroy_bitmap(bg);
    al_destroy_bitmap(titlejohnny);
    al_destroy_bitmap(papa_1);
    al_destroy_bitmap(textbox);
    al_destroy_bitmap(textbox2);
    al_destroy_bitmap(family);
    al_destroy_bitmap(frame);
    al_destroy_bitmap(door);
    al_destroy_bitmap(long_bg);
    al_destroy_bitmap(fox);
    al_destroy_bitmap(paperimg);
    al_destroy_bitmap(mama);
    al_destroy_bitmap(end_bg);
    al_uninstall_keyboard();
    al_destroy_sample(song);
    al_destroy_sample(footsteps);
    al_destroy_sample_instance(instance1);
}
