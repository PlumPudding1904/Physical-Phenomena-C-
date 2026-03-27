#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
double losowa(double a, double b)
{
    double u=(double)(rand()%RAND_MAX)/(double)RAND_MAX;
    return a+(b-a)*u;
}
typedef struct odcinek
{
    double x1w, x2w, y1w, y2w;
    double x1l, x2l, y1l, y2l;
    double dx, dy, theta;
};
int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    ALLEGRO_DISPLAY *display=al_create_display(600,600);
    bool redraw=true;
    const float FPS=666;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    ALLEGRO_COLOR czerwony=al_map_rgb(255,0,0);
    ALLEGRO_COLOR niebieski=al_map_rgb(0,0,255);

    FILE *plik=fopen("wynik.txt","w");
    odcinek *odcinki;
    int nmax=128;
    odcinki=(odcinek *)malloc(nmax*sizeof(odcinek));

    for(int i=0; i<nmax; i++)
    {
        odcinki[i].x1w=0.0;
        odcinki[i].y1w=-0.5;
        odcinki[i].x2w=0.0;
        odcinki[i].y2w=0.5;
    }


    double L=8.0;
    double V=L*L;
    int n=0;
    for(int i=0; i<1; i++)
    {

        odcinki[i].theta=losowa(0.0,2.0*M_PI);
        odcinki[i].dx=losowa(0.0,L);
        odcinki[i].dy=losowa(0.0,L);
        odcinki[i].x1l=odcinki[i].x1w*cos(odcinki[i].theta)-odcinki[i].y1w*sin(odcinki[i].theta)+odcinki[i].dx;
        odcinki[i].y1l=odcinki[i].y1w*cos(odcinki[i].theta)+odcinki[i].x1w*sin(odcinki[i].theta)+odcinki[i].dy;
        odcinki[i].x2l=odcinki[i].x2w*cos(odcinki[i].theta)-odcinki[i].y2w*sin(odcinki[i].theta)+odcinki[i].dx;
        odcinki[i].y2l=odcinki[i].y2w*cos(odcinki[i].theta)+odcinki[i].x2w*sin(odcinki[i].theta)+odcinki[i].dy;
    }

    n=1;
    while(n<nmax)
    {
        int czy=0;


        odcinki[n].theta=losowa(0.0,2.0*M_PI);
        odcinki[n].dx=losowa(0.0,L);
        odcinki[n].dy=losowa(0.0,L);
        odcinki[n].x1l=odcinki[n].x1w*cos(odcinki[n].theta)-odcinki[n].y1w*sin(odcinki[n].theta)+odcinki[n].dx;
        odcinki[n].y1l=odcinki[n].y1w*cos(odcinki[n].theta)+odcinki[n].x1w*sin(odcinki[n].theta)+odcinki[n].dy;
        odcinki[n].x2l=odcinki[n].x2w*cos(odcinki[n].theta)-odcinki[n].y2w*sin(odcinki[n].theta)+odcinki[n].dx;
        odcinki[n].y2l=odcinki[n].y2w*cos(odcinki[n].theta)+odcinki[n].x2w*sin(odcinki[n].theta)+odcinki[n].dy;

        for(int i=0; i<n; i++)
        {
            double s=(odcinki[n].x1l*(odcinki[i].y1l-odcinki[i].y2l)
                      +odcinki[i].x1l*(odcinki[i].y2l-odcinki[n].y1l)
                      +odcinki[i].x2l*(odcinki[n].y1l-odcinki[i].y1l))
                     /((odcinki[n].x1l-odcinki[n].x2l)*(odcinki[i].y1l-odcinki[i].y2l)
                       -(odcinki[i].x1l-odcinki[i].x2l)*(odcinki[n].y1l-odcinki[n].y2l));
            double t=(odcinki[n].x1l*(odcinki[n].y2l-odcinki[i].y1l)
                      +odcinki[n].x2l*(odcinki[i].y1l-odcinki[n].y1l)
                      +odcinki[i].x1l*(odcinki[n].y1l-odcinki[n].y2l))
                     /((odcinki[i].x1l-odcinki[i].x2l)*(odcinki[n].y1l-odcinki[n].y2l)
                       -(odcinki[n].x1l-odcinki[n].x2l)*(odcinki[i].y1l-odcinki[i].y2l));

            if(s>=0.0&&s<=1.0&&t>=0.0&&t<=1.0) czy++;
            if(czy>0) break;
        }

        if(czy==0) n++;

    }
    double Qxx, Qxy, Qyx, Qyy;
    Qxx=0.0;
    Qxy=0.0;
    Qyx=0.0;
    Qyy=0.0;

    for(int i=0; i<nmax; i++)
    {
        Qxx+=1.0/n*((odcinki[i].x2l-odcinki[i].x1l)*(odcinki[i].x2l-odcinki[i].x1l)-0.5);
        Qxy+=1.0/n*((odcinki[i].x2l-odcinki[i].x1l)*(odcinki[i].y2l-odcinki[i].y1l));
        Qyx+=1.0/n*((odcinki[i].y2l-odcinki[i].y1l)*(odcinki[i].x2l-odcinki[i].x1l));
        Qyy+=1.0/n*((odcinki[i].y2l-odcinki[i].y1l)*(odcinki[i].y2l-odcinki[i].y1l)-0.5);
    }
    printf("Par. porzadku=%.10lf\n",sqrt(Qxx*Qxx+Qxy*Qxy));

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

            for(int iter=0; iter<nmax; iter++)
            {

                int losowaczasteczka=rand()%nmax;
                double deltax=losowa(-0.1,0.1);
                double deltay=losowa(-0.1,0.1);
                double deltatheta=losowa(-0.1,0.1);

                if(odcinki[losowaczasteczka].dx+deltax>=0.0 && odcinki[losowaczasteczka].dx+deltax<=L &&
                        odcinki[losowaczasteczka].dy+deltay>=0.0 && odcinki[losowaczasteczka].dy+deltay<=L)
                {

                    odcinki[losowaczasteczka].dx+=deltax;
                    odcinki[losowaczasteczka].dy+=deltay;
                    odcinki[losowaczasteczka].theta+=deltatheta;

                    odcinki[losowaczasteczka].x1l = odcinki[losowaczasteczka].x1w*cos(odcinki[losowaczasteczka].theta)
                                                    -odcinki[losowaczasteczka].y1w*sin(odcinki[losowaczasteczka].theta)+odcinki[losowaczasteczka].dx;
                    odcinki[losowaczasteczka].y1l = odcinki[losowaczasteczka].y1w*cos(odcinki[losowaczasteczka].theta)
                                                    -odcinki[losowaczasteczka].x1w*sin(odcinki[losowaczasteczka].theta)+odcinki[losowaczasteczka].dy;
                    odcinki[losowaczasteczka].x2l = odcinki[losowaczasteczka].x2w*cos(odcinki[losowaczasteczka].theta)
                                                    -odcinki[losowaczasteczka].y2w*sin(odcinki[losowaczasteczka].theta)+odcinki[losowaczasteczka].dx;
                    odcinki[losowaczasteczka].y2l = odcinki[losowaczasteczka].y2w*cos(odcinki[losowaczasteczka].theta)
                                                    -odcinki[losowaczasteczka].x2w*sin(odcinki[losowaczasteczka].theta)+odcinki[losowaczasteczka].dy;
                    int czy=0;
                    for(int i=0; i<n; i++)
                    {
                        double s=(odcinki[losowaczasteczka].x1l*(odcinki[i].y1l-odcinki[i].y2l)
                                  +odcinki[i].x1l*(odcinki[i].y2l-odcinki[losowaczasteczka].y1l)
                                  +odcinki[i].x2l*(odcinki[losowaczasteczka].y1l-odcinki[i].y1l))
                                 /((odcinki[losowaczasteczka].x1l-odcinki[losowaczasteczka].x2l)*(odcinki[i].y1l-odcinki[i].y2l)
                                   -(odcinki[i].x1l-odcinki[i].x2l)*(odcinki[losowaczasteczka].y1l-odcinki[losowaczasteczka].y2l));
                        double t=(odcinki[losowaczasteczka].x1l*(odcinki[losowaczasteczka].y2l-odcinki[i].y1l)
                                  +odcinki[losowaczasteczka].x2l*(odcinki[i].y1l-odcinki[losowaczasteczka].y1l)
                                  +odcinki[i].x1l*(odcinki[losowaczasteczka].y1l-odcinki[losowaczasteczka].y2l))
                                 /((odcinki[i].x1l-odcinki[i].x2l)*(odcinki[losowaczasteczka].y1l-odcinki[losowaczasteczka].y2l)
                                   -(odcinki[losowaczasteczka].x1l-odcinki[losowaczasteczka].x2l)*(odcinki[i].y1l-odcinki[i].y2l));
                        if(s>=0.0 && s<=1.0 && t>=0 && t<=1.0)czy++;
                        if(czy>0)break;
                    }
                    if(czy>0)
                    {

                        odcinki[losowaczasteczka].dx-=deltax;
                        odcinki[losowaczasteczka].dy-=deltay;
                        odcinki[losowaczasteczka].theta-=deltatheta;

                        odcinki[losowaczasteczka].x1l = odcinki[losowaczasteczka].x1w*cos(odcinki[losowaczasteczka].theta)
                                                        -odcinki[losowaczasteczka].y1w*sin(odcinki[losowaczasteczka].theta)+odcinki[losowaczasteczka].dx;
                        odcinki[losowaczasteczka].y1l = odcinki[losowaczasteczka].y1w*cos(odcinki[losowaczasteczka].theta)
                                                        -odcinki[losowaczasteczka].x1w*sin(odcinki[losowaczasteczka].theta)+odcinki[losowaczasteczka].dy;
                        odcinki[losowaczasteczka].x2l = odcinki[losowaczasteczka].x2w*cos(odcinki[losowaczasteczka].theta)
                                                        -odcinki[losowaczasteczka].y2w*sin(odcinki[losowaczasteczka].theta)+odcinki[losowaczasteczka].dx;
                        odcinki[losowaczasteczka].y2l = odcinki[losowaczasteczka].y2w*cos(odcinki[losowaczasteczka].theta)
                                                        -odcinki[losowaczasteczka].x2w*sin(odcinki[losowaczasteczka].theta)+odcinki[losowaczasteczka].dy;
                    }
                }
            }
            Qxx=0.0;
            Qxy=0.0;
            Qyx=0.0;
            Qyy=0.0;
            for(int i=0; i<nmax; i++)
            {
                Qxx+=1.0/n*((odcinki[i].x2l-odcinki[i].x1l)*(odcinki[i].x2l-odcinki[i].x1l)-0.5);
                Qxy+=1.0/n*((odcinki[i].x2l-odcinki[i].x1l)*(odcinki[i].y2l-odcinki[i].y1l));
                Qxx+=1.0/n*((odcinki[i].y2l-odcinki[i].y1l)*(odcinki[i].x2l-odcinki[i].x1l));
                Qyy+=1.0/n*((odcinki[i].y2l-odcinki[i].y1l)*(odcinki[i].y2l-odcinki[i].y1l)-0.5);
            }
            printf("Par porzadku=%.10lf\n",sqrt(Qxx*Qxx+Qxy*Qxy));
            //printf("Ilosc=%d\n",n);
            //fprintf(plik,"%.2lf\t%.10lf\n",n/(L*L),sqrt(Qxx*Qxx+Qxy*Qxy));

            for(int i=0; i<n; i++)
            {
                al_draw_line(odcinki[i].x1l*100.0,odcinki[i].y1l*100.0
                             ,odcinki[i].x2l*100.0, odcinki[i].y2l*100.0,
                             al_map_rgb(255,255,255),2);
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

//double x1=losowa(0.0,8.0), y1=losowa(0.0,8.0);
//double x2=losowa(0.0,8.0), y2=losowa(0.0,8.0);
//double x3=losowa(0.0,8.0), y3=losowa(0.0,8.0);
//double x4=losowa(0.0,8.0), y4=losowa(0.0,8.0);

/*

            if(s>=0.0&&s<=1.0&&t>=0.0&&t<=1.0)
            {

                al_draw_line(x1*100.0,y1*100.0,x2*100.0,y2*100.0,al_map_rgb(255,0,0),2);
                al_draw_line(x3*100.0,y3*100.0,x4*100.0,y4*100.0,al_map_rgb(255,0,0),2);
            }
            else
            {
                al_draw_line(x1*100.0,y1*100.0,x2*100.0,y2*100.0,al_map_rgb(255,255,255),2);
                al_draw_line(x3*100.0,y3*100.0,x4*100.0,y4*100.0,al_map_rgb(255,255,255),2);
            }*/


