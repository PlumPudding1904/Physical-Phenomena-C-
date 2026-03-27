#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
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
    double *u, *pom;
    u=new double[imax];
    pom=new double[imax];
    double t, x, x0, h=0.01, a=1.0;
    double  dt=h*h/(2.0*a)*0.99;
    //double dt=0.001;
    x0=0.5*h*(double)imax;
    double v=10.0;
    double sigma=0.2;
    double maks;
    double gdzie=0;
    int iter=0;
    for(int i=0;i<imax;i++)
    {
        x=h*(double)i;
        u[i]=exp(-(x-x0)*(x-x0)/(2.0*sigma*sigma));
    }

    FILE *plik=fopen("dyfuzja.txt","w");
    for(int i=0;i<imax;i++)
    {
        //fprintf(plik,"%lf\t%lf\n",h*i,u[i]);
    }
    //fprintf(plik,"\n");
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
            //for(int k=0;k<100;k++)
            //{
            //warunki brzegowe
            pom[0]=u[0]+a*dt/(h*h)*(u[1]-2.0*u[0]+u[imax-1]);
            //pom[0]= - u[0]+a*dt/(h*h)*(u[1]-2.0*u[0]+u[imax-1]) + 0.5*(u[1]+u[imax-1])-v*dt/(2.0*h)*(u[1]-u[imax-1]);
            //pom[0]= u[0]+(a*dt/(h*h)*(u[1]-2.0*u[0]+u[imax-1])-v*dt/(h)*(u[1]-u[0]))*dt;

            for(int i=0;i<imax-1;i++)
            {
                pom[i]=u[i]+a*dt/(h*h)*(u[i+1]-2.0*u[i]+u[i-1]);
                //pom[i]= u[i]+a*dt/(h*h)*(u[i+1]-2.0*u[i]+u[i-1]) + 0.5*(u[i+1]+u[i-1])-v*dt/(2.0*h)*(u[i+1]-u[i-1]);
                //pom[i]= u[i]+(a*dt/(h*h)*(u[i+1]-2.0*u[i]+u[i-1])-v*dt/(h)*(u[i+1]-u[i]))*dt;
            }
            pom[imax-1]=u[imax-1]+a*dt/(h*h)*(u[0]-2.0*u[imax-1]+u[imax-2]); //warunki brzegowe
            //pom[imax-1]= - u[imax-1]+a*dt/(h*h)*(u[0]-2.0*u[imax-1]+u[imax-2]) + 0.5*(u[0]+u[imax-2])-v*dt/(2.0*h)*(u[0]-u[imax-2]);
            //pom[imax-1]= u[imax-1]+(a*dt/(h*h)*(u[0]-2.0*u[imax-1]+u[imax-2])-v*dt/(h)*(u[0]-u[imax-2]))*dt;

            for(int i=0;i<imax;i++)
            {
                u[i]=pom[i];
            }
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
            //}
            for(int i=0;i<imax-1;i++)
            {
                al_draw_line(i,600-600.0*u[i],i+1,600-600.0*u[i+1],al_map_rgb(250,10,40),4);
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
