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

### Registers

| Address | Description       |
| ------- | ----------------- |
| 0x001E  | T_ID              |
| 0x002A  | T_DISABLED_STATUS |
| 0x002B  | T_DELAYED_STATUS  |

### Internal SRAM

| Start  | End    | Description           |
| ------ | ------ | --------------------- |
| 0x0000 | 0x037F | Unused                |
| 0x0380 | 0x03FF | Thread Delay Counters |
| 0x0400 | 0x07FF | Thread Stacks         |

## EEPROM
| Start  | End    | Description |
| ------ | ------ | ----------- |
| 0x0000 | 0x03FF | Unused      |

1. Device Name
2. Description
3. Time of last boot
4. Min temperature
5. Max temperature
6. Min humidity
7. Max humidity
8. Light cycle data
9. 

## Thread Stack Structure

| Start  | End    | Description           |
| ------ | ------ | --------------------- |
| 0x0000 | 0x0001 | Delay                 |
| 0x0002 | 0x0003 | stack pointer         |
| 0x0004 | 0x0080 | stack buffer          |

## Ring Buffer Structure

| Start  | End    | Description           |
| ------ | ------ | --------------------- |
| 0x0000 | 0x0000 | In Index              |
| 0x0001 | 0x0001 | Out Index             |
| 0x0002 | 0x0003 | available             |
| 0x0004 | 0x0040 | stack buffer          |