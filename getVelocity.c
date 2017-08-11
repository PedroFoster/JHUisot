#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "turblib.h"
#include<omp.h>

int main(int argc,char** argv){
  int i,j,w,w1,N=1024;
  int attempts = 0;
  char *authtoken = "br.ufrj.if.foster-2377d412";
  char *dataset = "isotropic1024coarse";
//  float time0 = 0.60F;
    float time0;
//  float time0;
  float myclock;
  enum SpatialInterpolation spatialInterp = Lag6;
  enum TemporalInterpolation temporalInterp = NoTInt;
  float (*position)[3] = malloc(32*N*N*sizeof(*position));
  float (*velocity)[3] = malloc(32*N*N*sizeof(*velocity));

  FILE *resume = NULL;
  resume = fopen("./data/log_cubointeiro.txt","w");
  
  FILE *fdadosx = NULL;
  FILE *fdadosy = NULL;
  FILE *fdadosz = NULL;
  FILE *relatorio = NULL;
  fdadosx=fopen("./data/vx_long_isot_t25to200_.txt","w");
  fdadosy=fopen("./data/vy_transv_isot_t25to200_.txt","w");
//  fdadosz=fopen("./data/vz_long_isot_t60_N1024_cubointeiro_900to1023.txt","w");

  /* Start clock */
  myclock=omp_get_wtime();
  printf("Data is being downloaded...\n");

    /* Initialize gSOAP */
    soapinit();

    /* Enable exit on error.  See README for details. */
//    turblibSetExitOnError(1);
      turblibSetExitOnError(0);


 for(time0=0.25F; time0 < 2.1F; time0=time0+0.25F){ 

//   for(w=0;w<N;w+=1){
//    for(i=0;i<N;i+=1){
//     for(j=0;j<N;j+=1){
//  	  position[j][0] = (float)(j*2)*M_PI/((float)N);
//  	  position[j][1] = (float)(i*2)*M_PI/((float)N);
//  	  position[j][2] = (float)(w*2)*M_PI/((float)N);
//    }


   for(w1=0;w1<32;w1+=1){
    for(w=0;w<32;w+1){
      for(i=0;i<N;i+=1){
       for(j=0;j<N;j+=1){
  	    position[w*N*N+i*N+j][0] = (float)(j*2)*M_PI/((float)N);
  	    position[w*N*N+i*N+j][1] = (float)(i*2)*M_PI/((float)N);
  	    position[w*N*N+i*N+j][2] = (float)((w1*32+w)*2)*M_PI/((float)N);
      }
     }
    }    


    //getVelocity (authtoken, dataset, time0, spatialInterp, temporalInterp, N, position, velocity);

//    turblibSetExitOnError(0);
    while (getVelocity (authtoken, dataset, time0, spatialInterp, temporalInterp, N, position, velocity) != SOAP_OK) {
      if (attempts++ > 7000) {
        printf("Fatal Error: too many failures\n");
        exit(1);
        } 
      else {
        printf("Temporary Error: %s\n", turblibGetErrorString());
	printf("After error %d, returning to download...\n",attempts);
      }
      sleep(60);
    }

    for (j = 0; j < 32*N*N; j++) {
      fprintf(fdadosx,"%f\n", velocity[j][0]);
      fprintf(fdadosy,"%f\n", velocity[j][1]);
//      fprintf(fdadosz,"%f\n", velocity[j][2]);
    }

    relatorio = fopen("./data/relatorio.txt","w");
    fprintf(relatorio,"Block %d has been downloaded\n", w1);
    fclose(relatorio);	
    }
//    }
//    fprintf(fdados,"\n");
}
    /* Free gSOAP resources */
    soapdestroy();

  /* Stop clock */
  printf("Time elapsed: %f hours\n",myclock-omp_get_wtime()/3600);

  fclose(fdadosx);
  fclose(fdadosy);
  fclose(resume);
  return 0;
}
