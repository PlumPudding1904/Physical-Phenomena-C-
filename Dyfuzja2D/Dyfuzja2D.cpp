#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    ALLEGRO_DISPLAY *display=al_create_display(600, 600);
    bool redraw=true;
    const float FPS=60;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    double t=0.0;
    double D=1.0;
    double dt=0.001;
    double h=0.1;
    int imax=80, jmax=80;
    double **u=new double *[imax];
    for(int i=0;i<imax;i++)
        u[i]=new double[jmax];

    double **pom=new double *[imax];
    for(int i=0;i<imax;i++)
        pom[i]=new double[jmax];

    for(int i=0;i<imax;i++)
    {
        for(int j=0;j<jmax;j++)
            u[i][j]=0.0;
        u[imax/2][jmax/2]=1.0/(h*h);
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

            t=t+dt;


            for(int i=1;i<imax-1;i++)
            {
                for(int j=1;j<jmax-1;j++)
                {
                    //pom[i][j]=u[i][j]+D*dt/(h*h)*(u[i+1][j]+u[i-1][j]+u[i][j+1]+u[i][j-1]-4.0*u[i][j]);
                    pom[i][j]=u[i][j]+(D*dt/(h*h)*(u[i+1][j]+u[i-1][j]+u[i][j+1]+u[i][j-1]-4.0*u[i][j])-D*dt/h *(u[i+1][j]-u[i-1][j]+u[i][j+1]-u[i][j-1]));
                }
            }
            //pom[i]= u[i]+(a*dt/(h*h)*(u[i+1]-2.0*u[i]+u[i-1])-v*dt/(h)*(u[i+1]-u[i]))*dt;
            //warunek brzegowy
            for(int i=1;i<imax-1;i++)
                //pom[i][0]=u[i][0]+D*dt/(h*h)*(u[i+1][0]+u[i-1][0]+u[i][1]+u[i][jmax-1]-4.0*u[i][0]);
                pom[i][0]=u[i][0]+(D*dt/(h*h)*(u[i+1][0]+u[i-1][0]+u[i][1]+u[i][jmax-1]-4.0*u[i][0])-D*dt/h *(u[i+1][0]-u[i-1][0]+u[i][1]-u[i][jmax-1]));
            for(int i=1;i<imax-1;i++)
                //pom[i][jmax-1]=u[i][jmax-1]+D*dt/(h*h)*(u[i+1][jmax-1]+u[i-1][jmax-1]+u[i][0]+u[i][jmax-2]-4.0*u[i][jmax-1]);
                pom[i][jmax-1]=u[i][jmax-1]+(D*dt/(h*h)*(u[i+1][jmax-1]+u[i-1][jmax-1]+u[i][0]+u[i][jmax-2]-4.0*u[i][jmax-1])-D*dt/h *(u[i+1][jmax-1]-u[i-1][jmax-1]+u[i][0]-u[i][jmax-2]));
            for(int j=1;j<jmax-1;j++)
                //pom[0][j]=u[0][j]+D*dt/(h*h)*(u[1][j]+u[imax-1][j]+u[0][j+1]+u[0][j-1]-4.0*u[0][j]);
                pom[0][j]=u[0][j]+(D*dt/(h*h)*(u[1][j]+u[imax-1][j]+u[0][j+1]+u[0][j-1]-4.0*u[0][j])-D*dt/h *(u[1][j]-u[imax-1][j]+u[0][j+1]-u[0][j-1]));
            for(int j=1;j<jmax-1;j++)
                //pom[imax-1][j]=u[imax-1][j]+D*dt/(h*h)*(u[0][j]+u[imax-2][j]+u[imax-1][j+1]+u[imax-1][j-1]-4.0*u[imax-1][j]);
                pom[imax-1][j]=u[imax-1][j]+(D*dt/(h*h)*(u[0][j]+u[imax-2][j]+u[imax-1][j+1]+u[imax-1][j-1]-4.0*u[imax-1][j])-D*dt/h *(u[0][j]-u[imax-2][j]+u[imax-1][j+1]-u[imax-1][j-1]));

            //pom[0][0]=u[0][0]+D*dt/(h*h)*(u[1][0]+u[imax-1][0]+u[0][1]+u[0][jmax-1]-4.0*u[0][0]);
            //pom[0][jmax-1]=u[0][jmax-1]+D*dt/(h*h)*(u[1][jmax-1]+u[imax-1][jmax-1]+u[0][0]+u[0][jmax-2]-4.0*u[0][jmax-1]);
            //pom[imax-1][0]=u[imax-1][0]+D*dt/(h*h)*(u[0][0]+u[imax-2][0]+u[imax-1][1]+u[imax-1][jmax-1]-4.0*u[imax-1][0]);
            //pom[imax-1][jmax-1]=u[imax-1][jmax-1]+D*dt/(h*h)*(u[0][jmax-1]+u[imax-2][jmax-1]+u[imax-1][0]+u[imax-1][jmax-2]-4.0*u[imax-1][jmax-1]);

            pom[0][0]=u[0][0]+(D*dt/(h*h)*(u[1][0]+u[imax-1][0]+u[0][1]+u[0][jmax-1]-4.0*u[0][0])-D*dt/h *(u[1][0]-u[imax-1][0]+u[0][1]-u[0][jmax-1]));
            pom[0][jmax-1]=u[0][jmax-1]+(D*dt/(h*h)*(u[1][jmax-1]+u[imax-1][jmax-1]+u[0][0]+u[0][jmax-2]-4.0*u[0][jmax-1])-D*dt/h *(u[1][jmax-1]-u[imax-1][jmax-1]+u[0][0]-u[0][jmax-2]));
            pom[imax-1][0]=u[imax-1][0]+(D*dt/(h*h)*(u[0][0]+u[imax-2][0]+u[imax-1][1]+u[imax-1][jmax-1]-4.0*u[imax-1][0])-D*dt/h *(u[0][jmax-1]-u[imax-1][0]+u[imax-1][1]-u[imax-1][jmax-1]));
            pom[imax-1][jmax-1]=u[imax-1][jmax-1]+(D*dt/(h*h)*(u[0][jmax-1]+u[imax-2][jmax-1]+u[imax-1][0]+u[imax-1][jmax-2]-4.0*u[imax-1][jmax-1])-D*dt/h *(u[0][jmax-1]-u[imax-2][jmax-1]+u[imax-1][0]-u[imax-1][jmax-2]));


            for(int i=1;i<imax-1;i++)
            {
                for(int j=1;j<jmax-1;j++)
                    u[i][j]=pom[i][j];
            }

            double maks=u[0][0];
            for(int i=0;i<imax;i++)
            {
                for(int j=0;j<jmax;j++)
                {
                    if(u[i][j]>maks)
                        maks=u[i][j];
                }
            }
            for(int i=0;i<imax;i++)
            {
                for(int j=0;j<jmax;j++)
                {
                    al_draw_filled_rectangle(i*600.0/imax,j*600.0/jmax,(i+1)*600.0/imax,(j+1)*600.0/jmax,al_map_rgb(255.0*u[i][j]/maks,0,255-255.0*u[i][j]/maks));
                }
            }
            al_flip_display();
        }
    }
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
