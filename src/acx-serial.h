// Author:  Alexander Clarke
// Date:    2021-09-16


#ifndef ACX_SERIAL_H_
  #define ACX_SERIAL_H_
  // Make sure bool is defined.
  #ifndef bool
    #include <stdbool.h>
  #endif

  // Make sure NULL is defined for pointers.
  #ifndef NULL
    #define NULL ((void*)0)
  #endif

  /* ----- Framing parameters ----- */
  #define DATABITS 1
  #define STOPBITS 3
  #define PARITYBITS 4

  #define SERIAL_5N1  (0x00 | (0 << DATABITS))
  #define SERIAL_6N1  (0x00 | (1 << DATABITS))
  #define SERIAL_7N1  (0x00 | (2 << DATABITS))
  #define SERIAL_8N1  (0x00 | (3 << DATABITS))
  #define SERIAL_5N2  (0x08 | (0 << DATABITS))
  #define SERIAL_6N2  (0x08 | (1 << DATABITS))
  #define SERIAL_7N2  (0x08 | (2 << DATABITS))
  #define SERIAL_8N2  (0x08 | (3 << DATABITS))
  #define SERIAL_5E1  (0x20 | (0 << DATABITS))
  #define SERIAL_6E1  (0x20 | (1 << DATABITS))
  #define SERIAL_7E1  (0x20 | (2 << DATABITS))
  #define SERIAL_8E1  (0x20 | (3 << DATABITS))
  #define SERIAL_5E2  (0x28 | (0 << DATABITS))
  #define SERIAL_6E2  (0x28 | (1 << DATABITS))
  #define SERIAL_7E2  (0x28 | (2 << DATABITS))
  #define SERIAL_8E2  (0x28 | (3 << DATABITS))
  #define SERIAL_5O1  (0x30 | (0 << DATABITS))
  #define SERIAL_6O1  (0x30 | (1 << DATABITS))
  #define SERIAL_7O1  (0x30 | (2 << DATABITS))
  #define SERIAL_8O1  (0x30 | (3 << DATABITS))
  #define SERIAL_5O2  (0x38 | (0 << DATABITS))
  #define SERIAL_6O2  (0x38 | (1 << DATABITS))
  #define SERIAL_7O2  (0x38 | (2 << DATABITS))
  #define SERIAL_8O2  (0x38 | (3 << DATABITS))

  // PROVIDE SOME #defines for your RX and TX buffer sizes for each port
  #define DEFAULT_BUFFER_SIZE 64 // Must be power of 2.
  #define RX_BUFFER_SIZE DEFAULT_BUFFER_SIZE
  #define TX_BUFFER_SIZE DEFAULT_BUFFER_SIZE

  /* ----- Queue Control Block ----- */

  #define Q_EMPTY 0b00000001
  #define Q_FULL 0b00000010

  typedef struct {
    uint8_t   in;         // index of next char to be placed in queue (if not full)
    uint8_t   out;        // index of next char to be removed from queue (if not empty)
    uint8_t   smask;      // mask used to maintain circular queue access (mod size)
    uint8_t   flags;      // stores full and empty flags
    uint16_t  available;  // number of bytes available to be read from queue
    uint8_t*  pQ;         // pointer to queue data buffer
  } QCB;

  bool Q_putc(QCB* qPtr, uint8_t val);
  bool Q_getc(QCB* qPtr, uint8_t* valPtr);
  bool Q_init(QCB* qPtr, uint8_t*);

    /* ----- Serial Port Registers type ----- */
  typedef struct {
    volatile uint8_t  UCSRnA;
    volatile uint8_t  UCSRnB;
    volatile uint8_t  UCSRnC;
    volatile uint8_t  rsvd;
    volatile uint16_t UBRRn;
    volatile uint8_t  UDRn;
  } SERIAL_PORT_REGS;

  typedef struct {
    QCB* rxq;
    QCB* txq;
    SERIAL_PORT_REGS* regs;
  } SERIAL_PORT;

  /* ----- Prototypes ----- */
  bool x_serial_init(long baudrate, int framing);
  int x_serial_getc();
  int x_serial_putc(char c);
  int x_serial_gets(int maxlen, char* buffer);
  int x_serial_puts(char* buffer);
  uint16_t BaudRateToBitTime(long speed, bool u2x);
#endif