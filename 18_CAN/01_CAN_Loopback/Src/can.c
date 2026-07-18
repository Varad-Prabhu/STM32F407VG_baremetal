#include "can.h"
#include "gpio.h"

void CAN_Init(void)
{
    GPIO_InitTypeDef GPIO_CAN1;

    GPIO_CAN1.Port = GPIOB;
    GPIO_CAN1.Pin = GPIO_PIN_8 | GPIO_PIN_9;                                /* PB9 -> CAN1_TX, PB8 -> CAN1_Rx */
    GPIO_CAN1.Mode = GPIO_MODE_AF;
    GPIO_CAN1.Pull = GPIO_NOPULL;
    GPIO_CAN1.Speed = GPIO_SPEED_HIGH;
    GPIO_CAN1.OutputType = GPIO_OTYPE_PP;
    GPIO_CAN1.Alternate = GPIO_AF9_CAN1;

    GPIO_Init(&GPIO_CAN1);

    RCC->APB1ENR |= (1U << RCC_APB1ENR_CAN1EN_Pos);                         /* Enable clock for CAN1 */
    
    CAN1->MCR |= (1U << CAN_MCR_INRQ_Pos);                                  /* Request CAN initialization mode */
    while (!(CAN1->MSR & (1U << CAN_MSR_INAK_Pos)));                        /* Wait until hardware acknowledges */

    CAN1->MCR &= ~(1U << CAN_MCR_SLEEP_Pos);                                /* Disable sleep mode */
    CAN1->MCR &= ~(1U << CAN_MCR_TXFP_Pos);                                 /* Transmit FIFO priority driven by the identifier of the message */
    CAN1->MCR &= ~(1U << CAN_MCR_NART_Pos);                                 /* CAN hardware automatically retransmits the message until it has been successfully transmitted according to the CAN standard */
    CAN1->MCR &= ~(1U << CAN_MCR_AWUM_Pos);                                 /* Disable automatic wakeup*/
    CAN1->MCR &= ~(1U << CAN_MCR_TTCM_Pos);                                 /* Disable time triggerred communication */
    CAN1->MCR |= (1U << CAN_MCR_ABOM_Pos);                                  /* Automatic bus-off recovery */

    /*----------------------------------------------------------*/
    /* CAN Bit Timing Configuration                             */
    /*----------------------------------------------------------*/
    /* APB1 Clock (PCLK1) = 42 MHz                              */
    /*                                                          */
    /* Time Quantum (TQ) calculation:                           */
    /* TQ = (BRP + 1) / PCLK1                                   */
    /*    = (5 + 1) / 42 MHz                                    */
    /*    = 6 / 42000000                                        */
    /*    = 142.857 ns                                          */
    /*                                                          */
    /* CAN Bit Time = Sync_Seg + TS1 + TS2                      */
    /* Sync_Seg = 1 TQ (fixed by CAN protocol)                  */
    /* TS1      = 10 TQ                                         */
    /* TS2      = 3 TQ                                          */
    /*----------------------------------------------------------*/
    /* Total Bit Time = 1 + 10 + 3 = 14 TQ                      */
    /*                                                          */
    /* Bit Time = 14 × 142.857 ns                               */
    /*          = 2 µs                                          */
    /*                                                          */
    /* Baud Rate = 1 / Bit Time                                 */
    /*           = 1 / 2 µs                                     */
    /*           = 500 kbps                                     */
    /*----------------------------------------------------------*/

    CAN1->BTR = 0;                                                          /* Clear CAN Bit Timing Register */
    CAN1->BTR |= (1U << CAN_BTR_LBKM_Pos);                                  /* Enable Loopback mode so transmitted frames are internally received by the CAN controller without requiring another CAN node */
    CAN1->BTR |= (5U << CAN_BTR_BRP_Pos);                                   /* BRP = 5 -> Prescaler = BRP + 1 = 6 */
    CAN1->BTR |= (9U << CAN_BTR_TS1_Pos);                                   /* Time Segment 1 = 10 Time Quanta */
    CAN1->BTR |= (2U << CAN_BTR_TS2_Pos);                                   /* Time Segment 2 = 3 Time Quanta */
    CAN1->BTR |= (0U << CAN_BTR_SJW_Pos);                                   /* Synchronization Jump Width = 1 Time Quantum */

    /*----------------------------------------------------------*/
    /* Configure Filter Bank 0 in 32-bit Identifier Mask mode   */
    /* Mask = 0x00000000, so every received frame matches       */
    /* Accept all Standard and Extended CAN frames              */
    /* Store accepted frames into Receive FIFO 0                */
    /*----------------------------------------------------------*/
    CAN1->FMR |= (1U << CAN_FMR_FINIT_Pos);                                 /* Filter initialization mode */
    CAN1->FS1R |= (1U << CAN_FS1R_FSC0_Pos);                                /* 32 bit filter */
    CAN1->FM1R &= ~(1U << CAN_FM1R_FBM0_Pos);                               /* Two 32 bit registers of filter bank are in identifier mask mode */
    CAN1->FFA1R &= ~(1U << CAN_FFA1R_FFA0_Pos);                             /* Filter assigned to FIFO 0 */
    /* FR1 -> Meaning in identifier mask mode: Filter identifier, Meaning in identifier list mode: First CAN ID */
    /* FR2 -> Meaning in identifier mask mode: Filter mask, Meaning in identifier list mode: Second CAN ID */
    CAN1->sFilterRegister[0].FR1 = 0x00000000;                              /* Filter ID = 0 */
    CAN1->sFilterRegister[0].FR2 = 0x00000000;                              /* Mask = 0 -> Don't care about any ID bits, accept all frames */
    CAN1->FA1R |= (1U << CAN_FA1R_FACT0_Pos);                               /* Activate filter */
    CAN1->FMR &= ~(1U << CAN_FMR_FINIT_Pos);                                /* Exit filter initialization */

    CAN1->MCR &= ~(1U << CAN_MCR_INRQ_Pos);                                 /* Clear CAN initialization mode */
    while (CAN1->MSR & (1U << CAN_MSR_INAK_Pos));                           /* Wait until initialization mode is exited */
}

void CAN_Transmit(uint32_t id, uint8_t *data, uint8_t length)
{
    if (id > 0x7FFU)
    {
        return;
    }

    if (data == NULL)
    {
        return;
    }

    if (length > 8U)
    {
        length = 8U;
    }

    /* Check empty mailbox */
    while (!(CAN1->TSR & (1U << CAN_TSR_TME0_Pos)));                        /* Wait till transmit mailbox 0 is empty */
    
    /* Identifer */
    CAN1->sTxMailBox[0].TIR = 0;                                            /* Clear transmit identifier register */
    CAN1->sTxMailBox[0].TIR &= ~(1U << CAN_TI0R_RTR_Pos);                   /* Data frame */
    CAN1->sTxMailBox[0].TIR &= ~(1U << CAN_TI0R_IDE_Pos);                   /* Standard identifier */
    CAN1->sTxMailBox[0].TIR &= ~(0x3FFFFU << CAN_TI0R_EXID_Pos);            /* Extended identifier is not supported */
    CAN1->sTxMailBox[0].TIR &= ~(0x7FFU << CAN_TI0R_STID_Pos);              /* Clear standard identifier or extended identifier field */
    CAN1->sTxMailBox[0].TIR |= (id << CAN_TI0R_STID_Pos);                   /* Update identifier */
    
    /* DLC */
    CAN1->sTxMailBox[0].TDTR &= ~(0xFU << CAN_TDT0R_DLC_Pos);               /* Clear DLC bits */
    CAN1->sTxMailBox[0].TDTR |= (length << CAN_TDT0R_DLC_Pos);              /* Configure DLC */

    /* Data bytes 0-3 */
    CAN1->sTxMailBox[0].TDLR = ((uint32_t)data[3] << 24U) | ((uint32_t)data[2] << 16U) | ((uint32_t)data[1] << 8U) | ((uint32_t)data[0]);

    /* Data bytes 4-7 */
    CAN1->sTxMailBox[0].TDHR = ((uint32_t)data[7] << 24U) | ((uint32_t)data[6] << 16U) | ((uint32_t)data[5] << 8U) | ((uint32_t)data[4]);

    CAN1->sTxMailBox[0].TIR |= (1U << CAN_TI0R_TXRQ_Pos);                   /* Transmit mail box request */

    while (!(CAN1->TSR & (1U << CAN_TSR_RQCP0_Pos)));                       /* Wait until transmission completes */

    if (CAN1->TSR & (1U << CAN_TSR_TXOK0_Pos))
    {
        /* Transmission successful */
    }
    else
    {
        /* Transmission failed */
    }

    CAN1->TSR = (1U << CAN_TSR_RQCP0_Pos);                                  /* Clear completion flag for next transmission */
}

uint8_t CAN_Receive(uint32_t *id, uint8_t *data)
{
    uint8_t length;
    uint32_t rdlr, rdhr;

    if ((id == NULL) || (data == NULL))
    {
        return 0;
    }

    while (!(CAN1->RF0R & (1U << CAN_RF0R_FMP0_Pos)));                      /* Wait until at least one received CAN frame is available in Receive FIFO0 */

    if (CAN1->sFIFOMailBox[0].RIR & (1U << CAN_RI0R_IDE_Pos))               /* Currently only Standard Identifier supported */
    {
        CAN1->RF0R |= (1U << CAN_RF0R_RFOM0_Pos);
        return 0;
    }

    if (CAN1->sFIFOMailBox[0].RIR & (1U << CAN_RI0R_RTR_Pos))               /* Currently Remote frames not supported */
    {
        CAN1->RF0R |= (1U << CAN_RF0R_RFOM0_Pos);
        return 0;
    }

    *id = (CAN1->sFIFOMailBox[0].RIR >> CAN_RI0R_STID_Pos) & 0x7FFU;        /* Read Standard Identifier */
    length = (uint8_t)(CAN1->sFIFOMailBox[0].RDTR & 0x0FU);                 /* Read Data Length Code */

    /* Read Data Bytes 0-3 */
    rdlr = CAN1->sFIFOMailBox[0].RDLR;
    data[0] = (uint8_t)(rdlr);
    data[1] = (uint8_t)(rdlr >> 8U);
    data[2] = (uint8_t)(rdlr >> 16U);
    data[3] = (uint8_t)(rdlr >> 24U);

    /* Read Data Bytes 4-7 */
    rdhr = CAN1->sFIFOMailBox[0].RDHR;
    data[4] = (uint8_t)(rdhr);
    data[5] = (uint8_t)(rdhr >> 8U);
    data[6] = (uint8_t)(rdhr >> 16U);
    data[7] = (uint8_t)(rdhr >> 24U);

    CAN1->RF0R |= (1U << CAN_RF0R_RFOM0_Pos);                               /* Release FIFO0 so the hardware can store the next received frame */

    return length;
}
