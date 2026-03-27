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
    double t, x, x0, dt=0.001, h=0.01, v=1.0;
    x0=0.5*h*imax;
    double sigma=0.2;
    double up05, um05;
    int iter=0;
    int gdzie=0;
    double maks;
    for(int i=0;i<imax;i++)
    {
        x=h*(double)i;
        u[i]=exp(-(x-x0)*(x-x0)/(2.0*sigma*sigma));
    }

    FILE *plik=fopen("metoda_Laxa.txt","w");
    //for(int i=0;i<imax;i++)
   // {
        //fprintf(plik,"%lf\t%lf\n",h*i,u[i]);
   // }
    fprintf(plik,"\n");
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

            //warunki brzegowe
            //pom[0]=0.5*(u[1]+u[imax-1])-v*dt/(2.0*h)*(u[1]-u[imax-1]);
            //pom[0]= u[imax-1]-v*dt/(2.0*h)*(u[1]-u[imax-1]);
            up05 = 0.5*(u[1]+u[0])-v*dt/(2.0*h)*(u[1]-u[0]);
            um05 = 0.5*(u[0]+u[imax-1])-v*dt/(2.0*h)*(u[0]-u[imax-1]);
            pom[0]=u[0]-v*dt/h*(up05-um05);


            for(int i=0;i<imax-1;i++)
            {
                //pom[i]=0.5*(u[i+1]+u[i-1])-v*dt/(2.0*h)*(u[i+1]-u[i-1]);
                //pom[i]=u[i-1]-v*dt/(2.0*h)*(u[i+1]-u[i-1]);
                up05 = 0.5*(u[i+1]+u[i])-v*dt/(2.0*h)*(u[i+1]-u[i]);
                um05 = 0.5*(u[i]+u[i-1])-v*dt/(2.0*h)*(u[i]-u[i-1]);
                pom[i]=u[i]-v*dt/h*(up05-um05);
            }
           //pom[imax-1]=0.5*(u[0]+u[imax-2])-v*dt/(2.0*h)*(u[0]-u[imax-2]); //warunki brzegowe
            //pom[imax-1]=u[imax-2]-v*dt/(2.0*h)*(u[0]-u[imax-2]);
            up05 = 0.5*(u[0]+u[imax-1])-v*dt/(2.0*h)*(u[0]-u[imax-1]);
            um05 = 0.5*(u[imax-1]+u[imax-2])-v*dt/(2.0*h)*(u[imax-1]-u[imax-2]);
            pom[imax-1]=u[imax-1]-v*dt/h*(up05-um05);

            for(int i=0;i<imax;i++)
            {
                u[i]=pom[i];
            }

            t=t+dt;
            iter++;
            maks=u[0];
            for(int i=0;i<imax;i++)
            {
                if(u[i]>maks)
                {
                    maks=u[i];
                    gdzie=i;
                }

            }
            if(iter%10==0)
                    fprintf(plik,"%lf\t%lf\n",t,maks); //h*gdzie,maks);
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
