#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double losowa(double a,double b)
{
    double u=(double)(rand()%RAND_MAX)/(double)RAND_MAX;
    return a+(b-a)*u;
}

int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();

    const int L=400;

    ALLEGRO_DISPLAY *display=al_create_display(L,L);

    bool redraw=true;
    const float FPS=6000;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    ALLEGRO_COLOR czerwony=al_map_rgb(255,0,0);
    ALLEGRO_COLOR niebieski=al_map_rgb(0,0,255);
    ALLEGRO_COLOR bialy=al_map_rgb(255,255,255);
    ALLEGRO_COLOR szary=al_map_rgb(127,127,127);
    ALLEGRO_COLOR zielony=al_map_rgb(0,255,0);

    bool **siec=(bool**)malloc(L*sizeof(bool*));
    for(int i=0;i<L;i++)siec[i]=(bool*)malloc(L*sizeof(bool));

    for(int i=0;i<L;i++)
        for(int j=0;j<L;j++)
            siec[i][j]=false;

    siec[L/2][L/2]=true;

    int histogram[L/2];
    for(int i=0;i<L/2;i++)histogram[i]=0;
    histogram[0]=1;
    int histogramc[L/2];
    for(int i=0;i<L/2;i++)histogramc[i]=0;


    srand(time(NULL));
   while(1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue,&ev);
        if(ev.type==ALLEGRO_EVENT_TIMER)
        {
            redraw=true;
        }
        else if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            al_set_target_bitmap(al_get_backbuffer(display));
           // al_clear_to_color(al_map_rgb(0,0,0));

            for(int prob=0;prob<50;prob++)
            {

            double kat=losowa(0.0,2.0*M_PI);
            double r=(L-1.0)/2.0;
            int x=(int)(r*cos(kat))+L/2;
            int y=(int)(r*sin(kat))+L/2;

            do
            {
                double los=losowa(0.0,1.0);
                if(los<0.25)y++;
                else if(los<0.5 && los>=0.25)y--;
                else if(los<0.75 && los>=0.5)x++;
                else x--;
                if(x<1)x=L-2;
                if(x>L-2)x=1;
                if(y<1)y=L-2;
                if(y>L-2)y=1;
                if(siec[x-1][y]==true||siec[x+1][y]==true||siec[x][y-1]==true||siec[x][y+1]==true)
                    siec[x][y]=true;
            }while(siec[x][y]==false);
            double histr=sqrt((x-0.5*L)*(x-0.5*L)+(y-0.5*L)*(y-0.5*L));
            if((int)histr<L/2)
                histogram[(int)histr]++;
            }
            for(int i=0;i<L;i++)
                for(int j=0;j<L;j++)
                    al_draw_pixel(i,j,al_map_rgb(255*siec[i][j],255*siec[i][j],255*siec[i][j]));

            al_flip_display();
        }
    }
    for(int i=0;i<L/2;i++)
        for(int j=0;j<=i;j++)
        histogramc[i]+=histogram[j];
    FILE *plik=fopen("wynik.txt","w");
    for(int i=0;i<L/2;i++)
    {
        fprintf(plik,"%d\t%d\n",i,histogramc[i]);
    }
    fclose(plik);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
