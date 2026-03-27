#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
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

    double **funfal;
    funfal=(double**)malloc(320*sizeof(double*));
    for(int i=0; i<320; i++)
        funfal[i]=(double*)malloc(180*sizeof(double));

    double **pom;
    pom=(double**)malloc(320*sizeof(double*));
    for(int i=0; i<320; i++)
        pom[i]=(double*)malloc(180*sizeof(double));

    double ax=-6.4, bx=6.4;
    double ay=-3.6, by=3.6;
    double u=1.0;
    double x,y;

    for(int i=0; i<320; i++)
        for(int j=0; j<180; j++)
        {
            x=ax+ i/320.0*(bx-ax);
            y=ay+(180.-j)/180.0*(by-ay);
            //funfal[i][j]=exp(-(x*x+y*y))*cos(2.0*M_PI*(u*x-sqrt(1.0-u*u)*y))+exp(-((x-3.0)*(x-3.0)+y*y))*cos(2.0*M_PI*(u*x-sqrt(1.0-u*u)*y));
            funfal[i][j]=exp(-(x*x+y*y)/1.0)*cos(0.0*M_PI*(u*x-y*sqrt(1.0-u*u)));
        }

    ALLEGRO_COLOR kolor;
    double maxwal, minwal;
    double nasyc;
    int er,gie,be;


    int indn,inds,indw,inde;
    double wagan,wagas,wagaw,wagae;
    double n,s,w,e;

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

            for(int i=0; i<320; i++)
            {

                for(int j=0; j<180; j++)
                {
                    if((i+j)%2==0)
                    {
                        wagan=1.0;
                        wagas=1.0;
                        wagaw=1.0;
                        wagae=1.0;
                        inde=i+1;
                        indw=i-1;
                        indn=j+1;
                        inds=j-1;
                        if(inde>318)
                        {
                            e=0.0;
                            wagaw=2.0;
                        }
                        else
                            e=funfal[i+1][j];
                        if(indw<1)
                        {
                            w=0.0;
                            wagae=2.0;
                        }
                        else
                            w=funfal[i-1][j];
                        if(indn>178)
                        {
                            n=0.0;
                            wagas=2.0;
                        }
                        else
                            n=funfal[i][j+1];
                        if(inds<1)
                        {
                            s=0.0;
                            wagan=2.0;
                        }
                        else
                            s=funfal[i][j-1];

                        pom[i][j]=0.5*(wagan*n+wagas*s+wagaw*w+wagae*e-2.0*funfal[i][j]);

                    }
                }
            }

            for(int i=0; i<320; i++)
            {

                for(int j=0; j<180; j++)
                {
                    if((i+j)%2==0)
                    {
                        funfal[i][j]=pom[i][j];
                    }
                }
            }

            for(int i=0; i<320; i++)
            {

                for(int j=0; j<180; j++)
                {
                    if((i+j)%2==1)
                    {
                        wagan=1.0;
                        wagas=1.0;
                        wagaw=1.0;
                        wagae=1.0;
                        inde=i+1;
                        indw=i-1;
                        indn=j+1;
                        inds=j-1;
                        if(inde>318)
                        {
                            e=0.0;
                            wagaw=2.0;
                        }
                        else
                            e=funfal[i+1][j];
                        if(indw<1)
                        {
                            w=0.0;
                            wagae=2.0;
                        }
                        else
                            w=funfal[i-1][j];
                        if(indn>178)
                        {
                            n=0.0;
                            wagas=2.0;
                        }
                        else
                            n=funfal[i][j+1];
                        if(inds<1)
                        {
                            s=0.0;
                            wagan=2.0;
                        }
                        else
                            s=funfal[i][j-1];

                        pom[i][j]=0.5*(wagan*n+wagas*s+wagaw*w+wagae*e-2.0*funfal[i][j]);
                    }
                }
            }

            for(int i=0; i<320; i++)
            {

                for(int j=0; j<180; j++)
                {
                    if((i+j)%2==1)
                    {
                        funfal[i][j]=pom[i][j];
                    }
                }
            }

            minwal=funfal[0][0];
            maxwal=funfal[0][0];
            for(int i=0; i<320; i++)
            {

                for(int j=0; j<180; j++)
                {
                    if(funfal[i][j]>maxwal)
                        maxwal=funfal[i][j];
                    if(funfal[i][j]<minwal)
                        minwal=funfal[i][j];
                }
            }
            for(int i=0; i<320; i++)
            {

                for(int j=0; j<180; j++)
                {
                    nasyc=255.0*(maxwal-funfal[i][j])/(maxwal-minwal);
                    if(nasyc<85.0)
                    {

                        er=255.0-3.0*nasyc;
                        gie=3.0*nasyc;
                        be=0.0;
                    }
                    else if(nasyc>171.0)
                    {
                        er=-519.107+3.03517*nasyc;
                        gie=0.0;
                        be=774.107-3.03517*nasyc;
                    }
                    else
                    {
                        er=0.0;
                        gie=507.035-2.96512*nasyc;
                        be=-252.035+2.96512*nasyc;
                    }

                    kolor=al_map_rgb(er,gie,be);
                    al_draw_filled_rectangle(4*i,4*j,4*i+4,4*j+4,kolor);
                }
            }
            al_flip_display();
        }
    }
    al_save_bitmap("koniec.bmp",al_get_backbuffer(display));
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
