#include <fxcg/keyboard.h>
#include <fxcg/display.h>
#include <fxcg/serial.h>
#include <fxcg/rtc.h>
#include <fxcg/system.h>
#include <stdio.h>
#include <stdlib.h>
#define KEY_F1 0x070A
#define KEY_F2 0x060A
#define KEY_F3 0x050A
#define KEY_F4 0x040A
#define KEY_F5 0x030A
#define KEY_F6 0x020A
#define KEY_SHIFT 0x0709
#define KEY_OPTN  0x0609
#define KEY_VARS  0x0509
#define KEY_MENU  0x0409
#define KEY_UP 0x0209
#define KEY_LEFT 0x0309
#define KEY_RIGHT 0x0208
#define KEY_DOWN 0x0308
#define KEY_ALPHA 0x0708 	
#define KEY_SQUARE 0x0608 	
#define KEY_CARET 0x0508 	
#define KEY_EXIT 0x0408
#define KEY_X_THETA_T 0x0707
#define KEY_LOG 0x0607
#define KEY_LN 0x0507
#define KEY_SIN 0x0407
#define KEY_COS 0x0307
#define KEY_TAN 0x0207
#define KEY_MIXFRAC 0x0706
#define KEY_FTOD 0x0606
#define KEY_LPARENS 0x0506
#define KEY_RPARENS 0x0406
#define KEY_COMMA 0x0306
#define KEY_RARROW 0x0206
#define KEY_SEVEN 0x0705
#define KEY_EIGHT 0x0605
#define KEY_NINE 0x0505
#define KEY_DEL 0x0405
#define KEY_AC_ON 0x0101
#define KEY_FOUR 0x0704
#define KEY_FIVE 0x0604
#define KEY_SIX 0x0504
#define KEY_TIMES 0x0404
#define KEY_DIVIDE 0x0304
#define KEY_ONE 0x0703
#define KEY_TWO 0x0603
#define KEY_THREE 0x0503
#define KEY_PLUS 0x0403
#define KEY_MINUS 0x0303
#define KEY_ZERO 0x0702
#define KEY_PERIOD 0x0602
#define KEY_EXP 0x0502
#define KEY_SIGN 0x0402
#define KEY_EXE 0x0302

#define MAX_TICKS 128 * 30


#define PLL_24x 0b010111 // 87 MHz
#define PLL_18x 0b010001 // 65.25 MHz
#define PLL_16x 0b001111 // 58 MHz (NORMAL SPEED)

/* void change_freq(int mult) { */
/*    __asm__( */
/*        "mov r4, r0\n\t" */
/*        "and #0x3F, r0\n\t"  */
/*        "shll16 r0\n\t"  */
/*        "shll8 r0\n\t"  */
/*        "mov.l frqcr, r1\n\t"   */
/*        "mov.l pll_mask, r3\n\t"   */
/*        "mov.l @r1, r2\n\t"   */
/*        "and r3, r2\n\t"   */
/*        "or r0, r2\n\t"  */
/*        "mov.l r2, @r1\n\t"  */
/*        "mov.l frqcr_kick_bit, r0\n\t"  */
/*        "mov.l @r1, r2\n\t" */
/*        "or r0, r2\n\t" */
/*        "rts\n\t" */
/*        "mov.l r2, @r1\n\t" */
/*        ".align 4\n\t" */
/*        "frqcr_kick_bit: .long 0x80000000\n\t" */
/*        "pll_mask: .long 0xC0FFFFFF\n\t"  */
/*        "frqcr: .long 0xA4150000\n\t" */
/*    ); */
/* } */

/* #define LCD_GRAM    0x202 */
/* #define LCD_BASE    0xB4000000 */
/* #define SYNCO() __asm__ volatile("SYNCO\n\t":::"memory"); */
/* // Module Stop Register 0 */
/* #define MSTPCR0     (volatile unsigned*)0xA4150030 */
/* // DMA0 operation register */
/* #define DMA0_DMAOR  (volatile unsigned short*)0xFE008060 */
/* #define DMA0_SAR_0  (volatile unsigned*)0xFE008020 */
/* #define DMA0_DAR_0  (volatile unsigned*)0xFE008024 */
/* #define DMA0_TCR_0  (volatile unsigned*)0xFE008028 */
/* #define DMA0_CHCR_0 (volatile unsigned*)0xFE00802C */
/* void DmaWaitNext(void){ */
/*     if ((*DMA0_CHCR_0)&2 || (*DMA0_DMAOR)&4) { */
/*       return 1; */
/*     } else { */
      
/*       return 0; */
/*     } */
/* } */
/* void DoDMAlcdNonblockStrip(unsigned y1,unsigned y2){ */
/*     Bdisp_WriteDDRegister3_bit7(1); */
/*     Bdisp_DefineDMARange(6,389,y1,y2); */
/*     Bdisp_DDRegisterSelect(LCD_GRAM); */
/*     *MSTPCR0&=~(1<<21);//Clear bit 21 */
/*     *DMA0_CHCR_0&=~1;//Disable DMA on channel 0 */
/*     *DMA0_DMAOR=0;//Disable all DMA */
/*     *DMA0_SAR_0=(GetVRAMAddress()+(y1*384*2))&0x1FFFFFFF;//Source address is VRAM */
/*     *DMA0_DAR_0=LCD_BASE&0x1FFFFFFF;//Destination is LCD */
/*     *DMA0_TCR_0=((y2-y1+1)*384)/16;//Transfer count bytes/32 */
/*     *DMA0_CHCR_0=0x00101400; */
/*     *DMA0_DMAOR|=1;//Enable DMA on all channels */
/*     *DMA0_DMAOR&=~6;//Clear flags */
/*     *DMA0_CHCR_0|=1;//Enable channel0 DMA */
/* } */
/* void DoDMAlcdNonblock(void){ */
/*     DoDMAlcdNonblockStrip(0,215); */
/* } */

typedef struct {} unit;

typedef enum {
  none,
  wep,
  wpa,
  wpa2_personal,
  wpa_wpa2_personal,
  wpa3_personal,
  wpa2_wpa3_personal,
  wapi_personal,
} auth_method;

typedef enum {
  signal,
  security,
} scan_sort_method;

typedef union {
  scan_sort_method complete_scan;
  unit fast_scan;
} scan_method;

typedef struct {
  unsigned char required: 1;
} pmf_capable;

typedef union {
  unit not_capable;
  pmf_capable capable;
} pmf_configuration;

typedef struct {
  unsigned char ssid[32];
  unsigned char bssid[6];
  auth_method auth;
  unsigned char password[64];
  union {
    unit none;
    unsigned char ch;
  } channel;
  scan_method scan;
  pmf_configuration pmf_cfg;
} client_configuration;

typedef struct {
  client_configuration conf;
} set_config;

typedef enum {
  check_is_started = 0,
  check_is_connected = 1,
  get_capabilities = 2,
  start = 3,
  stop = 4,
  scan = 5,
  connect = 6,
  disconnect = 7,
} unit_action;

typedef struct {
  unsigned int discriminant;
  union {
    unit_action
    set_config;
  } actions;
} wifi_actions;

typedef struct {
  void* buf;
  unsigned int len;
} rust_vec;

typedef enum {
  error = 0,
  is_started = 1,
  is_connected = 2,
  access_points = 3,
  capabilities = 4,
  started = 5,
  stopped = 6,
  connected = 7,
  disconnected = 8,
  configured = 9,
} res_id;

typedef struct {
  union {
    int error;
    unsigned int is_started: 1;
    unsigned int is_connected: 1;
    rust_vec access_points; // Vec<AccessPointInfo>
    //enum_set capabilities;
    res_id s;
  } responses;
} wifi_response;


int await_serial_conn(unsigned char mode[6]) {
  int total = 0;
  int start = RTC_GetTicks();
  while (total <= MAX_TICKS) {
    int elapsed = RTC_GetTicks() - start;
    total += elapsed;
    if (elapsed >= 92) {
      int result = Serial_Open(mode);
      if ((!result) || (result == 3)) {
	return 0;
      } else if ((result == 4) || (total >= MAX_TICKS)) {
	return 1;
      } else {
	start = RTC_GetTicks();
      }
    } 
  }
}

int start_wifi_service(unsigned char bitrate,
		       unsigned char parity,
		       unsigned char data_length,
		       unsigned char stop_bits) {
  unsigned char mode[6] = {0x0, bitrate, parity, data_length, stop_bits, 0x0};
  if (await_serial_conn(mode) == 1) {
    // Should be connected
    char* msg = malloc(sizeof(char)*30);
    Serial_ClearRX();
    while (1) {
      Serial_WriteSingle(scan);
      
      PrintLine(sprintf(msg, "Writing %d!", scan), 15);
    }
    int recv = Serial_PollRX();
    while (recv == 0) {
      recv = Serial_PollRX();
    }
    res_id res;
    Serial_ReadSingle(&res);
    PrintLine(sprintf(msg, "Received %d!", res), 15);
    return res;
  }
  return -1;
}


#define PLL_24x 0b010111 // 87 MHz
#define PLL_18x 0b010001 // 65.25 MHz
#define PLL_16x 0b001111 // 58 MHz (NORMAL SPEED)

/* void change_freq(int mult) { */
/*    __asm__( */
/*        "mov r4, r0\n\t" */
/*        "and #0x3F, r0\n\t"  */
/*        "shll16 r0\n\t"  */
/*        "shll8 r0\n\t"  */
/*        "mov.l frqcr, r1\n\t"   */
/*        "mov.l pll_mask, r3\n\t"   */
/*        "mov.l @r1, r2\n\t"   */
/*        "and r3, r2\n\t"   */
/*        "or r0, r2\n\t"  */
/*        "mov.l r2, @r1\n\t"  */
/*        "mov.l frqcr_kick_bit, r0\n\t"  */
/*        "mov.l @r1, r2\n\t" */
/*        "or r0, r2\n\t" */
/*        "rts\n\t" */
/*        "mov.l r2, @r1\n\t" */
/*        ".align 4\n\t" */
/*        "frqcr_kick_bit: .long 0x80000000\n\t" */
/*        "pll_mask: .long 0xC0FFFFFF\n\t"  */
/*        "frqcr: .long 0xA4150000\n\t" */
/*    ); */
/* } */

/* #define LCD_GRAM    0x202 */
/* #define LCD_BASE    0xB4000000 */
/* #define SYNCO() __asm__ volatile("SYNCO\n\t":::"memory"); */
/* // Module Stop Register 0 */
/* #define MSTPCR0     (volatile unsigned*)0xA4150030 */
/* // DMA0 operation register */
/* #define DMA0_DMAOR  (volatile unsigned short*)0xFE008060 */
/* #define DMA0_SAR_0  (volatile unsigned*)0xFE008020 */
/* #define DMA0_DAR_0  (volatile unsigned*)0xFE008024 */
/* #define DMA0_TCR_0  (volatile unsigned*)0xFE008028 */
/* #define DMA0_CHCR_0 (volatile unsigned*)0xFE00802C */
/* void DmaWaitNext(void){ */
/*     if ((*DMA0_CHCR_0)&2 || (*DMA0_DMAOR)&4) { */
/*       return 1; */
/*     } else { */
      
/*       return 0; */
/*     } */
/* } */
/* void DoDMAlcdNonblockStrip(unsigned y1,unsigned y2){ */
/*     Bdisp_WriteDDRegister3_bit7(1); */
/*     Bdisp_DefineDMARange(6,389,y1,y2); */
/*     Bdisp_DDRegisterSelect(LCD_GRAM); */
/*     *MSTPCR0&=~(1<<21);//Clear bit 21 */
/*     *DMA0_CHCR_0&=~1;//Disable DMA on channel 0 */
/*     *DMA0_DMAOR=0;//Disable all DMA */
/*     *DMA0_SAR_0=(GetVRAMAddress()+(y1*384*2))&0x1FFFFFFF;//Source address is VRAM */
/*     *DMA0_DAR_0=LCD_BASE&0x1FFFFFFF;//Destination is LCD */
/*     *DMA0_TCR_0=((y2-y1+1)*384)/16;//Transfer count bytes/32 */
/*     *DMA0_CHCR_0=0x00101400; */
/*     *DMA0_DMAOR|=1;//Enable DMA on all channels */
/*     *DMA0_DMAOR&=~6;//Clear flags */
/*     *DMA0_CHCR_0|=1;//Enable channel0 DMA */
/* } */
/* void DoDMAlcdNonblock(void){ */
/*     DoDMAlcdNonblockStrip(0,215); */
/* } */

/* int init() { */
/*   if (start_wifi_service(9, 0, 0, 0)) */
/* } */


int main() {
  int i = 0;
  Bdisp_EnableColor(1);
  Bdisp_Fill_VRAM(COLOR_SILVER, 1);

  PrintLine("Haven't written a byte", 15);
  int key;
  unsigned char mode[6] = {0, 6, 2, 1, 0, 0};
  char* msg = malloc(sizeof(char)*30);
  sprintf(&msg, "Comm_Open returned %d", Comm_Open(0xffef));
  PrintLine(msg, 15);
  OS_InnerWait_ms(2500);
  sprintf(&msg, "Comm_Open returned %d", Serial_Open(mode));
  return 0;
}
