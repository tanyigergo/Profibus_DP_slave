/************************  Filename: twi.c  **********************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0      Einsteinstraße 6        */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      91074 Herzogenaurach    */
/* 0000  000  0   0 000   0 0     00000 0 0000       Germany                 */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Tel: ++49-9132-744-200  */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax: ++49-9132-744-204  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Function:       I2C Funktionen                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/*                                                                           */
/* Technical support:       P. Fredehorst                                    */
/*                          Tel. : ++49-9132/744-2150                        */
/*                          Fax. :              -2164                        */
/*                          eMail: pfredehorst@profichip.com                 */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* contents:

  - function prototypes
  - data structures
  - internal functions

*/
/*****************************************************************************/
/* include hierarchy */

#include "..\platform.h"
#include "twi.h"

#if VPC3_SERIAL_MODE
   #if VPC3_I2C


volatile bit   b_I2C_busy;
volatile uint8_t I2C_Err;
volatile uint8_t I2C_nb_transmited;
volatile uint8_t I2C_wLocalAddressTransmited;
volatile uint8_t I2C_RecPtr;

volatile uint8_t xdata I2C_slave_data[I2C_NB_SLAVE_DATA];

volatile I2C_type_message xdata	I2C_message;

uint8_t DebugEnable;

/*---------------------------------------------------------------------------*/
/* function: I2C_Error                                                       */
/*---------------------------------------------------------------------------*/
void I2C_Error( uint8_t bTransferTyp, uint16_t wSubAddress, uint8_t bErrorCode )
{
	print_string("\r\nI2C-Error: ");
	print_hexbyte( bTransferTyp );
	print_hexword( wSubAddress );
	print_hexbyte( bErrorCode );

	while( 1 )
	{
      #ifdef RS232_SERIO
         if(bRecCounter > 0)
         {
            PrintSerialInputs();
         }
      #endif//#ifdef RS232_SERIO
	}//while( 1 )		
}//void I2C_Error( uint8_t bErrorCode )

uint8_t i2C_baud_rate (unsigned int ratio)
{
  switch ( ratio )
  {
    case 256:
      return I2C_RATIO_256;
    break;

    case 224:
      return I2C_RATIO_224;
    break;

    case 192:
     return I2C_RATIO_192;
    break;

    case 160:
      return I2C_RATIO_160;
    break;

    case 960:
      return I2C_RATIO_960;
    break;

    case 120:
      return I2C_RATIO_120;
    break;

    case 60:
      return I2C_RATIO_60;
    break;
    }
}
	

void I2C_decode_status (void)
{
uint8_t bTemp;

  switch (SSSTA)
  {
    //  STATE 00h: Bus Error has occurred 
    //  ACTION:    Enter not addressed SLV mode and release bus
    case  0x00 :
      b_I2C_busy = FALSE;
      I2C_Err  = BusError;
    break;

    //STATE 08h: A start condition has been sent
    //ACTION:    SLR+R/W are transmitted, ACK bit received
    case 0x08 :
      I2C_CLEAR_START;
      SSDAT = I2C_message.bAddress<<1;
      if ( I2C_message.bRw == I2C_READ )	SSDAT++; // Add 1 for Read bit in AdrWord
      I2C_SET_AA;
      //from here to 0x18 transmit or 0x40 receive                 
    break;

    //STATE 10h: A repeated start condition has been sent
    //ACTION:    SLR+R/W are transmitted, ACK bit received
    case 0x10 :
      I2C_CLEAR_START;              // Reset STA bit in SSCON
      SSDAT = I2C_message.bAddress<<1;
      if ( I2C_message.bRw == I2C_READ )	SSDAT++; // Add 1 for Read bit in AdrWord
      I2C_SET_AA;                   //wait on ACK bit
    break;

    //STATE 18h:   SLR+W was transmitted, ACK bit received 
    //ACTION:      Transmit data byte, ACK bit received
    //PREVIOUS STATE: 0x08 or 0x10                     
    case 0x18 :                     	//master transmit, after sending
      I2C_nb_transmited=0;          	//slave address, now load data
	  I2C_wLocalAddressTransmited=0;
      //SSDAT = *(I2C_message.pToData);   //byte and send it              
      SSDAT = (uint8_t)(I2C_message.wLocalAddress>>8);   //byte and send it              
      I2C_SET_AA;                   	//wait on ACK bit
    break;

    //STATE 20h:   SLR+W was transmitted, NOT ACK bit received
    //ACTION:      Transmit STOP
    case 0x20 :
      I2C_SET_STOP;
      b_I2C_busy = FALSE;
      I2C_Err  = NoHostAdrAck;
    break;

  //STATE 28h:   DATA was transmitted, ACK bit received 
  //ACTION:      If last byte, send STOP, else send more data bytes 
  case 0x28 :             //master transmit, after sending
                          //data byte, ACK received
	if( I2C_wLocalAddressTransmited < 2 )
	{	
		I2C_wLocalAddressTransmited++;
		if( I2C_wLocalAddressTransmited == 1 ) 
		{	
   	  		SSDAT = (uint8_t)(I2C_message.wLocalAddress);
    		I2C_SET_AA;                           //wait on ACK bit
   	  	}//if( I2C_wLocalAddressTransmited == 1 )
   	  	else
   	  	{
   	  		if( I2C_message.wNrOfByte > 0 )
   	  		{
				bTemp = *(I2C_message.pToData);
    	  		SSDAT = bTemp;//*(I2C_message.pToData);
    	  		I2C_SET_AA;                           //wait on ACK bit
				if( DebugEnable )
				{
					print_hexbyte(bTemp);
				}
   	  		}//if( I2C_message.wNrOfByte > 0 )
   	  		else
   	  		{
    	  		I2C_SET_STOP;
    	  		I2C_Err  = Tranfer_OK;
    	  		b_I2C_busy = FALSE;         //transfer complete, clear b_I2C_busy
   	  		}//else of if( I2C_message.wNrOfByte > 0 )		
   	  	}//else of if( I2C_wLocalAddressTransmited == 1 )
	}//if( I2C_wLocalAddressTransmited < 2 )
    else
    {	
    	I2C_nb_transmited++;                    // inc nb data transmit on message 
    	I2C_message.pToData++;                      // inc pointer ti data to be transmited
    	if ( I2C_nb_transmited < I2C_message.wNrOfByte  ) //if there are still bytes to send
    	{
    	  bTemp = *(I2C_message.pToData);
    	  SSDAT = bTemp;//*(I2C_message.pToData);
    	  I2C_SET_AA;                           //wait on ACK bit
				if( DebugEnable )
				{
					print_hexbyte(bTemp);
				}
    	}
    	else 
    	{                    //run out of data, send stop,
    	  I2C_SET_STOP;
    	  I2C_Err  = Tranfer_OK;
    	  b_I2C_busy = FALSE;         //transfer complete, clear b_I2C_busy
    	}
    }//else of if( I2C_wLocalAddressTransmited < 2 )	
  break;

    //STATE 30h:   DATA was transmitted, NOT ACK bit received 
    //ACTION:      Transmit STOP     
    case 0x30 :
      I2C_SET_STOP;
      b_I2C_busy = FALSE;
      I2C_Err  = NoHostDataAck;
    break;

    //STATE 38h:   Arbitration lost in SLA+W or DATA.  
    //ACTION:      Release bus, enter not addressed SLV mode
    //             Wait for bus lines to be free 
    case 0x38 :
      b_I2C_busy = FALSE;
      I2C_Err  = ArbitrationLost;
      /*
      #ifdef USER_I2C_FCT_ARBITRATION_LOST_IN_SLA+W_OR_DATA
      I2C_fct_arb_lostinSLAorDATA();
      #endif
      */
    break;

    //MASTER RECEIVER MODE FOLLOWS 
    //STATE 40h:   SLA+R transmitted, ACK received 
    //ACTION:      Receive DATA, ACK to be returned 
    //PREVIOS STATE: 0x08 or 0x10
    case 0x40 :               //master receive, after sending
      if ( I2C_message.wNrOfByte == 1 )
      {
				if( DebugEnable )
				{
					print_string("k");
				}
        I2C_CLEAR_AA;      // only one data to receive, noACK to be send after the fisrt incoming data
      }
      else
      {
        I2C_SET_AA;      //wait on ACK bit 
        I2C_nb_transmited=0;      //data byte to be received, NOT ACK bit to follow  --> 0x58    
      }
    break;
  
    //STATE 48h:   SLA+R transmitted, NOT ACK received 
    //ACTION:      Transmit STOP 
    case 0x48 :
      I2C_SET_STOP;
      b_I2C_busy = FALSE;
      I2C_Err  = NoHostAdrAck;
    break;

    //STATE 50h:  Data has been received, ACK returned 
    //ACTION:     Read DATA. If expecting more continue else STOP 
    case 0x50 :   //master receive, received data 
                  //byte and ACK, copy it into
      bTemp = SSDAT;      //buffer             
      *(I2C_message.pToData+I2C_nb_transmited) = bTemp;//SSDAT;      //buffer 
      I2C_nb_transmited++;
      if ( I2C_nb_transmited < ( I2C_message.wNrOfByte - 1 ) )     //changed fredehorst
      {
        I2C_SET_AA;             //get more bytes 
      }
      else
      {
        I2C_CLEAR_AA;           //only one more byte to come 
      }
				if( DebugEnable )
				{
					print_hexbyte(bTemp);
				}
    break;

    //STATE 58h:   Data has been received, NOT ACK returned 
    //ACTION:      Read DATA. Generate STOP     
    case 0x58 :
      bTemp = SSDAT;      //buffer             
      *(I2C_message.pToData+I2C_nb_transmited) = bTemp;//SSDAT;      //buffer 
      I2C_SET_STOP;
      b_I2C_busy = FALSE;
      I2C_Err  = Tranfer_OK;
				if( DebugEnable )
				{
					print_string("END");
					print_hexbyte(bTemp);
				}
    break;

    //STATE 60h:   Own SLA+W has been received,ACK returned 
    //ACTION:      Read DATA. return ACK    
    case 0x60 :
      I2C_SET_AA;
      b_I2C_busy = TRUE;
      I2C_RecPtr=0;
    break;

    //STATE 68h:   Arbitration lost in SLA and R/W as MASTER. Own SLA 
    //             +W has been received. ACK returned   
    //ACTION:      Read DATA. return ACK
    //             re-start MASTER,
    case 0x68 :
      I2C_SET_START;
    break;

    //STATE 70h:   General call received, ACK returned
    //ACTION:      Receive DATA. return ACK
    case 0x70 :
      I2C_SET_AA;
      b_I2C_busy = TRUE;
    break;

    //STATE 78h:   Arbitration lost in SLA+R/W as MASTER. General call
    //             has arrived ack returned
    //ACTION:      Receive DATA. return ACK. Restart master
    case 0x78 :
      I2C_SET_START;
      b_I2C_busy = TRUE;
    break;

    //STATE 80h:   Previously addressed with own SLA. Data received 
    //             and ACK returned 
    //ACTION:      Read DATA. 
    case 0x80 :
      I2C_slave_data[I2C_RecPtr] = SSDAT;
      I2C_RecPtr ++;
      if ( I2C_RecPtr < I2C_NB_SLAVE_DATA )
      {
        I2C_SET_AA;  // ACK will be returned
      }
      else
      {
        I2C_CLEAR_AA; // it was last data not ACK will be returned ( buffer full )
      }
    break;

    //STATE 88h:   Previously addressed with own SLA. Data received     
    //             and NOT ACK returned                                 
    //ACTION:      Dont' Read DATA. Enter NOT addressed SLV mode        
    case 0x88 :
      I2C_SET_AA;  // ready to send another ACK if addresed as slave
    break;

    //STATE 90h:   Previously addressed with general call. Data received
    //             and ACK returned                                     
    //ACTION:      Read DATA.                                           
    case 0x90 :
      I2C_slave_data[I2C_RecPtr] = SSDAT;
      I2C_RecPtr = I2C_RecPtr + 1;
      SSCON  = 0xC1;
    break;

    //STATE 98h:   Previously addressed with general call. Data arrved
    //             and NOT ACK returned 
    //ACTION:      Dont Read DATA. Enter NOT addressed SLV mode
    case 0x98 :
      I2C_SET_AA;
    break;

    //STATE A0h:   A stop or repeated start has arrived, whilst still
    //             addressed as SLV/REC or SLV/TRX
    //ACTION:      Dont Read DATA. Enter NOT addressed SLV mode
    case 0xA0 :
      I2C_SET_AA;
      b_I2C_busy = FALSE;
      I2C_Err = Tranfer_OK;
    break;

    //STATE A8h:   addressed with own SLA+R
    //ACTION:      Prepare first data to be transmited
    case 0xA8 :
      I2C_SET_AA;
      b_I2C_busy = TRUE;
      I2C_RecPtr=0;
      SSDAT = I2C_slave_data[0];  //Prepare next data
    break;


    //STATE B8h:   Previously addressed with own SLA. Data sent 
    //             and ACK returned 
    //ACTION:      Write DATA. 
    case 0xB8 :
      I2C_RecPtr ++;
      if ( I2C_RecPtr < I2C_NB_SLAVE_DATA )
      {
        SSDAT = I2C_slave_data[I2C_RecPtr];
      }
      else
      {
      ///////// FIX ME : adressed with as slave + R : but not enought data"
        I2C_CLEAR_AA; // it was last data not ACK will be returned ( buffer full )
      }
    break;


    //STATE C0h:   Previously addressed with own SLA. Data sent 
    //             and NACK returned 
    //ACTION:      It was last data to be sent 
    case 0xC0 :
      b_I2C_busy=FALSE;
      I2C_Err  = Tranfer_OK;
    break;

     //if we arrived here, unknown state has occurred..... 
    default :
      I2C_SET_STOP;
      b_I2C_busy = FALSE;
      I2C_Err  = UnknownError;
    break;
  }
}


uint8_t I2C_Send_Message_polling( uint8_t bSlaveAddr, uint16_t wLocalAddress, uint8_t bRw, uint16_t wNrOfByte, uint8_t * pToData )
{
	I2C_message.bAddress = bSlaveAddr;
	I2C_message.bRw = bRw;
	I2C_message.wLocalAddress = wLocalAddress;
	I2C_message.wNrOfByte = wNrOfByte;
	I2C_message.pToData = pToData;
	I2C_CLEAR_STOP;
  	I2C_nb_transmited=0;

  	if( ~b_I2C_busy )
  	{
  	  	b_I2C_busy =1;
  	  	I2C_Err = Tranfer_OK;
  	  	I2C_SET_START;
  	  	while( b_I2C_busy  )
  	  	{
  	    	I2C_WAIT_EVENT;
  	    	I2C_decode_status();
  	    	I2C_CLEAR_SI;
  	  	}
  	  	I2C_SET_STOP;
  	  	return I2C_Err;
  	}//if( ~b_I2C_busy )
  	else
  	{
  	  	I2C_SET_STOP;
  	  	return I2C_NOT_FREE;
  	}//else of if( ~b_I2C_busy )
}//uint8_t I2C_Send_Message_polling( uint8_t bSlaveAddr, uint16_t wLocalAddress, uint8_t bRw, uint16_t wNrOfByte, uint8_t * pToData )

uint8_t I2C_Send_Message_interrupt (uint8_t slave_adr,bit rw, uint8_t nbytes, uint8_t *info)
{
  I2C_message.bAddress = slave_adr;
  I2C_message.wLocalAddress = 0;
  I2C_message.bRw = rw;
  I2C_message.wNrOfByte = nbytes;
  I2C_message.pToData = info;
  I2C_CLEAR_STOP;
  I2C_nb_transmited=0;
  if ( ~b_I2C_busy )
  {
    I2C_Err = Tranfer_OK;
    b_I2C_busy =1;
    I2C_SET_START; 
  }
  else
  {
    I2C_SET_STOP;
    return I2C_NOT_FREE;
  }
}

uint8_t I2C_slave_polling (void)
{
  I2C_WAIT_EVENT;
  I2C_decode_status();
  I2C_CLEAR_SI;
  while( b_I2C_busy  )
  {
    I2C_WAIT_EVENT;
    I2C_decode_status();
    I2C_CLEAR_SI;
  }
  return I2C_Err;
}

void I2C_slave_interrupt (void)
{
  //ENABLE_IT;
  //ENABLE_I2C_IT;
}


/*
void I2C_interrupt() interrupt IRQ_I2C using 1
{
  I2C_decode_status();
  I2C_CLEAR_SI;
}
*/

void I2C_Init( void )
{
	SSCON = 0;
	SSCON |= i2C_baud_rate(120);//60);
	SSCON |= MSK_SSPE;

	DebugEnable = FALSE;
}//void I2C_Init( void )



/*---------------------------------------------------------------------------*/
/* function: I2CWriteVpc3Byte                                                */
/*---------------------------------------------------------------------------*/
void I2CWriteVpc3Byte( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData )
{
uint8_t bI2C_Err;

	bI2C_Err = I2C_Send_Message_polling( bAddress, wSubAddress, I2C_WRITE, wNrOfBytes, pToData );
	if( bI2C_Err != Tranfer_OK )
	{
		I2C_Error( I2C_WRITE, wSubAddress, bI2C_Err );
	}//if( bI2C_Err != Tranfer_OK )	
}//void I2CWriteVpc3Byte( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData )

/*---------------------------------------------------------------------------*/
/* function: I2CReadVpc3Byte                                                 */
/*---------------------------------------------------------------------------*/
void I2CReadVpc3Byte( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData )
{
uint8_t bI2C_Err;

	bI2C_Err = I2C_Send_Message_polling( bAddress, wSubAddress, I2C_WRITE, 0, 0 );
	if( bI2C_Err != Tranfer_OK )
	{
		I2C_Error( I2C_WRITE, wSubAddress, bI2C_Err );
	}//if( bI2C_Err != Tranfer_OK )	

	bI2C_Err = I2C_Send_Message_polling( bAddress, 0, I2C_READ, wNrOfBytes, pToData );
	if( bI2C_Err != Tranfer_OK )
	{
		I2C_Error( I2C_READ, wSubAddress, bI2C_Err );
	}//if( bI2C_Err != Tranfer_OK )	
}//void I2CReadVpc3Byte( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData )

/*---------------------------------------------------------------------------*/
/* function: DebugI2CWriteVpc3Byte                                           */
/*---------------------------------------------------------------------------*/
void DebugI2CWriteVpc3Byte( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData )
{
uint8_t bI2C_Err;

	print_string("\r\nWrite: ");
	print_hexbyte( bAddress );
	print_hexword( wSubAddress );
	
	bI2C_Err = I2C_Send_Message_polling( bAddress, wSubAddress, I2C_WRITE, wNrOfBytes, pToData );
	if( bI2C_Err != Tranfer_OK )
	{
		I2C_Error( I2C_WRITE, wSubAddress, bI2C_Err );
	}//if( bI2C_Err != Tranfer_OK )	
}//void DebugI2CWriteVpc3Byte( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData )

/*---------------------------------------------------------------------------*/
/* function: DebugI2CReadVpc3Byte                                            */
/*---------------------------------------------------------------------------*/
void DebugI2CReadVpc3Byte( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData )
{
uint8_t bI2C_Err;

	print_string("\r\nRead: ");
	print_hexbyte( bAddress );
	print_hexword( wSubAddress );
	print_hexword( wNrOfBytes );

	DebugEnable = TRUE;

	bI2C_Err = I2C_Send_Message_polling( bAddress, wSubAddress, I2C_WRITE, 0, 0 );
	if( bI2C_Err != Tranfer_OK )
	{
		I2C_Error( I2C_WRITE, wSubAddress, bI2C_Err );
	}//if( bI2C_Err != Tranfer_OK )	

	bI2C_Err = I2C_Send_Message_polling( bAddress, 0, I2C_READ, wNrOfBytes, pToData );
	if( bI2C_Err != Tranfer_OK )
	{
		I2C_Error( I2C_READ, wSubAddress, bI2C_Err );
	}//if( bI2C_Err != Tranfer_OK )	
	
	DebugEnable = FALSE;
}//void DebugI2CReadVpc3Byte( uint8_t bAddress, uint16_t wSubAddress, uint16_t wNrOfBytes, uint8_t * pToData )


   #endif//#if VPC3_I2C
#endif//#if VPC3_SERIAL_MODE

