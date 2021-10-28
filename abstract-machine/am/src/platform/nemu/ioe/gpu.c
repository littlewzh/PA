#include <am.h>
#include <nemu.h>
#include <string.h>
#define SYNC_ADDR (VGACTL_ADDR + 4)
#define IMG 0xa1000000
int min(int a, int b){
	if(a <= b) return a;
	else return b;
}
//static uint32_t* fb =(uint32_t *)(uintptr_t)FB_ADDR;
static int W;
static int H;
void __am_gpu_init() {
  W=(int)inw(VGACTL_ADDR+2);
  H=(int)inw(VGACTL_ADDR);
  /*int i;
  //int w = W;  // TODO: get the correct width
  //int h = H;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < W* H; i ++) fb[i] = i;
  outl(SYNC_ADDR,0x00888888);*/
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = W, .height = H,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    /*int x=ctl->x,y=ctl->y,w=ctl->w,h=ctl->h;
    //uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
    uint32_t *pixels = ctl->pixels;
    uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
    //for (i = 0; i < W* H; i ++) fb[i] = i;
    for(int j = 0; j < h  && y + j < H; ++j){
      outl(SYNC_ADDR, *pixels);
      pixels+=w;*/
      int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
			uint32_t *pixels = ctl->pixels;
			//int cp_bytes = sizeof(uint32_t) * min(w, W - x);
			for(int j = 0; j < h  && y + j < H; ++j){{
        for(int i=0;i+x<W&&i<w;++i){
          outl(IMG+(y + j) * W + x+i ,*pixels);
          pixels ++;
    }
        }
      }
				
				
    
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
