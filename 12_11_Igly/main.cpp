#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;
double losowa(double a,double b)
{
    double u=(double)(rand()%RAND_MAX)/(double)RAND_MAX;
    return a+(b-a)*u;
}
int main()
{
    int N=10;
    double f[N],fpom[N];
    double dtheta=2.0*M_PI/(double)N;
    double rho;
    double calka;
    FILE *plik=fopen("wynik.txt","w");
    for(rho=1.0; rho<10.0; rho++)
    {
        for(int i=0; i<N; i++)
            f[i]=losowa(0.0,1.0);
        double fsuma=0.0;
        for(int i=0; i<N; i++)
            fsuma+=f[i]*dtheta;
        for(int i=0; i<N; i++)
            f[i]/=fsuma;

        int itermax=10;
        for(int iter=0; iter<itermax; iter++)
        {
            for(int i=0; i<N; i++)
            {
                double suma=0.0;
                calka=0.0;
                double thetai=(double)i/(double)N*2.0*M_PI;
                for(int j=0; j<N; j++)
                {
                    double thetaj=(double)j/(double)N*2.0*M_PI;
                    suma+=fabs(sin(thetai-thetaj))*f[j]*dtheta;
                    calka+=fabs(sin(thetai-thetaj))*f[j]*f[i]*dtheta*dtheta;
                }
                fpom[i]=exp(-rho*suma);
            }
            fsuma=0.0;
            for(int i=0; i<N; i++)
                fsuma+=fpom[i]*dtheta;
            for(int i=0; i<N; i++)
                fpom[i]/=fsuma;
            for(int i=0; i<N; i++)
                f[i]=fpom[i];
        }
        int indeksmax=0;
        double maximum=0.0;
        for(int i=0; i<N; i++)
        {
            if(f[i]>maximum)
            {
                maximum=f[i];
                indeksmax=i;
            }
        }
        double S=0.0;
        double BP=0.0;
        for(int i=0; i<N; i++)
        {
            S+=cos(2.0*(double)(i-indeksmax)/(double)N*2.0*M_PI)*f[i]*dtheta;
            BP=calka;
        }
        //BP=rho + 0.5 * rho*rho * BP;
        fprintf(plik,"%.2lf\n",BP);
        printf("%.2lf\n",BP);
        //fprintf(plik,"%.2lf\t%.10lf\n",rho,S);
    }
    fclose(plik);
    return 0;
}
