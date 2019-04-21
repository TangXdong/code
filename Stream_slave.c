#include <slave.h>
//#include <stdlib.h>
#include <dma.h>
#include <simd.h>
#include "Argument.h"
#include "elem.h"
#include "txd_DMA.h"
#include <math.h>
/*------------------------------------
 *      Main Computing Part
 *-----------------------------------*/


__thread_local volatile unsigned  int  get_reply,put_reply;
__thread_local int MYID;
//__thread_local  Real slave_n_1[250*19];
//__thread_local  int slave_w[250*19];
//__thread_local  int slave_f[500]; 
//__thread_local  float slave_n_2[250*19];//n_1拷入，n_2拷出
void fd_stream(Swlbm *swlbm)
{
	floatv4 v1; //__attribute__((aligned(16)));
	//floatv4 v2;// __attribute__((aligned(16)));
//	float *fv __attribute__((aligned(16)));
//	float *gv __attribute__((aligned(16)));
	int i, j, k, l,g,p;
	
	//float *Nodes;
	float *Nodes_o;
	float *Nodes_c;
	int *Flags;
	int *Walls;
	int Xst;
	int Xed; 
	int Yst;
	int Yed;
	int nz; 
 	int other; 
	int current;
        int  X,Y;
	float slave_n_1[250*19];// __attribute__((aligned(16)));
	float slave_n_2[4750];//  __attribute__((aligned(16)));
	//int slave_w[4750];
	int slave_f[500];
	int ex[19],ey[19],ez[19];
	 int dfI[19];
	int fluid,bounce;
	int flag=0;
        int inv,inv1,inv2,inv3,inv4;
 	int s=0;
	float rho, u_x, u_y, u_z,tmp;
	float fi,CSmagos,omegas,nus;
	float feq[19] ;// __attribute__((aligned(16)));
	float  Qo, omegaNew;
	float ws[19]  ;//__attribute__((aligned(16)));
	float Sij[3][3],S;
	float e[3][19] ;//__attribute__((aligned(16)));
	int x1, y1, k1;
	int addr,exx,eyy,ezz;
	dma_desc dg=0;
	dma_desc dp=0;

	Xst = swlbm->Xst;
	Xed = swlbm->Xed; 
	Yst = swlbm->Yst;
	Yed = swlbm->Yed;
	nz =swlbm->Z; 
	other =  swlbm->other;
	current = swlbm->current;
	//Nodes_s = swlbm->Nodes_s;
       // Nodes  =  swlbm->Nodes;
        Nodes_c  =  swlbm->Nodes_c;
        Nodes_o  =  swlbm->Nodes_o;
	Flags  = swlbm->Flags;
	Walls  = swlbm->Walls;
	MYID = athread_get_id(-1);
	fluid = FLUID;
	bounce = BOUNCE;
	CSmagos = CSmago;
	omegas = omega;
	
	get_reply = 0;
	athread_get(PE_MODE,e_x,ex,76,&get_reply,0,0,0);
	athread_get(PE_MODE,e_y,ey,76,&get_reply,0,0,0);
	athread_get(PE_MODE,e_z,ez,76,&get_reply,0,0,0);
	athread_get(PE_MODE,dfInv,dfI,76,&get_reply,0,0,0);
	athread_get(PE_MODE,w,ws,76,&get_reply,0,0,0);	
	while(get_reply!=5);

	TXD_DMA_GET_SET(dg,get_reply);
	TXD_DMA_PUT_SET(dp,put_reply);


              for(i = Xst; i < Xed; i++) {
			X = i - Xst + 1;

		   for(j = Yst; j < Yed; j++) {
			Y = j -Yst + 1;
                        if( (Y-1)%64 == MYID) 
			{      
				
				 get_reply = 0;
		//333		athread_get(PE_MODE,(Flags+X*127*500+Y*500),slave_f,500*4,&get_reply,0,0,0);
				TXD_DMA_NEW(dg,&Flags[X*LENF3+Y*500],slave_f,2000,get_reply,1);
				TXD_DMA_NEW(dg , &Nodes_c[X*LEN4+Y*LEN3],slave_n_2,19000,get_reply,2);//&Nodes_s[0][0]
		///333			while(get_reply!=2);//{printf("k=0,get..  blocked\n");};
			
				for(l=0;l<19;l++)
	    	  {//LL
				exx = (X+ex[dfI[l]])*LEN4+(Y+ey[dfI[l]])*LEN3;
				get_reply=0;
			//  {	//slave_n_1[k*19+l]= Nodes_o[ l+(k+ez[dfI[l]])*19+(Y+ey[dfI[l]])*LEN3+(X+ex[dfI[l]])*LEN4 ];
				TXD_DMA_NEW(dg , &Nodes_o[exx +ez[dfI[l]]*19+l],&slave_n_1[0],250*19*4,get_reply,1);//k  stepsize=19
				
				//{ 	   //(ss<9500)
				//simd_loadu(v1,&(Nodes_o[l+(k+ez[dfI[l]])*19+(Y+ey[dfI[l]])*LEN3+(X+ex[dfI[l]])*LEN4]));
			//	v1=simd_set_floatv4(Nodes_o[l+(k+ezz)*19+exx ],Nodes_o[l+(k+1+ezz)*19+exx],Nodes_o[l+(k+2+ezz)*19+exx],Nodes_o[l+(k+3+ezz)*19+exx]);
			//	simd_storeu(v1,&slave_n_1[k*19+l]);
			//}
		//  }//LL	
	
				//dma_wait(&get_reply1,9500);	         
				for(k = 0; k < 250; k++) 
			  	{// if(k<250){
					if( slave_f[k] == fluid  ) {
					
					//  for(l = 0; l < 16; l+=4) {
						//222 inv = dfI[l];
				        //v1=simd_set_floatv4(slave_n_1[k*19+l],slave_n_1[k*19+l+1],slave_n_1[k*19+l+2],slave_n_1[k*19+l+3]);
					//simd_storeu(v1,&(slave_n_2[k*19+l]));
				//	}
					  //for(l = 0; l < 19; l++) {
					slave_n_2[k*19+l] = slave_n_1[k*19];
					//			}
								  }	
					if( slave_f[k] == bounce)  {
					//for(l = 0; l < 19; l++) {
					  inv = dfI[l];
						if( Walls[(X-1)*125*500*19+(Y-1)*19*500+k*19+l ]) {
  		slave_n_2[k*19+l] =  Nodes_o[X*LEN4+Y*LEN3+k*19+inv];
						} else {
			slave_n_2[k*19+l] = slave_n_1[k*19];
						}
							//	}//for
							          }
				//0-249
				}
	}//LL
			for(k = 0; k < 250; k++) {
				if(slave_f[k] == fluid || slave_f[k] == bounce) {
					rho = 0.0, u_x = 0.0, u_y = 0.0, u_z = 0.0;
					for (l = 0; l < 19; l++) {
						fi = slave_n_2[k*19+l];
						rho += fi;
						u_x += ex[l] * fi;
						u_y += ey[l] * fi;
						u_z += ez[l] * fi;
					}
					
					u_x /= rho;
                    			u_y /= rho;
                    			u_z /= rho;

					//11111111
					for (l = 0; l < 19; l++) {
						tmp = (ex[l] * u_x + ey[l] * u_y + ez[l] * u_z);
						feq[l] = ws[l] * rho * (1.0 -
							(1.5 * (u_x * u_x + u_y * u_y + u_z * u_z)) +
							(3.0 *     tmp) +
							(4.5 * tmp * tmp));
					}
							//(3.0/2.0 * (u_x * u_x + u_y * u_y + u_z * u_z)) +
							//(3.0 *     tmp) +
							//(9.0/2.0 * tmp * tmp));
				
					Qo=0;
									
	
					for(k1 = 0; k1 < 19; k1++) {
						e[0][k1] = ex[k1];
						
						e[1][k1] = ey[k1];
						e[2][k1] = ez[k1];
					}

					for(x1 = 0; x1 < 3; x1++) {
						for(y1 = 0; y1 < 3; y1++) {
							Sij[x1][y1] = 0;
							for(k1 = 0; k1 < 19; k1++) {
						 		//3333	
								Sij[x1][y1] += e[x1][k1] * e[y1][k1] * (slave_n_2[k*19+k1] - feq[k1]);
							}
							Qo += Sij[x1][y1] * Sij[x1][y1];
						}
					}
					
					nus = (2.0 / omegas - 1.0) / 6.0;
					S = (-nus + sqrt(nus * nus + 18 * CSmagos * CSmagos * sqrt(Qo))) / 6.0 / CSmagos / CSmagos;
					omegaNew = 1.0 / (3.0 * (nus + CSmagos * CSmagos * S) + 0.5);
						
						for (l = 0; l < 19; l++) {
						slave_n_2[k*19+l] =
							(1.0 - omegaNew) * slave_n_2[k*19+l] +
							omegaNew * feq[l];
					}
				} //endif
			}//end k
	  		      
			     		  put_reply=0;
			 		TXD_DMA_NEW( dp , &Nodes_c[X*LEN4+Y*LEN3] ,slave_n_2 , 19000,put_reply,1);
			   
		 	//	 if(k==250)
		 	 //     {//250----499-------------------------------------
					get_reply=0;
				//222  athread_get(PE_MODE,Nodes + (other*252*127*500*19 +X*127*500*19 +Y*500*19 +250*19),slave_n_1,250*19*sizeof(float),&get_reply,0,0,0);
			///333	athread_get(PE_MODE,Nodes + current*252*127*500*19+X*127*500*19+Y*500*19+250*19  ,slave_n_2,250*19*sizeof(float),&get_reply,0,0,0);//&Nodes_s[250][0]
			///333	athread_get(PE_MODE,Walls+ ((X-1)*125*500*19+ (Y-1)*500*19 +250*19) ,&slave_w[0],250*19*4,&get_reply,0,0,0);//walls[998][498][500][19]
				TXD_DMA_NEW(dg,&Nodes_c[X*LEN4+Y*LEN3+ 4750],slave_n_2,19000,get_reply,1);//&Nodes_s[250][0]
			///333		while(get_reply!=2);//{printf("get k =250-499 blocked!\n");};
		
		//	for(k=250;k<500;k++)//bulianxu2
				for(l=0;l<19;l++)
	    	  {//LL
				exx = (X+ex[dfI[l]])*LEN4+(Y+ey[dfI[l]])*LEN3;
				get_reply=0;
			//  {	//slave_n_1[k*19+l]= Nodes_o[ l+(k+ez[dfI[l]])*19+(Y+ey[dfI[l]])*LEN3+(X+ex[dfI[l]])*LEN4 ];
				TXD_DMA_NEW(dg , &Nodes_o[exx +(250+ez[dfI[l]])*19+l],&slave_n_1[0],250*19*4,get_reply,1);//k  stepsize=19
	       	  //    {	
		//		s=k-250;
		//		for(l=0;l<19;l++)
		//	{
		//		///inv = dfI[l];
		//	    	slave_n_1[s*19+l]= *(Nodes + l+(k+ez[dfI[l]])*19+(Y+ey[dfI[l]])*LEN3+(X+ex[dfI[l]])*LEN4+other*LEN5 );
		//	}
		  //    }	 		         
			   
				for(k = 250; k < 500; k++)
			 {
	 				s = k-250;//代替k进行访问
					if( slave_f[k] == fluid ) 
					{
					//for(l = 0; l < 19; l++) {
					//	inv = dfI[l];
				slave_n_2[s*19+l] = slave_n_1[s*19];}
					//		}	
					if( slave_f[k] == bounce) {
					//for(l = 0; l < 19; l++) {
						inv = dfI[l];
						if(Walls[(X-1)*125*500*19+(Y-1)*500*19+k*19+l]) {
  		slave_n_2[s*19+l] =  Nodes_o[X*LEN4+Y*LEN3+k*19+inv];
						} else {
					slave_n_2[s*19+l] = slave_n_1[s*19];
						}
					    // }
								 }
			}//k : 250-499
}//LL
			for(k = 250; k < 500; k++) {
				s=k-250;
				if(slave_f[k] == fluid || slave_f[k] == bounce) {
					rho = 0.0, u_x = 0.0, u_y = 0.0, u_z = 0.0;
					for (l = 0; l < 19; l++) {
						fi = slave_n_2[s*19+l];
						rho += fi;
						u_x += ex[l] * fi;
						u_y += ey[l] * fi;
						u_z += ez[l] * fi;
					}
					
					u_x /= rho;
                    			u_y /= rho;
                    			u_z /= rho;

					//11111111
					for (l = 0; l < 19; l++) {
						tmp = (ex[l] * u_x + ey[l] * u_y + ez[l] * u_z);
						feq[l] = ws[l] * rho * (1.0 -
							(1.5 * (u_x * u_x + u_y * u_y + u_z * u_z)) +
							(3.0 *     tmp) +
							(4.5 * tmp * tmp));
					}
				
					Qo=0;
									
	
					for(k1 = 0; k1 < 19; k1++) {
						e[0][k1] = ex[k1];
						
						e[1][k1] = ey[k1];
						e[2][k1] = ez[k1];
					}

					for(x1 = 0; x1 < 3; x1++) {
						for(y1 = 0; y1 < 3; y1++) {
							Sij[x1][y1] = 0;
							for(k1 = 0; k1 < 19; k1++) {
						 		//3333	
								Sij[x1][y1] += e[x1][k1] * e[y1][k1] * (slave_n_2[s*19+k1] - feq[k1]);
							}
							Qo += Sij[x1][y1] * Sij[x1][y1];
						}
					}
					
					nus = (2.0 / omegas - 1.0) / 6.0;
					S = (-nus + sqrt(nus * nus + 18 * CSmagos * CSmagos * sqrt(Qo))) / 6.0 / CSmagos / CSmagos;
					omegaNew = 1.0 / (3.0 * (nus + CSmagos * CSmagos * S) + 0.5);
						
						for (l = 0; l < 19; l++) {
						slave_n_2[s*19+l] =
							(1.0 - omegaNew) * slave_n_2[s*19+l] +
							omegaNew * feq[l];
					}
				} //endif
			}//end k
			  		    put_reply=0;
	                      ///333	athread_put( PE_MODE,slave_n_2, (Nodes+( current*252*127*500*19 +X*127*500*19+Y*500*19+ 250*19)) ,250*19*sizeof(float),&put_reply,0,0);
	                      	TXD_DMA_NEW( dp , &Nodes_c[X*LEN4+Y*LEN3+ 4750],slave_n_2,19000,put_reply,1);
		
		   } //if Y 分块
		}//for j
	}//for i:
 //  if(MYID==1){ printf("athread------------------end--------------%d--\n!",MYID);}
}//fd_stream
