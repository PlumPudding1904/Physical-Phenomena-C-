#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_image.h>

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
    {
        siec[i]=new double[L];
    }
    for(int i=0; i<L; i++)
    {
        for(int j=0; j<L; j++)
            siec[i][j]=losowa_naab(0.0,2.0*M_PI);
    }
}

void krokmc(double T, double h)
{
    int i=rand()%L;
    int j=rand()%L;
    double dE=0.0;
    double dTheta=losowa_naab(-0.5,0.5);
    double nowykat=siec[i][j]+dTheta;
    dE+=-0.5*J*(-1.0+3*cos(siec[i][j]-siec[(i+1)%L][j])*cos(siec[i][j]-siec[(i+1)%L][j]))+h*cos(siec[i][j]);
    dE+=-0.5*J*(-1.0+3*cos(siec[i][j]-siec[(i-1+L)%L][j])*cos(siec[i][j]-siec[(i-1+L)%L][j]))+h*cos(siec[i][j]);
    dE+=-0.5*J*(-1.0+3*cos(siec[i][j]-siec[i][(j+1)%L])*cos(siec[i][j]-siec[i][(j+1)%L]))+h*cos(siec[i][j]);
    dE+=-0.5*J*(-1.0+3.0*cos(siec[i][j]-siec[i][(j-1+L)%L])*cos(siec[i][j]-siec[i][(j-1+L)%L]))+h*cos(siec[i][j]);

    dE-=-0.5*J*(-1.0+3.0*cos(nowykat-siec[(i+1)%L][j])*cos(nowykat-siec[(i+1)%L][j]))+h*cos(nowykat);
    dE-=-0.5*J*(-1.0+3.0*cos(nowykat-siec[(i-1+L)%L][j])*cos(nowykat-siec[(i-1+L)%L][j]))+h*cos(nowykat);
    dE-=-0.5*J*(-1.0+3.0*cos(nowykat-siec[i][(j+1)%L])*cos(nowykat-siec[i][(j+1)%L]))+h*cos(nowykat);
    dE-=-0.5*J*(-1.0+3.0*cos(nowykat-siec[i][(j-1+L)%L])*cos(nowykat-siec[i][(j-1+L)%L]))+h*cos(nowykat);
    if(dE<0.0)
    {
        siec[i][j]=nowykat;
    }
    else
    {
        double losowa=losowa_naab(0.0,1.0);
        if(losowa<exp(-dE/T))
            siec[i][j]=nowykat;
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

    ALLEGRO_DISPLAY *display=al_create_display(500, 500);
    //ALLEGRO_DISPLAY *display2=al_create_display(1000, 200);
    //ALLEGRO_DISPLAY *display3=al_create_display(1000, 200);
    ALLEGRO_FONT *czcionka=al_load_ttf_font("cakecafe.ttf",18,0);

    bool redraw=true;
    const float FPS=60;
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
    //double T=1.0;//2.6 granica przejœcie fazowe
    int licznik=0;
    inicjalizacja();
    double sinus,kosinus;
    double skala_x=al_get_bitmap_width(al_get_backbuffer(display))/(double)L;
    double skala_y=al_get_bitmap_height(al_get_backbuffer(display))/(double)L;
    double wskx,wsky;
    ALLEGRO_COLOR kolor;

    FILE *plik=fopen("wynik.txt","w");

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

                       M=0.0;
            E=0.0;
            for(int i=0;i<L;i++)
            {
                for(int j=0;j<L;j++)
                {
                    M+=sqrt(cos(siec[i][j])*cos(siec[i][j])+sin(siec[i][j])*sin(siec[i][j]));
                    E += J * cos(siec[i][j] - siec[(i + 1) % L][j]);
                    E += J * cos(siec[i][j] - siec[(i - 1 + L) % L][j]);
                    E += J * cos(siec[i][j] - siec[i][(j + 1) % L]);
                    E += J * cos(siec[i][j] - siec[i][(j - 1 + L) % L]);
                }
            }
            M/=(double)N;
            Mt[licznik]=M;
            E/=2*(double)N;
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
            fprintf(plik, "%.2f\t%.6f\t%.6f\n", T, Msr, Esr);

            al_set_target_bitmap(al_get_backbuffer(display));
            al_clear_to_color(al_map_rgb(0,0,0));

            for(int i=0; i<L; i++)
                krokmc(0.5,0.0);

            for(int i=0; i<L; i++)
            {
                for(int j=0; j<L; j++)
                {
                    kolor=al_map_rgb((cos(siec[i][j])+1.0)*0.5*255.0,0,255.0-(cos(siec[i][j])+1.0)*0.5*255.0);
                    al_draw_filled_rectangle(skala_x*i,skala_y*j,skala_x*(i+1),skala_y*(j+1),kolor);
                }
            }
            for(int i=0; i<L; i++)
            {
                for(int j=0; j<L; j++)
                {
                    sinus=sin(siec[i][j]);
                    kosinus=cos(siec[i][j]);
                    wskx=0.5*kosinus*skala_x;
                    wsky=0.5*sinus*skala_y;

                    al_draw_line(skala_x*(i+0.5),skala_y*(j+0.5),skala_x*(i+0.5)+wskx,skala_y*(j+0.5)+wsky,bialy,1.0);
                    al_draw_filled_circle(skala_x*(i+0.5)+wskx,skala_y*(j+0.5)+wsky,2.0,bialy);
                }
            }
            for(int i=0; i<L; i++)
            {
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
            }
            //deltaE=2.0*J*spiny[li][lj]*(spiny[(li+1)%L][lj]+spiny[(li-1+L)%L][lj]+spiny[li][(lj+1)%L]+spiny[li][(lj-1+L)%L])+2.0*h*spiny[li][lj];
            //E+=-0.5*J*spiny[i][j]*(spiny[(i+1)%L][j]+spiny[(i-1+L)%L][j]+spiny[i][(j+1)%L]+spiny[i][(j-1+L)%L])-h*spiny[i][j];
            al_flip_display();
        }
    }
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
