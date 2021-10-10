#ifndef _CAN_HW_H
#define _CAN_HW_H

#ifdef __cplusplus
    extern "C" {
#endif

// Baudrates (see http://www.bittiming.can-wiki.info/)

#define CAN_BAUDRATE_1000KHZ 0x00060003 /*   25 m */
#define CAN_BAUDRATE_800KHZ  0x001B0002 /*   50 m */
#define CAN_BAUDRATE_500KHZ  0x00050008 /*  100 m */
#define CAN_BAUDRATE_250KHZ  0x001C0008 /*  250 m */
#define CAN_BAUDRATE_125KHZ  0x001C0011 /*  500 m */
#define CAN_BAUDRATE_100KHZ  0x0005002C
#define CAN_BAUDRATE_83KHZ   0x001C001A
#define CAN_BAUDRATE_50KHZ   0x001C002C /* 1000 m */
#define CAN_BAUDRATE_20KHZ   0x000500E0 /* 2500 m */
#define CAN_BAUDRATE_10KHZ   0x001C00E0 /* 5000 m */

// Public functions

void can_hw_init(void);
void USB_HP_CAN1_TX_IRQHandler(void);
void USB_LP_CAN1_RX0_IRQHandler(void);

#if CAN_TX    
    void can_hw_tx(can_hw_t * packet);
#endif

#if CAN_RX
    int32_t can_hw_filter(int32_t n, uint32_t id, uint32_t id_mask, uint8_t ext, uint8_t ext_mask);
#endif

#endif

#ifdef __cplusplus
    }
#endif
