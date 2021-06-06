#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int main(int argc, char** argv)
{
    
    double ki, km, kn, kf, ps;
    int T; //no. of segments
    const double MSS = 1;
    const double RWS = 1024;
    double thresh; //threshold

    double CW;
    int N;
    

    FILE *fp;

    
    ki = atof(argv[2]);
    km = atof(argv[4]);
    kn = atof(argv[6]);
    kf = atof(argv[8]);
    ps = atof(argv[10]);
    T = atof(argv[12]);
    fp = fopen(argv[14],"w");

    int u=0; //line count

    fprintf(fp,"0 %f\n",CW);

    CW = ki * MSS * 1.0;
    thresh = T/2;

    int sent=0; //sent
    int segment_count=0; //segm. count
    
    while(sent<T)
    {
        N = (int)ceil(CW/MSS);
        segment_count=0;
        //srand ( time(NULL) );
        while(segment_count<N)
        {
            double R = (1.0*(rand()%RAND_MAX))/RAND_MAX;

            if(R<ps)
            {
                segment_count=0;
                thresh = CW/2;
                CW = MAX(1, kf*CW);
            }
            else
            {
                if (CW < thresh) {
                    CW = MIN(CW + km*MSS, RWS);
                    sent++;
                }
                else {
                    CW = MIN(CW + kn*MSS*MSS/CW, RWS);
                    sent++;
                }
                segment_count++;
            }
            
            fprintf(fp,"%d %f\n",u+1,CW);
            u++;
        }

        //sent+=N;
        

    }
    fclose(fp);
    return 0;
}
