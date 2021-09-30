#ifndef ACX_SRF_DEFS_H_
  #define ACX_SRF_DEFS_H_

  /* ----- SRAM DEFS ----- */
  #define T_STACK_BUFF _SFR_MEM8(0x0100)
  #define T_DELAY_BUFF _SFR_MEM16(0x0500)
  #define T_DISABLED_STATUS _SFR_MEM8(0x0510)
  #define T_DELAYED_STATUS _SFR_MEM8(0x0511)
  #define T_ID _SFR_MEM8(0x0512)
#endif