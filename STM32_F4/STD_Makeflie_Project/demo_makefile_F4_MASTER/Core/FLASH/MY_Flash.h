/*
 *  Flash.h
 *  Save data into Flash for situation power off ,after MCU reset data will reload 
 *  from flash and write into QEI 
 *  Created on: May 23, 2016
 *  Author: AnhTuan
 */
 #ifndef MY_Flash_H_
#define  MY_Flash_H_

#define FLASH_Sector_HOME						FLASH_Sector_11
#define FLASH_Sector_LAB						FLASH_Sector_10 
#define FLASH_Sector_User_Program		FLASH_Sector_9

#define startAddress_HOME  					0x080E0000 //starting from 896KB, the beginning of last sector 
#define startAddress_LAB  				  0x080C0000 //starting from 768KB, the beginning of last sector 
#define startAddress_User_Program		0x080A0000 //starting from 512KB, the beginning of last sector 
										//0x8010000 is 64KB
										//0x8040000 is 256KB
										//0x8080000 is 512KB
										//0x80C0000 is 768KB
									
								  
// /* Base address of the Flash sectors */
// #define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
// #define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
// #define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
// #define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
// #define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
// #define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
// #define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
// #define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
// #define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
// #define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
// #define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
// #define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */	

extern void Get_possition(void);// get data of encoder after that save into encoder register
extern void Save_Pos(void);//Save all data content QEI value and POSx value 
extern void Display_possition(void);//Display Data from Flash 
extern void Load_possition(void);//Load data from Flash save into Servo_Pluse_buffer[j][i]
extern void Write_User_Program(uint8_t code[] ,uint16_t lenght) ;
extern uint8_t  Read_Byte(uint32_t address);
extern uint32_t Read_Word(uint32_t address);
extern uint16_t Read_HalfWord(uint32_t address);

#endif /*  MY_Flash_H_ */


