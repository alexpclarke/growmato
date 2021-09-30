# Memory
## Flash
32k bytes (of which .5k is used for the bootloader)

| Start  | End    | Description |
| ------ | ------ | ----------- |
| 0x0000 | 0x07FF | ???         |

# SRAM
2k bytes
1k byte

| Start  | End    | Description     |
| ------ | ------ | --------------- |
| 0x0000 | 0x007F | Thread 0 Stack  |
| 0x0080 | 0x00FF | Thread 1 Stack  |
| 0x0100 | 0x017F | Thread 2 Stack  |
| 0x0180 | 0x01FF | Thread 3 Stack  |
| 0x0200 | 0x02F7 | Thread 4 Stack  |
| 0x0280 | 0x02FF | Thread 5 Stack  |
| 0x0300 | 0x03F7 | Thread 6 Stack  |
| 0x0380 | 0x03FF | Thread 7 Stack  |
| 0x0400 | 0x043F | USART RX Buffer |
| 0x0440 | 0x047F | USART TX Buffer |
| 0x0480 | 0x07FF | Unused          |

## EEPROM

| Start  | End    | Description |
| ------ | ------ | ----------- |
| 0x0000 | 0x03FF | Unused      |
