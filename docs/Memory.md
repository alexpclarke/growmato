# Memory
## Flash
| Start  | End    | Description |
| ------ | ------ | ----------- |
| 0x0000 | 0x07FF | ???         |

## SRAM

### Memory Map
| Start  | End    | Description            |
| ------ | ------ | ---------------------- |
| 0x0000 | 0x001F | 32 Registers           |
| 0x0020 | 0x005F | 64 I/O Registers       |
| 0x0060 | 0x00FF | 160 Ext. I/O Registers |
| 0x0100 | 0x08FF | Internal SRAM          |

### Internal SRAM
| Start  | End    | Description           |
| ------ | ------ | --------------------- |
| 0x0100 | 0x04FF | Thread Stacks         |
| 0x0500 | 0x050F | Thread Delay Counters |
| 0x0510 | 0x0510 | T_DISSABLED_STATUS    |
| 0x0511 | 0x0511 | T_DELAYED_STATUS      |
| 0x0512 | 0x0512 | T_ID                  |
| 0x0513 | 0x08FF | Unused                |

## EEPROM
| Start  | End    | Description |
| ------ | ------ | ----------- |
| 0x0000 | 0x03FF | Unused      |
