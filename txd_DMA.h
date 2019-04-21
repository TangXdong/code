#ifndef TXD_DMA_H_INCLUDED
#define TXD_DMA_H_INCLUDED

#define LEN5 304038000
#define LEN4 1206500
#define LEN3 9500
#define LENF3 63500
//向同行从核发数据
#define LONG_PUTR(var,dest) asm volatile ("putr %0,%1\n"::"r"(var),"r"(dest):"memory")
//读列通信缓存
#define LONG_GETR(var) asm volatile ("getr %0\n":"=r"(var)::"memory")

//向同列从核送数
#define LONG_PUTC(var,dest) asm volatile ("putc %0,%1\n"::"r"(var),"r"(dest):"memory")
//读行通信缓存
#define LONG_GETC(var) asm volatile ("getc %0\n":"=r"(var)::"memory")

//行广播？
#define LONG_ROW(row) asm volatile ("rcsr  %0,1":"=r"(row))
//列广播？
#define LONG_COL(col) asm volatile ("rcsr  %0,2":"=r"(col))
//dma_desc da=0;
#define TXD_DMA_GET_BSET(da,reply)  \
{                                          \
	dma_set_stepsize(&da,0x128);          \
        dma_set_op(&da,DMA_GET); \
        dma_set_mode(&da,BCAST_MODE);             \
        dma_set_reply(&da,&reply);          \
}
#define TXD_DMA_GET_SET(da,reply)  \
{                                          \
        dma_set_op(&da,DMA_GET);            \
        dma_set_mode(&da,PE_MODE);             \
        dma_set_reply(&da,&reply);          \
}

#define TXD_GET_RUN(da,src,dest)            \
{                                          \
        dma(da,src,dest);                   \
 }

#define TXD_DMA_PUT_SET(da,reply)  \
{                                          \
        dma_set_op(&da,DMA_PUT);            \
        dma_set_mode(&da,PE_MODE);             \
        dma_set_reply(&da,&reply);          \
 }

 #define TXD_PUT_WAIT(reply,n)              \
 {                                         \
       dma_wait(&reply,n);                  \
 }

 #define TXD_GET_WAIT(reply,n)              \
 {                                         \
        dma_wait(&reply,n);                 \
  }

#define TXD_DMA_GET(da,src,dest,len)    \
{                                          \
        dma_set_size(&da,len);              \
        dma(da,(long)(src),(long)(dest));                  \
 }
//first GET don't need block;
 #define TXD_DMA_NEW(da,src,dest,len,reply,n) \
 {                                                 \
        dma_set_size(&da,len);                      \
        dma(da,(long)(src),(long)(dest));                          \
        dma_wait(&reply,n);                 \
    }

#define TXD_DMA_PUT(da,src,dest,len) \
{                                                \
        dma_set_size(&da,len);                    \
        dma(da,(long)(src),(long)(dest));                        \
}



#endif // TXD_DMA_H_INCLUDED
