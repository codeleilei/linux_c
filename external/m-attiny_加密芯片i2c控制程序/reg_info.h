/* 
 * File:   reg_info.h
 * Author: xbh <xbrahen@gmail.com Hi-Target Co., Ltd>
 *
 * Created on November 20, 2013, 9:48 AM
 */

#ifndef REG_INFO_H
#define	REG_INFO_H

#ifdef	__cplusplus
extern "C" {
#endif

    /* hardware information addresses and size, in bytes */
#define REG_MAX_SIZE    (200-1)
#define PRODUCT_MODEL_ADDR      0x00
#define PRODUCT_MODEL_SIZE      1 /*  */
#define MACHINE_NUMBER_ADDR     (PRODUCT_MODEL_ADDR + PRODUCT_MODEL_SIZE)
#define MACHINE_NUMBER_SIZE     16
#define REGISTRATION_CODE_ADDR  (MACHINE_NUMBER_ADDR + MACHINE_NUMBER_SIZE)
#define REGISTRATION_CODE_SIZE  32
#define REGISTRATION_DATE_ADDR  (REGISTRATION_CODE_ADDR + REGISTRATION_CODE_SIZE)
#define REGISTRATION_DATE_SIZE  6
#define EDITION_ADDR            (REGISTRATION_DATE_ADDR + REGISTRATION_DATE_SIZE)
#define EDITION_SIZE            2
#define FUNCTIONALITY_LIMIT_ADDR        (EDITION_ADDR + EDITION_SIZE)
#define FUNCTIONALITY_LIMIT_SIZE        4 /* function:4-bytes */

    /* accelerometer parameter */
#define ACCEL_HORIZ_COEF_ADDR        (FUNCTIONALITY_LIMIT_ADDR + FUNCTIONALITY_LIMIT_SIZE)
#define ACCEL_HORIZ_COEF_SIZE      50

    /* ad parameter */
#define ADC_PARAMETER8V_COEF_ADDR    (ACCEL_HORIZ_COEF_ADDR + ACCEL_HORIZ_COEF_SIZE)
#define ADC_PARAMETER8V_SIZE    2
#define ADC_PARAMETER12V_COEF_ADDR    (ADC_PARAMETER8V_COEF_ADDR + ADC_PARAMETER8V_SIZE)
#define ADC_PARAMETER12V_SIZE    2

    /* language */
#define LANGUAGE_LIMMIT_ADDR	(ADC_PARAMETER12V_COEF_ADDR + ADC_PARAMETER12V_SIZE)
#define LANGUAGE_LIMMIT_SIZE	1

#define EXPIRED_FLAG    (LANGUAGE_LIMMIT_ADDR + LANGUAGE_LIMMIT_SIZE)
#define EXPIRED_FLAG_SIZE       1

#define MAC_ADDR (EXPIRED_FLAG + EXPIRED_FLAG_SIZE)
#define MAC_ADDR_SIZE 20

#define LOCK_MARK_ADDR       (MAC_ADDR + MAC_ADDR_SIZE)
#define LOCK_MARK_SIZE  2

#define MAGNET_EPARA_ADDR (LOCK_MARK_ADDR + LOCK_MARK_SIZE)
#define MAGNET_EPARA_SIZE        35

#define BACKUP_ADDR     (MAGNET_EPARA_ADDR + MAGNET_EPARA_SIZE)
#define BACKUP_SIZE     (25)

#define REG_LIMIT (BACKUP_ADDR + BACKUP_SIZE)

#if REG_LIMIT > (199)
#error "running out of memory in attiny45"
#endif

#define ED_STANDARD        (0x0001 << 0)        /* the standard edition */
#define ED_CLASSIC         (0x0001 << 1)
#define ED_PREMIUM         (0x0001 << 2)
#define ED_COMPASS         (0x0001 << 3)
#define ED_CDMA_PREMIUM         (0x0001 << 4)
#define ED_IRTK2                (0x0001 << 6)  
#define ED_V90                (0x0001 << 7)  
#define ED_A12                (0x0001 << 8)  
#define ED_IRTK2S                (0x0001 << 9)  
    /* functionality limit, bit significant
     * for now, only the lower byte is used.
     */
#define FUNC_DIY_VOICE                  (0x00000001 << 0)
#define FUNC_LBS_EN                     (0x00000001 << 1)
#define FUNC_STATIC_DOWNLOAD            (0x00000001 << 2)
#define FUNC_3G_EN                      (0x00000001 << 3)
#define FUNC_REMOTE_CONNECT             (0x00000001 << 4)
#define FUNC_REMOTE_NOTIFICATION        (0x00000001 << 5)
#define FUNC_REMOTE_UPGRADE             (0x00000001 << 6)
#define FUNC_REMOTE_REGISTER            (0x00000001 << 7)
#define FUNC_PPP                        (0x00000001 << 8)
#define FUNC_ALL                        (0xffff)

#ifdef	__cplusplus
}
#endif

#endif	/* REG_INFO_H */

