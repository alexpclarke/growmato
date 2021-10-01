#ifndef ACX_SRF_DEFS_H_
  #define ACX_SRF_DEFS_H_

  #define T_STACK_SIZE 128

  /* ----- SRAM DEFS ----- */
  #define T_BUFF_START 0x0400
  // #define T_BUFF _SFR_MEM8(T_BUFF_START)
  // #define TN_BUFF(n) (uint8_t*)&_SFR_MEM8(T_BUFF_START + (T_STACK_SIZE * n))
  #define TN_DELAY(n) _SFR_MEM16(T_BUFF_START + (T_STACK_SIZE * n))
  #define TN_SP(n) _SFR_MEM16(T_BUFF_START + (T_STACK_SIZE * n) + 2)
  #define TN_SPBASE(n) (uint8_t*)(T_BUFF_START + (T_STACK_SIZE * n) + (T_STACK_SIZE - 1))

  #define T_ID GPIOR0
  #define T_DISABLED_STATUS GPIOR1
  #define T_DELAYED_STATUS GPIOR2
#endif