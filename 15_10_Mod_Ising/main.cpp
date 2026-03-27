#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_image.h>

int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();

    ALLEGRO_DISPLAY *display=al_create_display(500, 500);
    ALLEGRO_DISPLAY *display2=al_create_display(1000, 200);
    ALLEGRO_DISPLAY *display3=al_create_display(1000, 200);
    ALLEGRO_FONT *czcionka=al_load_ttf_font("cakecafe.ttf",18,0);

    bool redraw=true;
    const float FPS=60;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    int L=100;
    int N=L*L;
    int **spiny;
    spiny=(int**)malloc(L*sizeof(int*));

    for(int i=0; i<L; i++)
        spiny[i]=(int*)malloc(L*sizeof(int));

    for(int i=0; i<L; i++)
        for(int j=0; j<L; j++)
            spiny[i][j]=-1+2*(rand()%2);
    ALLEGRO_COLOR czerwony=al_map_rgb(255,0,0);
    ALLEGRO_COLOR niebieski=al_map_rgb(0,0,255);
    ALLEGRO_COLOR bialy=al_map_rgb(255,255,255);
    ALLEGRO_COLOR szary=al_map_rgb(127,127,127);
    ALLEGRO_COLOR zielony=al_map_rgb(0,255,0);
    double T=2.0;//2.6 granica przejœcie fazowe
    double h=3.0;
    double J=1.0;
    double liczbalosowa;
    double expB;
    double deltaE;
    int li, lj;
    double M;
    double Mt[1000];
    for(int i=0;i<1000;i++)
    {
        Mt[i]=0.0;
    }
    double Msr=0.0;

    double E;
    double Et[1000];
    for(int i=0;i<1000;i++)
    {
        Et[i]=0.0;
    }
    double Esr=0.0;

    double M2;
    double M2t[1000];
    for(int i=0;i<1000;i++)
    {
        M2t[i]=0.0;
    }
    double M2sr=0.0;

    double E2;
    double E2t[1000];
    for(int i=0;i<1000;i++)
    {
        E2t[i]=0.0;
    }
    double E2sr=0.0;
    int licznik=0;
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
            al_clear_to_color(al_map_rgb(0,0,0));

            for(int k=0; k<N; k++)
            {

                //int li=rand()%L;
                //int lj=1+rand()%L;
                li=1+rand()%(L-2);
                lj=1+rand()%(L-2);
                deltaE=2.0*J*spiny[li][lj]*(spiny[(li+1)%L][lj]+spiny[(li-1+L)%L][lj]+spiny[li][(lj+1)%L]+spiny[li][(lj-1+L)%L])+2.0*h*spiny[li][lj];;

                if(deltaE<0.0)
                {
                    spiny[li][lj]*=-1;
                }
                else
                {
                    expB=exp(-deltaE/T);
                    liczbalosowa=(double)(rand()%RAND_MAX)/(double)RAND_MAX;
                    if(liczbalosowa<expB)
                        spiny[li][lj]*=-1;
                }
            }
            M;
            M=0.0;
            E=0.0;
            for(int i=0;i<L;i++)
            {
                for(int j=0;j<L;j++)
                {
                    M+=(double)spiny[i][j];
                    E+=-0.5*J*spiny[i][j]*(spiny[(i+1)%L][j]+spiny[(i-1+L)%L][j]+spiny[i][(j+1)%L]+spiny[i][(j-1+L)%L])-h*spiny[i][j];
                }
            }
            M/=(double)N;
            Mt[licznik]=M;
            E/=(double)N;
            Et[licznik]=E;

            M2t[licznik]=M*M;
            E2t[licznik]=E*E;
            licznik++;
            licznik=licznik%1000;

            for(int i=0;i<1000;i++)
            {
                Msr=Msr+Mt[i];
                M2sr=M2sr+M2t[i];
                Esr=Esr+Et[i];
                E2sr=E2sr+E2t[i];
            }
            Msr=Msr/1000.0;
            M2sr=M2sr/1000.0;
            Esr=Esr/1000.0;
            E2sr=E2sr/1000.0;

            al_set_target_bitmap(al_get_backbuffer(display));
            al_clear_to_color(al_map_rgb(0,0,0));

            for(int i=0; i<L; i++)
                for(int j=0; j<L; j++)
                    if(spiny[i][j]==1)
                        al_draw_filled_rectangle(5*i,5*j,5*i+5,5*j+5,czerwony);
                    else
                        al_draw_filled_rectangle(5*i,5*j,5*i+5,5*j+5,niebieski);

            al_flip_display();
            al_set_target_bitmap(al_get_backbuffer(display2));
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_line(0,100,1000,100,szary,1);
            for(int i=0;i<1000;i++)
            {
                al_draw_filled_circle(i,100.0*(-Mt[i]+1.0),2,bialy);
                al_draw_filled_circle(i,100.0*(-M2t[i]+1.0),2,zielony);
                //Msr=M/i;
                M2sr=(M*M)/i;
                //printf("%lf\n",Msr);
            }
            al_draw_textf(czcionka,bialy,10,10,ALLEGRO_ALIGN_LEFT,"<M>=%f",Msr);
            al_draw_textf(czcionka,bialy,10,30,ALLEGRO_ALIGN_LEFT,"<M2>=%f",M2sr);
            al_flip_display();

            al_set_target_bitmap(al_get_backbuffer(display3));
            al_clear_to_color(al_map_rgb(0,0,0));
            for(int i=0;i<1000;i++)
            {
                al_draw_filled_circle(i,133.333-33.333*Et[i],2,bialy);
                al_draw_filled_circle(i,133.333-33.333*E2t[i],2,zielony);
            }
            al_draw_textf(czcionka,bialy,10,10,ALLEGRO_ALIGN_LEFT,"<E>=%f",Esr);
            al_draw_textf(czcionka,bialy,10,30,ALLEGRO_ALIGN_LEFT,"<E2>=%f",E2sr);
            al_flip_display();

        }
    }
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
