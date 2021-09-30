# Memory
## Flash
32k bytes (of which .5k is used for the bootloader)

| Start  | End    | Description |
| ------ | ------ | ----------- |
| 0x0000 | 0x07FF | ???         |

# SRAM

## Memory Map
| Start  | End    | Description            |
| ------ | ------ | ---------------------- |
| 0x0000 | 0x001F | 32 Registers           |
| 0x0020 | 0x005F | 64 I/O Registers       |
| 0x0060 | 0x00FF | 160 Ext. I/O Registers |
| 0x0100 | 0x08FF | Internal SRAM          |

## Registers

| Addr.  | Description |
| ------ | ----------- |
| 0x0000 | R0          |
| 0x0001 | R1          |
| 0x0002 | R2          |
| 0x0003 | R3          |
| 0x0004 | R4          |
| 0x0005 | R5          |
| 0x0006 | R6          |
| 0x0007 | R7          |
| 0x0008 | R8          |
| 0x0009 | R9          |
| 0x000A | R10         |
| 0x000B | R11         |
| 0x000C | R12         |
| 0x000D | R13         |
| 0x000E | R14         |
| 0x000F | R15         |
| 0x0010 | R16         |
| 0x0011 | R17         |
| 0x0012 | R18         |
| 0x0013 | R19         |
| 0x0014 | R20         |
| 0x0015 | R21         |
| 0x0016 | R22         |
| 0x0017 | R23         |
| 0x0018 | R24         |
| 0x0019 | R25         |
| 0x001A | R26         |
| 0x001B | R27         |
| 0x001C | R28         |
| 0x001D | R29         |
| 0x001E | R30         |
| 0x001F | R31         |

## I/O Registers

| Addr.  | Description |
| ------ | ----------- |
| 0x0020 | Reserved    |
| 0x0021 | Reserved    |
| 0x0022 | Reserved    |
| 0x0023 | PINB        |
| 0x0024 | DDRB        |
| 0x0025 | PORTB       |
| 0x0026 | PINC        |
| 0x0027 | DDRC        |
| 0x0028 | PORTC       |
| 0x0029 | PIND        |
| 0x002A | DDRD        |
| 0x002B | PORTD       |
| 0x002C | Reserved    |
| 0x002D | Reserved    |
| 0x002E | Reserved    |
| 0x002F | Reserved    |
| 0x0030 | Reserved    |
| 0x0031 | Reserved    |
| 0x0032 | Reserved    |
| 0x0033 | Reserved    |
| 0x0034 | Reserved    |
| 0x0035 | TIFR0       |
| 0x0036 | TIFR1       |
| 0x0037 | TIFR2       |
| 0x0038 | Reserved    |
| 0x0039 | Reserved    |
| 0x003A | Reserved    |
| 0x003B | PCIFR       |
| 0x003C | EIFR        |
| 0x003D | EIMSK       |
| 0x003E | GPIOR0      |
| 0x003F | EECR        |
| 0x0040 | EEDR        |
| 0x0041 | EEARL       |
| 0x0042 | EEARH       |
| 0x0043 | GTCCR       |
| 0x0044 | TCCR0A      |
| 0x0045 | TCCR0B      |
| 0x0046 | TCNT0       |
| 0x0047 | OCR0A       |
| 0x0048 | OCR0B       |
| 0x0049 | Reserved    |
| 0x004A | GPIOR1      |
| 0x004B | GPIOR2      |
| 0x004C | SPCR        |
| 0x004D | SPSR        |
| 0x004E | SPDR        |
| 0x004F | Reserved    |

## Ext. I/O Registers

## Internal SRAM
| Start  | End    | Description           |
| ------ | ------ | --------------------- |
| 0x0100 | 0x04FF | Thread Stacks         |
| 0x0500 | 0x050F | Thread Delay Counters |
| 0x0510 | 0x0510 | T_DISSABLED_STATUS    |
| 0x0511 | 0x0511 | T_DELAYED_STATUS      |
| 0x0512 | 0x0512 | T_ID                  |
| 0x0513 | 0x0552 | 


| 0x0512 | 0x08FF | Unused                |


| 0x0510 | 0x054F | USART RX Buffer       |
| 0x0550 | 0x058F | USART TX Buffer       |


## EEPROM

| Start  | End    | Description |
| ------ | ------ | ----------- |
| 0x0000 | 0x03FF | Unused      |
