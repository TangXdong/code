#include "Argument.h"
#include "elem.h"
#include <athread.h>
/*------------------------------------
 *      Main Computing Part
 *-----------------------------------*/
extern  SLAVE_FUN(fd_stream)(Swlbm *);
void stream(Real *****nodes,
		    int ****walls,
			int ***flags,
			int Xst,
			int Xed,
			int Yst,
			int Yed,
			int nz,
		    int current,
			int other)
{

	//int i,j,p,g,inv;

//	Real *****Nodes_s;
	
	//Nodes_s = array5DF(1, 252 , 127 , 500, 19);
	int i;
	Swlbm swlbm;
				     
       /*       for(i = Xst; i < Xed; i++) {
			X = i - Xst + 1;

		   for(j = Yst; j < Yed; j++) {
			Y = j -Yst + 1;
					//调整数组nodes[other]顺序
					for(g = 0; g < nz; g++) {
					for(p = 0; p < 19; p++) {
					inv = dfInv[p];
				Nodes_s[0][X][Y][g][p] = nodes[other][X+e_x[inv]][Y+e_y[inv]][g+e_z[inv]][p];/(Nodes + (other*252*127*500*19+ (X+e_x[inv])*127*500*19+(Y+e_y[inv])*500*19+ (g+e_z[inv])*19+p)); 
							}
						}
			}
		}*/
	//swlbm.Nodes = &nodes[0][0][0][0][0];
	swlbm.Nodes_c = &nodes[current][0][0][0][0];
	swlbm.Nodes_o = &nodes[other][0][0][0][0];
	swlbm.Walls = &walls[0][0][0][0];
	swlbm.Flags = &flags[0][0][0];
	swlbm.Xst = Xst;
	swlbm.Xed = Xed;
	swlbm.Yst = Yst;
	swlbm.Yed = Yed;
	swlbm.Z = nz;
	swlbm.current = current;
	swlbm.other =  other;
	//printf("athread start,%d\n",Xst);
	//athread_set_num_threads(64);
	athread_spawn64(SLAVE_FUN(fd_stream),&swlbm);
	//i = athread_get_num_threads();	
	//printf("--------------------%d\n",i);
	athread_join64();

	//arrayFree5DF(Nodes_s);
}
