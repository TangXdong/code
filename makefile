TARGET = LbmCavity3D
USER = $(shell whoami)
CC = sw5cc 
LD = mpicc 

CFLAGS = -O3  -host -ver 5.421-sw-gy433 -LNO:simd=1:simd_report=2 -I/usr/sw-mpp/mpi2/include/ -lm -OPT:alias=disjoint:IEEE_arithmetic=3:Ofast:ro=1:div_split=OFF  #-OPT:IEEE_arith=1

OBJ = LbmCavity3D.o  Parallel.o  Stream_slave.o Stream.o #Collide.o Collide_slave.o

LIB = lib/liblbm.a

#-ver 5.421-sw-gy436 -O3 LNO:simd=1:simd_repot=2

(TARGET): $(OBJ) 
	$(LD) $(OBJ) $(LIB) -msimd -OPT:alias=disjoint:IEEE_arithmetic=3:Ofast:ro=1:div_split=OFF  -hybrid  -o $(TARGET) 
	rm $(OBJ)

LbmCavity.o:LbmCavity.c
	$(CC)  $(CFLAGS)  -c LbmCavity.c
	
Parallel.o:Parallel.c
	$(CC)  $(CFLAGS)  -c Parallel.c
#Collide.o:Collide.c
#	$(CC)  $(CFLAGS)  -c Collide.c
#Collide_slave.o:Collide_slave.c
#	$(CC) -ver 5.421-sw-gy436 -O3 LNO:simd=1:simd_report=2  -msimd -slave -I/usr/sw-mpp/mpi2/include/ -lm   -c Collide_slave.c
Stream.o:Stream.c
	$(CC)  -O3 -host -I/usr/sw-mpp/mpi2/include/ -lm   -c Stream.c
Stream_slave.o:Stream_slave.c
	$(CC) -ver 5.421-sw-gy433  -O3 LNO:simd=1:simd_report=2  -msimd  -slave -I/usr/sw-mpp/mpi2/include/ -lm   -c Stream_slave.c

run:
	bsub -I -b -q q_sw_cpc_2 -cgsp 64 -n 16  -np 4  -host_stack 512  -share_size 6500  -J test ./LbmCavity3D $(USER)

#-------------------------------------*
.PHONY : clean clear
clean:
	-rm -rf $(TARGET) $(OBJ) 

