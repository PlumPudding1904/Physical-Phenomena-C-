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
    double *E, *B, *E_pom, *B_pom;
    E = new double[imax];
    B = new double[imax];
    E_pom = new double[imax];
    B_pom = new double[imax];

    double t=0.0, dt=0.001, h=0.001, v=1.0, x, x0;
    x0=0.5*h*imax;
    double sigma=0.02;
    FILE *plik=fopen("B.txt","w");
    FILE *plik1=fopen("E.txt","w");
    int iter=0;
    double maksE, maksB;
    int gdzieE, gdzieB;

    for(int i=0; i<imax; i++) //du(x,t)/dt dla t=0
    {
        x=i*h;
        //E[i]=0.5*cos(4.0*M_PI*x)*exp(-(x-x0)*(x-x0)/(2.0*sigma*sigma));
        //B[i]=0.5*sin(4.0*M_PI*x)*exp(-(x-x0)*(x-x0)/(2.0*sigma*sigma));
        E[i]=0.5*cos(40.0*M_PI*x);
        B[i]=0.5*sin(40.0*M_PI*x);
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

            E_pom[0]=0.5*(E[1]+E[imax-1])-0.5*v*dt/h*(B[1]-B[imax-1]);
            B_pom[0]=0.5*(B[1]+B[imax-1])-0.5*v*dt/h*(E[1]-E[imax-1]);
            for(int i=1; i<imax-1; i++)
                E_pom[i]=0.5*(E[i+1]+E[i-1])-0.5*v*dt/h*(B[i+1]-B[i-1]);
            for(int i=1; i<imax-1; i++)
                B_pom[i]=0.5*(B[i+1]+B[i-1])-0.5*v*dt/h*(E[i+1]-E[i-1]);

            E_pom[imax-1]=0.5*(E[0]+E[imax-2])-0.5*v*dt/h*(B[0]-B[imax-2]);
            B_pom[imax-1]=0.5*(B[0]+B[imax-2])-0.5*v*dt/h*(E[0]-E[imax-2]);

            for(int i=0; i<imax; i++)
                E[i]=E_pom[i];
            for(int i=0; i<imax; i++)
                B[i]=B_pom[i];

            maksE=E[0];
            for(int i=0;i<imax;i++)
            {
                if(maksE<E[i])
                    {maksE=E[i];
                    gdzieE=i;}
            }
            maksB=B[0];
            for(int i=0;i<imax;i++)
            {
                if(maksB<B[i])
                    {maksB=B[i];
                    gdzieB=i;}
            }
            t=t+dt;
            iter++;
            if(iter%10==0)
                    fprintf(plik,"%.5lf\t%.15lf\n",t,maksB);
            if(iter%10==0)
                    fprintf(plik1,"%.5lf\t%.15lf\n",t,maksE);

            for(int i=0; i<imax-1; i++)
                al_draw_line(i,300.0-300.0*E[i],i+1,300.0-300.0*E[i+1],al_map_rgb(0,0,128+127*E[i]),2);
            for(int i=0; i<imax-1; i++)
                al_draw_line(i,300.0-300.0*B[i],i+1,300.0-300.0*B[i+1],al_map_rgb(128+128*B[i],0,0),2);

            al_flip_display();
        }
    }
    fclose(plik);
    fclose(plik1);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}

