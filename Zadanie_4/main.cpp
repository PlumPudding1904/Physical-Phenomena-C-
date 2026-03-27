#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
double **siec;
int L=40;
int N=L*L;
double J=1.0;
double losowa_naab(double a, double b)
{
    double u=(double)(rand()%RAND_MAX)/(double)RAND_MAX;
    return a+(b-a)*u;
}
void inicjalizacja()
{
    siec=new double*[L];
    for(int i=0; i<L; i++)
        siec[i]=new double[L];
    for(int i=0; i<L; i++)
        for(int j=0; j<L; j++)
            siec[i][j]=losowa_naab(0.0,2.0*M_PI);
}
void krokmc(double T, double h)
{
    int i=rand()%L;
    int j=rand()%L;
    double dE=0.0;
    double dTheta=losowa_naab(-0.5,0.5);
    double nowykat=siec[i][j]+dTheta;
    dE+=-0.5+1.5*cos(siec[i][j]-siec[(i+1)%L][j])*(cos(siec[i][j]-siec[(i+1)%L][j]));
    dE+=-0.5+1.5*cos(siec[i][j]-siec[(i-1+L)%L][j])*(cos(siec[i][j]-siec[(i-1+L)%L][j]));
    dE+=-0.5+1.5*cos(siec[i][j]-siec[i][(j+1)%L ])*(cos(siec[i][j]-siec[i][(j+1)%L]));
    dE+=-0.5+1.5*cos(siec[i][j]-siec[i][(j-1+L)%L])*(cos(siec[i][j]-siec[i][(j-1+L)%L]));
    dE-=-0.5+1.5*cos(nowykat-siec[(i+1)%L][j] )*(cos(nowykat-siec[(i+1)%L][j]));
    dE-=-0.5+1.5*cos(nowykat-siec[(i-1+L)%L][j])*(cos(nowykat-siec[(i-1+L)%L][j]));
    dE-=-0.5+1.5*cos(nowykat-siec[i][(j+1)%L ])*(cos(nowykat-siec[i][(j+1)%L]));
    dE-=-0.5+1.5*cos(nowykat-siec[i][(j-1+L)%L])*(cos(nowykat-siec[i][(j-1+L)%L]));
    if(dE<0.0)
    {
    siec[i][j]=nowykat;
    }
    else
    {
        double losowa=losowa_naab(0.0,1.0);
        if(losowa<exp(-dE/T))siec[i][j]=nowykat;
    }
}
int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    ALLEGRO_DISPLAY *display=al_create_display(600,600);
//ALLEGRO_DISPLAY *display2=al_create_display(1000,200);
//ALLEGRO_DISPLAY *display3=al_create_display(1000,200);
    ALLEGRO_FONT *czcionka=al_load_ttf_font("czcionka.ttf",18,0);
    bool redraw=true;
    const float FPS=60;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue,al_get_display_event_source(display));
//al_register_event_source(event_queue,al_get_display_event_source(display2));
//al_register_event_source(event_queue,al_get_display_event_source(display3));
    al_register_event_source(event_queue,al_get_timer_event_source(timer));
    ALLEGRO_COLOR czerwony=al_map_rgb(255,0,0);
    ALLEGRO_COLOR niebieski=al_map_rgb(0,0,255);
    ALLEGRO_COLOR bialy=al_map_rgb(255,255,255);
    ALLEGRO_COLOR szary=al_map_rgb(127,127,127);
    ALLEGRO_COLOR zielony=al_map_rgb(0,255,0);
    FILE *plik=fopen("wynik.txt","w");
    //double T=1.0;
    int licznik=0;
    inicjalizacja();
    double sinus,kosinus;
    double skala_x=(double)al_get_bitmap_width(al_get_backbuffer(display))/(double)L;
    double skala_y=(double)al_get_bitmap_height(al_get_backbuffer(display))/(double)L;
    double wskx,wsky;
    ALLEGRO_COLOR kolor;
    double Qxx,Qxy,Qyx,Qyy;
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
            al_clear_to_color(al_map_rgb(0,0,0));
            for(int i=0; i<L*L; i++)
                krokmc(0.2,0.0);
            Qxx=0.0;
            Qxy=0.0;
            Qyx=0.0;
            Qyy=0.0;
            for(int i=0; i<L; i++)
                for(int j=0; j<L; j++)
                {
                    Qxx+=cos(siec[i][j])*cos(siec[i][j])-0.5;
                    Qxy+=cos(siec[i][j])*sin(siec[i][j]);
                    Qyx+=sin(siec[i][j])*cos(siec[i][j]);
                    Qyy+=sin(siec[i][j])*sin(siec[i][j])-0.5;
                }
            Qxx/=(double)N;
            Qxy/=(double)N;
            Qyx/=(double)N;
            Qyy/=(double)N;
            printf("Qxx=%.5lf\tQxy=%.5lf\tQyx=%.5lf\tQyy=%.5lf\n",Qxx,Qxy,Qyx,Qyy);
            printf("S=%.5lf\n",sqrt(Qxx*Qxx+Qxy*Qxy));
            fprintf(plik,"%.5lf\n",sqrt(Qxx*Qxx+Qxy*Qxy));
            for(int i=0; i<L; i++)
                for(int j=0; j<L; j++)
                {
                    kolor=al_map_rgb(
                              (cos(2.0*siec[i][j])+1.0)*0.5*255.0,0,
                              255-(cos(2.0*siec[i][j])+1.0)*0.5*255.0);
                    al_draw_filled_rectangle(skala_x*i,skala_y*j,skala_x*(i+1),skala_y*(j+1),kolor);
                }
            for(int i=0; i<L; i++)
                for(int j=0; j<L; j++)
                {
                    sinus=sin(siec[i][j]);
                    kosinus=cos(siec[i][j]);
                    wskx=0.5*kosinus*skala_x;
                    wsky=0.5*sinus*skala_y;
                    al_draw_line(skala_x*(i+0.5)-wskx,skala_y*(j+0.5)-wsky,
                                 skala_x*(i+0.5)+wskx,skala_y*(j+0.5)+wsky,
                                 bialy,1.0);
// al_draw_filled_circle(skala_x*(i+0.5)+wskx,skala_y*(j+0.5)+wsky,
// 2.0,bialy);
                }
            for(int i=0; i<L; i++)
                for(int j=0; j<L; j++)
                {
                    double kat1=fabs(siec[i][j]-siec[(i+1)%L][j]);
                    double kat2=fabs(siec[i][j]-siec[(i-1+L)%L][j]);
                    double kat3=fabs(siec[i][j]-siec[i][(j+1)%L]);
                    double kat4=fabs(siec[i][j]-siec[i][(j-1+L)%L]);
                    if((kat1>1.31&&kat1<1.83)||(kat1>4.45&&kat1<4.97))
                        al_draw_rectangle(skala_x*i,skala_y*j,skala_x*(i+1),skala_y*(j+1),zielony,2.0);
                    if((kat2>1.31&&kat2<1.83)||(kat2>4.45&&kat2<4.97))
                        al_draw_rectangle(skala_x*i,skala_y*j,skala_x*(i+1),skala_y*(j+1),zielony,2.0);
                    if((kat3>1.31&&kat3<1.83)||(kat3>4.45&&kat3<4.97))
                        al_draw_rectangle(skala_x*i,skala_y*j,skala_x*(i+1),skala_y*(j+1),zielony,2.0);
                    if((kat4>1.31&&kat4<1.83)||(kat4>4.45&&kat4<4.97))
                        al_draw_rectangle(skala_x*i,skala_y*j,skala_x*(i+1),skala_y*(j+1),zielony,2.0);
                }
            al_flip_display();
        }
    }
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    fclose(plik);
    return 0;
}
