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

typedef struct grzybiarz
{
    double x,y;
    double v0;
    double theta, beta;
}grzybiarz;

void aktualizujgrzybiarza(grzybiarz &g, double alpha, double sigma, double kappa, double dt)
{
    double eps=1e-8;
    double U1=losowa(0.0+eps,1.0+eps);
    double U2=losowa(0.0+eps,1.0+eps);
    double V=M_PI*(U1-0.5);
    double W=-log(U2);
    double X=sin(alpha*V)/pow(cos(V),1.0/alpha)*pow(cos((alpha-1)*V)/W,(1.0-alpha)/alpha);

    g.beta=atan2(g.y,g.x);
    g.theta=g.theta+kappa*sin(g.theta-g.beta)*dt+sigma/g.v0*X*pow(dt,1.0/alpha);
    g.x=g.x+g.v0*cos(g.theta)*dt;
    g.y=g.y+g.v0*sin(g.theta)*dt;
}

int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();

    ALLEGRO_DISPLAY *display=al_create_display(600,600);

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

    grzybiarz Zdzislaw;
    Zdzislaw.x=0.5;
    Zdzislaw.y=0.5;
    Zdzislaw.v0=1.0;
    Zdzislaw.theta=1.0;
    double dt=0.01;
    double kappa=1.0;//do obiektu
    double sigma=0.1;//zaklocenia ruchu
    double alpha=1.5;

    ALLEGRO_BITMAP *gbmp=al_load_bitmap("est.bmp");

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
            //al_clear_to_color(al_map_rgb(0,0,0));

            aktualizujgrzybiarza(Zdzislaw,alpha,sigma,kappa,dt);
            al_draw_filled_circle(300+50.0*Zdzislaw.x,300+50.0*Zdzislaw.y,2,bialy);
            //al_draw_bitmap(gbmp,400+50.0*Zdzislaw.x,400+50.0*Zdzislaw.y,0);
//al_map_rgba_f(1,1,1,0),
            al_flip_display();
        }
    }
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
