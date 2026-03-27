#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
double fx(double x,double y,double vx,double vy,double t)
{
    return vx;
}
double fy(double x,double y,double vx,double vy,double t)
{
    return vy;
}
double gx(double x,double y,double vx,double vy,double t)
{
    double r=sqrt(x*x+y*y);
    return -x/(r*r*r);
}
double gy(double x,double y,double vx,double vy,double t)
{
    double r=sqrt(x*x+y*y);
    return -y/(r*r*r);
}

void rysujwykres(ALLEGRO_BITMAP *bitmapa,double *x,double *y,int n,ALLEGRO_FONT *czcionka)
{
    al_set_target_bitmap(bitmapa);
    double rx=al_get_bitmap_width(bitmapa);
    double ry=al_get_bitmap_height(bitmapa);
    double b=0.1;
    al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_line(b*rx,b*ry,rx-b*rx,b*ry,al_map_rgb(0,0,0),2);
    al_draw_line(b*rx,ry-b*ry,rx-b*rx,ry-b*ry,al_map_rgb(0,0,0),2);
    al_draw_line(b*rx,b*ry,b*rx,ry-b*ry,al_map_rgb(0,0,0),2);
    al_draw_line(rx-b*rx,b*ry,rx-b*rx,ry-b*ry,al_map_rgb(0,0,0),2);
    al_draw_text(czcionka, al_map_rgb(0,0,0), rx*0.5, ry-0.5*b*ry, ALLEGRO_ALIGN_CENTRE,"t");
    al_draw_text(czcionka, al_map_rgb(0,0,0), rx*0.5*b, ry*0.5, ALLEGRO_ALIGN_CENTRE,"error");
    al_draw_text(czcionka, al_map_rgb(0,0,0), rx*0.5, ry*0.25*b, ALLEGRO_ALIGN_CENTRE,"Error(t)");
    double xmin=x[0],xmax=x[0],ymin=y[0],ymax=y[0];
    for(int i=1; i<n; i++)
    {
        if(x[i]<xmin)xmin=x[i];
        if(x[i]>xmax)xmax=x[i];
        if(y[i]<ymin)ymin=y[i];
        if(y[i]>ymax)ymax=y[i];
    }
    double xekran,yekran,xekran2, yekran2;
    for(int i=0;i<n-1;i++)
    {
        xekran=b*rx+(x[i]-xmin)/(xmax-xmin)*(rx-2.0*b*rx);
        yekran=ry-(b*ry+(y[i]-ymin)/(ymax-ymin)*(ry-2.0*b*ry));
        xekran2=b*rx+(x[i+1]-xmin)/(xmax-xmin)*(rx-2.0*b*rx);
        yekran2=ry-(b*ry+(y[i+1]-ymin)/(ymax-ymin)*(ry-2.0*b*ry));
        al_draw_line(xekran,yekran,xekran2,yekran2,al_map_rgb(255,127,127),1);
    }
    for(int i=0; i<n; i++)
    {
        xekran=b*rx+(x[i]-xmin)/(xmax-xmin)*(rx-2.0*b*rx);
        yekran=ry-(b*ry+(y[i]-ymin)/(ymax-ymin)*(ry-2.0*b*ry));
        al_draw_filled_circle(xekran,yekran,3,al_map_rgb(255,0,0));
    }
    al_draw_textf(czcionka, al_map_rgb(0,0,0), b*rx, ry-0.5*b*ry, ALLEGRO_ALIGN_CENTRE,"%.2f",xmin);
    al_draw_textf(czcionka, al_map_rgb(0,0,0), rx-b*rx, ry-0.5*b*ry,
                  ALLEGRO_ALIGN_CENTRE,"%.2f",xmax);
    al_draw_textf(czcionka, al_map_rgb(0,0,0), b*rx*0.5, ry-b*ry, ALLEGRO_ALIGN_CENTRE,"%.2f",pow(10.0,ymin));
    al_draw_textf(czcionka, al_map_rgb(0,0,0), b*rx*0.5, b*ry, ALLEGRO_ALIGN_CENTRE,"%.2f",pow(10.0,ymax));
}

int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_DISPLAY *display=al_create_display(800, 600);
    ALLEGRO_DISPLAY *display2=al_create_display(800,600);
    ALLEGRO_FONT *czcionka=al_load_ttf_font("Cakecafe.ttf",32,0);
    ALLEGRO_BITMAP *obraz=al_create_bitmap(800,600);
    bool redraw=true;
    const float FPS=600;
    ALLEGRO_TIMER *timer=al_create_timer(1.0/FPS);
    al_start_timer(timer);
    ALLEGRO_EVENT_QUEUE *event_queue=al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    double t=0.0,dt=0.001;
    double kx[4],ky[4],lx[4],ly[4];
    double x,y,vx,vy;
    double x0=0.0,y0=1.0,vx0=1.0,vy0=0.0;
    x=x0;
    y=y0;
    vx=vx0;
    vy=vy0;

    double blad;
    int licznik=0;
    int coile=100;
    int itermax=10000;
    const int n=itermax/coile;
    double wykresx[n];
    double wykresy[n];

    for(int i=0; i<n; i++)
    {
        wykresx[i]=0.0;
        wykresy[i]=0.0;
    }
    FILE *plik=fopen("wynik.txt","w");
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
            kx[0]=fx(x,y,vx,vy,t);
            ky[0]=fy(x,y,vx,vy,t);
            lx[0]=gx(x,y,vx,vy,t);
            ly[0]=gy(x,y,vx,vy,t);
            kx[1]=fx(x+0.5*dt*kx[0],y+0.5*dt*ky[0],vx+0.5*dt*lx[0],vy+0.5*dt*ly[0],t+0.5*dt);
            ky[1]=fy(x+0.5*dt*kx[0],y+0.5*dt*ky[0],vx+0.5*dt*lx[0],vy+0.5*dt*ly[0],t+0.5*dt);
            lx[1]=gx(x+0.5*dt*kx[0],y+0.5*dt*ky[0],vx+0.5*dt*lx[0],vy+0.5*dt*ly[0],t+0.5*dt);
            ly[1]=gy(x+0.5*dt*kx[0],y+0.5*dt*ky[0],vx+0.5*dt*lx[0],vy+0.5*dt*ly[0],t+0.5*dt);
            kx[2]=fx(x+0.5*dt*kx[1],y+0.5*dt*ky[1],vx+0.5*dt*lx[1],vy+0.5*dt*ly[1],t+0.5*dt);
            ky[2]=fy(x+0.5*dt*kx[1],y+0.5*dt*ky[1],vx+0.5*dt*lx[1],vy+0.5*dt*ly[1],t+0.5*dt);
            lx[2]=gx(x+0.5*dt*kx[1],y+0.5*dt*ky[1],vx+0.5*dt*lx[1],vy+0.5*dt*ly[1],t+0.5*dt);
            ly[2]=gy(x+0.5*dt*kx[1],y+0.5*dt*ky[1],vx+0.5*dt*lx[1],vy+0.5*dt*ly[1],t+0.5*dt);
            kx[3]=fx(x+dt*kx[2],y+dt*ky[2],vx+dt*lx[2],vy+dt*ly[2],t+dt);
            ky[3]=fy(x+dt*kx[2],y+dt*ky[2],vx+dt*lx[2],vy+dt*ly[2],t+dt);
            lx[3]=gx(x+dt*kx[2],y+dt*ky[2],vx+dt*lx[2],vy+dt*ly[2],t+dt);
            ly[3]=gy(x+dt*kx[2],y+dt*ky[2],vx+dt*lx[2],vy+dt*ly[2],t+dt);

            //RK4
            /*
            x=x+dt/6.0*(kx[0]+2.0*kx[1]+2.0*kx[2]+kx[3]);
            y=y+dt/6.0*(ky[0]+2.0*ky[1]+2.0*ky[2]+ky[3]);
            vx=vx+dt/6.0*(lx[0]+2.0*lx[1]+2.0*lx[2]+lx[3]);
            vy=vy+dt/6.0*(ly[0]+2.0*ly[1]+2.0*ly[2]+ly[3]);
            t=t+dt;*/

            x=x+dt*kx[0];
            y=y+dt*ky[0];
            vx=vx+dt*lx[0];
            vy=vy+dt*ly[0];
            t=t+dt;

            al_set_target_bitmap(al_get_backbuffer(display));
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_filled_circle(400,300,100,al_map_rgb(255,255,255));
            al_draw_filled_circle(400+x*100,300+y*100,5,al_map_rgb(255,0,0));
            al_flip_display();

            if(licznik%coile==0)
            {
                blad=fabs(1.0-sqrt(x*x+y*y));
                wykresx[licznik/coile]=t;
                wykresy[licznik/coile]=log10(blad);
                fprintf(plik,"%.5lf\t%.15lf\n",t,blad);
                fprintf(stdout,"%.5lf\t%.15lf\n",t,blad);
            }
            licznik++;
            if(licznik>itermax)
                break;
            rysujwykres(obraz,wykresx,wykresy,licznik/coile,czcionka);
            al_set_target_bitmap(al_get_backbuffer(display2));
            al_clear_to_color(al_map_rgb(255,255,255));
            al_draw_bitmap(obraz,0,0,0);
            al_flip_display();
        }
    }
    al_save_bitmap("bladEuler.bmp",obraz);
    al_save_bitmap("koniecsymulacji.bmp",al_get_backbuffer(display));
    fclose(plik);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}
