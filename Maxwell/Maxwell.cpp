#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    ALLEGRO_DISPLAY *display=al_create_display(800, 600);
    bool redraw=true;
    const float FPS=60;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    int imax=800;
    double *u, *u_pop, *u_wcz;
    u = new double[imax];
    u_pop = new double[imax];
    u_wcz = new double[imax];
    double *pr;
    pr = new double[imax];
    int iter=0;
    int gdzie;


    double t=0.0, dt=0.001, h=0.01, v=1.0, x, x0;
    x0=0.5*h*imax;
    double sigma=0.2;
    double maks;
    FILE *plik=fopen("u.txt","w");

    for(int i=0;i<imax;i++)//du(x,t)/dt dla t=0
    {
        x=h*i;
        pr[i]=2.0*cos(M_PI*x);
    }

    for(int i=0;i<imax;i++)
    {
        x=h*i;
        u_wcz[i]=0.5*exp(-(x-x0)*(x-x0)/(2.0*sigma*sigma));//u(x,0) dla t=0
        u_pop[i]=u_wcz[i]+pr[i]*dt;
    }

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

            u[0]=v*v*dt*dt/(h*h)*(u_pop[1]-2.0*u_pop[0]+u_pop[imax-1])+2.0*u_pop[0]-u_wcz[0];

            for(int i=1;i<imax-1;i++)
                u[i]=v*v*dt*dt/(h*h)*(u_pop[i+1]-2.0*u_pop[i]+u_pop[i-1])+2.0*u_pop[i]-u_wcz[i];

            u[imax-1]=v*v*dt*dt/(h*h)*(u_pop[0]-2.0*u_pop[imax-1]+u_pop[imax-2])+2.0*u_pop[imax-1]-u_wcz[imax-1];

            for(int i=0;i<imax;i++)
                u_wcz[i]=u_pop[i];
            for(int i=0;i<imax;i++)
                u_pop[i]=u[i];
            maks=u[0];
            for(int i=0;i<imax;i++)
            {
                if(maks<u[i])
                    {maks=u[i];
                    gdzie=i;}
            }
            t=t+dt;
            iter++;
            if(iter%10==0)
                fprintf(plik,"%lf\t%lf\n",t,gdzie*h);
            for(int i=0;i<imax-1;i++)
            {


                al_draw_line(i,300.0-300.0*u[i],i+1,300.0-300.0*u[i+1],al_map_rgb(255*u[i],0,255-255*u[i]),4);
            }

            al_flip_display();
        }
    }
    fclose(plik);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
