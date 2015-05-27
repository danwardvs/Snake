#include<allegro.h>
#include<alpng.h>
#include<time.h>

BITMAP* buffer;
BITMAP* background;

bool close_button_pressed;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 10;
volatile int game_time = 0;


int fps;
int frames_done;
int old_time;

void ticker(){
  ticks++;
}
END_OF_FUNCTION(ticker)

void game_time_ticker(){
  game_time++;
}
END_OF_FUNCTION(ticker)

void close_button_handler(void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

// Random number generator. Use int random(highest,lowest);
int random(int newLowest, int newHighest)
{
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}
struct snake{
    int x;
    int y;
    int direction;

}snakes[100];

int food_x=10;
int food_y=10;

int snake_size=1;


//A function to streamline error reporting in file loading
void abort_on_error(const char *message){
	 set_window_title("Error!");
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}

void update(){

    if(food_x==snakes[0].x && food_y==snakes[0].y){
        snake_size++;
        snakes[snake_size].x=snakes[0].x-1;
        snakes[snake_size].y=snakes[0].y;
        food_x=random(0,20);
        food_y=random(0,20);
    }

    if(key[KEY_LEFT])snakes[0].direction=1;
    if(key[KEY_UP])snakes[0].direction=2;
    if(key[KEY_RIGHT])snakes[0].direction=3;
    if(key[KEY_DOWN])snakes[0].direction=4;

    for( int i=0; i<snake_size; i++){
       if(snakes[i].direction==1){
            snakes[i].x-=1;
       }
       if(snakes[i].direction==2){
            snakes[i].y-=1;
       }
       if(snakes[i].direction==3){
            snakes[i].x+=1;
       }
       if(snakes[i].direction==4){
            snakes[i].y+=1;
       }
    }
}

void draw(){


    draw_sprite(buffer,background,0,0);
    for( int i=0; i<100; i++){
        rectfill( buffer, snakes[i].x*20,snakes[i].y*20,(snakes[i].x+1)*20,(snakes[i].y+1)*20, makecol(0,0,0));
    }
    rectfill( buffer, food_x*20,food_y*20,(food_x+1)*20,(food_y+1)*20, makecol(255,0,0));





    draw_sprite(screen,buffer,0,0);
}







void setup(){
    for( int i=0; i<1; i++){
        snakes[i].x=i;
    }


    buffer=create_bitmap(800,600);


    srand(time(NULL));

     // Setup for FPS system
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    // Close button
    LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(close_button_handler);

    if (!(background = load_bitmap("background.png", NULL)))
      abort_on_error("Cannot find image background.png\nPlease check your files and try again");
}






int main(){
for( int i=0; i<100; i++){
       snakes[i].direction=3;
    }
  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);


  set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");



  set_window_title("Sci-Fi game!");
  setup();


      while(!key[KEY_ESC] && !close_button_pressed){
        while(ticks == 0){
            rest(1);
        }
    while(ticks > 0){
        int old_ticks = ticks;

        update();

        ticks--;
        if(old_ticks <= ticks){
            break;
        }
    }
        if(game_time - old_time >= 10){
            fps = frames_done;
            frames_done = 0;
            old_time = game_time;
        }
        draw();
    }


	return 0;
}
END_OF_MAIN()
