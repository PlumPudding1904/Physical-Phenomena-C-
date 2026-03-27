#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
double losowa(double a, double b)
{
    double u=(double)(rand()%RAND_MAX)/(double)RAND_MAX;
    return a+(b-a)*u;
}
typedef struct fibrynogen
{
    double x[23], y[23];
    double r[23];
    double dx, dy, theta;
    double x_rt[23], y_rt[23];
} fibrynogen;

void osadz(fibrynogen *fb,int gdzie, fibrynogen &fb2)
{
    for(int i=0; i<23; i++)
    {
        fb[gdzie].x[i]=fb2.x[i];
        fb[gdzie].y[i]=fb2.y[i];
        fb[gdzie].r[i]=fb2.r[i];
        fb[gdzie].x_rt[i]=fb2.x_rt[i];
        fb[gdzie].y_rt[i]=fb2.y_rt[i];
    }
    fb[gdzie].dx=fb2.dx;
    fb[gdzie].dy=fb2.dy;
    fb[gdzie].theta=fb2.theta;
}

bool czysieprzekrywaja(fibrynogen *fb, int ilezaadsorbowanych, fibrynogen fb2)
{
    double dx, dy,dr;
    for(int k=0; k<ilezaadsorbowanych; k++)
        for(int j=0; j<23; j++)
            for(int i=0; i<23; i++)
            {
                dx=fb[k].x_rt[j]-fb2.x_rt[i];
                dy=fb[k].y_rt[j]-fb2.y_rt[i];
                dr=sqrt(dx*dx+dy*dy);
                if(dr<=fb[k].r[j]+fb2.r[i])
                    return true;
            }
            return false;
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
    const float FPS=666;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue,
                             al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    ALLEGRO_COLOR czerwony=al_map_rgb(255,0,0);
    ALLEGRO_COLOR niebieski=al_map_rgb(0,0,255);

    fibrynogen fb1, fb2;

    fibrynogen *fb=(fibrynogen *)malloc(10000*sizeof(fibrynogen));

    fb1.r[0]=6.7/2.0;
    for(int i=1; i<11; i++) fb1.r[i]=1.5/2.0;
    fb1.r[11]=5.3/2.0;
    for(int i=12; i<22; i++) fb1.r[i]=1.5/2.0;
    fb1.r[22]=6.7/2.0;
    for(int i=0; i<23; i++) fb1.y[i]=0.0;
    fb1.x[0]=-21.0;
    for(int i=0; i<10; i++) fb1.x[i+1]=-5.3/2.0-1.5/2.0-1.5*i;
    fb1.x[11]=0.0;
    for(int i=0; i<10; i++) fb1.x[i+12]=5.3/2.0+1.5/2.0+1.5*i;
    fb1.x[22]=21.0;

    for(int i=0; i<23; i++)
    {
        fb1.x_rt[i]=fb1.x[i];
        fb1.y_rt[i]=fb1.y[i];
    }
    osadz(fb,0,fb1);
    int ileosadzono=1;

    double L=2000.0;
    double poleSubstratu=L*L;
    double poleFibrynogenu=127.918;
    int czas=0;
    double pokrycie;
    FILE *plik=fopen("wyniki.txt","w");
    srand(time(NULL));
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
            for(int k=0; k<100; k++)
            {
                for(int i=0; i<23; i++)
                {
                    fb2.x[i]=fb1.x[i];
                    fb2.y[i]=fb1.y[i];
                    fb2.r[i]=fb1.r[i];
                    fb2.x_rt[i]=fb1.x_rt[i];
                    fb2.y_rt[i]=fb1.y_rt[i];
                }

                fb2.dx=losowa(-L/2.0,L/2.0);
                fb2.dy=losowa(-L/2.0,L/2.0);
                fb2.theta=losowa(0.0,2.0*M_PI);
                double sintheta=sin(fb2.theta);
                double costheta=cos(fb2.theta);

                for(int i=0; i<23; i++)
                {
                    fb2.x_rt[i]=costheta*fb2.x[i]-sintheta*fb2.y[i];
                    fb2.y_rt[i]=costheta*fb2.y[i]+sintheta*fb2.x[i];
                }
                for(int i=0; i<23; i++)
                {
                    fb2.x_rt[i]+=fb2.dx;
                    fb2.y_rt[i]+=fb2.dy;
                }
                if((czysieprzekrywaja(fb,ileosadzono,fb2)==false)&&(ileosadzono<10000))
                {
                    osadz(fb,ileosadzono,fb2);
                    ileosadzono++;
                }
                czas++;
                }
                pokrycie=ileosadzono*poleFibrynogenu/poleSubstratu;
                fprintf(plik,"%d\t%d\t%.15lf\n",czas,ileosadzono,pokrycie);
                printf("%d\t%d\t%.15lf\n",czas,ileosadzono,pokrycie);
                for(int j=0; j<ileosadzono; j++)
                {
                    for(int i=0; i<23; i++)
                        al_draw_filled_circle(400+1.0*fb[j].x_rt[i],400+1.0*fb[j].y_rt[i],1.0*fb[j].r[i],al_map_rgb(255,255,255));
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


