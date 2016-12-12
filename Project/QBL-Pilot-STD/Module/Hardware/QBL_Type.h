#ifndef QBL_TYPE_H
#define QBL_TYPE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus

extern "C" {

#endif


/**
 * ״̬������
 */
typedef enum {
    QBL_OK = 0, /**< û�д����� */
    QBL_FAIL, /**< �������� */
    QBL_TIMEOUT, /**< �����޶���ִ��ʱ�� */
    QBL_BUSY,  /**< ģ��������æµ״̬ */
    QBL_START_FAILED,  /**< ����ʧ�� */
    QBL_WRITE_ADDR_FAILED, /**< д��ַʧ�� */
    QBL_WRITE_FAILED,   /**< д����ʧ�� */
    QBL_ADDR_NOT_MATCH, /**< ��ַ��ƥ�� */
    QBL_RECEIVE_FAILED, /**< ��ȡ����ʧ�� */
} QBL_STATUS;


#ifdef __cplusplus

}

#endif

#endif // QBL_H
