
#ifndef _I2C_LIB_H_
#define _I2C_LIB_H_

#define I2C_NB_SLAVE_DATA   100


/*_____ D E F I N I T I O N S ______________________________________________*/

typedef struct
{ 
	uint8_t bAddress;
	uint16_t wLocalAddress;
    uint8_t bRw;
    uint16_t wNrOfByte;
    uint8_t *pToData;
} I2C_type_message;

/*_____ D E C L A R A T I O N ______________________________________________*/

extern  bit                 b_I2C_busy;
extern  unsigned char       I2C_Err;
extern  xdata               I2C_type_message  I2C_message;        // The I2C message to be sent in Master Mode
extern  unsigned char xdata I2C_slave_data[I2C_NB_SLAVE_DATA];  //The slave data buffer when I2C ask in slave


/*_____ M A C R O S ________________________________________________________*/


#define I2C_RATIO_256     0x00
#define I2C_RATIO_224     0x01
#define I2C_RATIO_192     0x02
#define I2C_RATIO_160     0x03
#define I2C_RATIO_960     0x80
#define I2C_RATIO_120     0x81
#define I2C_RATIO_60      0x82
#define I2C_RATIO_TIMER1  0x83

#define I2C_CR2           0x80
#define I2C_SSIE          0x40
#define I2C_STA           0x20
#define I2C_STO           0x10
#define I2C_SI            0x08
#define I2C_AA            0x04
#define I2C_CR1           0x02
#define I2C_CR0           0x01

#define I2C_WAIT_EVENT    { while ( (SSCON & I2C_SI) != I2C_SI ); }
#define I2C_SET_START     { SSCON |= I2C_STA;   } 
#define I2C_CLEAR_START   { SSCON &= ~I2C_STA;  }
#define I2C_SET_STOP      { SSCON |= I2C_STO;   }
#define I2C_CLEAR_STOP    { SSCON &= ~I2C_STO;  }
#define I2C_SET_AA        { SSCON |= I2C_AA;    } 
#define I2C_CLEAR_AA      { SSCON &= ~I2C_AA;   }
#define I2C_CLEAR_SI      { SSCON &= ~I2C_SI;   }



#define Tranfer_OK      0
#define BusError        1
#define NoHostAdrAck    2
#define NoHostDataAck   3
#define ArbitrationLost 4
#define UnknownError    5
#define I2C_NOT_FREE    6


#define I2C_READ        1
#define I2C_WRITE       0

/*_____ P R O T O T Y P E S ____________________________________________________________*/

unsigned char I2C_baud_rate (unsigned int ratio);
uint8_t I2C_Send_Message_polling( uint8_t bSlaveAddr, uint16_t wLocalAddress, uint8_t bRw, uint16_t wNrOfByte, uint8_t * pToData );
unsigned char I2C_Send_Message_interrupt( unsigned char slave_adr,bit rw, unsigned char nbytes, unsigned char *info);
void I2C_decode_status();
unsigned char I2C_slave_polling(void);
void I2C_slave_interrupt(void);
void I2C_Init(void);

void I2CReadVpc3Byte	( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData );
void I2CWriteVpc3Byte	( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData );

void DebugI2CReadVpc3Byte	( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData );
void DebugI2CWriteVpc3Byte	( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData );

#endif /* _I2C_H_ */