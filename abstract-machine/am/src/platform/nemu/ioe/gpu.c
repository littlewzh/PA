#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
static int W;
static int H;
void __am_gpu_init() {
  W=(int)inw(VGACTL_ADDR+2);
  H=(int)inw(VGACTL_ADDR);
  int i;
  //int w = W;  // TODO: get the correct width
  //int h = H;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < W* H; i ++) fb[i] = i;
  outl(SYNC_ADDR,0x00888888);
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
    int x=ctl->x,y=ctl->y,w=ctl->w,h=ctl->h;
    //uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
    uint32_t *pixels = ctl->pixels;
    for(int j = 0; j < h  && y + j < H; ++j){
      outl(FB_ADDR+(y + j) * W + x, *pixels);
      pixels+=w;
    }
    
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
