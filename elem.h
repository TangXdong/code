#ifndef __elem_h__
#define __elem_h__

typedef struct
{
	
	int myrank;
	
	int Xst;
        int Xed;
        int Yst;
        int Yed;
        int Z;
        int current;
	int other;
	
	int *Walls;
        //Real *Nodes;
	Real *Nodes_c;
	Real *Nodes_o;
        int *Flags;
}Swlbm;


#endif
