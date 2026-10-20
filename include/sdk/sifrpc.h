/* sifrpc.h (Sony EE SDK): the declarations the library units use. */
#ifndef SDK_SIFRPC_H
#define SDK_SIFRPC_H

typedef void (*sceSifEndFunc)(void *);

struct _sif_rpc_data {
    void *paddr;
    unsigned int pid;
    int tid;
    unsigned int mode;
};

typedef struct _sif_client_data {
    struct _sif_rpc_data rpcd;
    unsigned int command;
    void *buff;
    void *gp;
    sceSifEndFunc func;
    void *para;
    struct _sif_serve_data *serve;
} sceSifClientData;

typedef void *(*sceSifRpcFunc)(unsigned int fno, void *data, int size);

typedef struct _sif_receive_data {
    struct _sif_rpc_data rpcd;
    void *src;
    void *dest;
    int size;
} sceSifReceiveData;

typedef struct _sif_serve_data {
    unsigned int command;
    sceSifRpcFunc func;
    void *buff;
    int size;
    sceSifRpcFunc cfunc;
    void *cbuff;
    int csize;
    sceSifClientData *client;
    void *paddr;
    unsigned int fno;
    void *receive;
    int rsize;
    int rmode;
    unsigned int rid;
    struct _sif_serve_data *link;
    struct _sif_serve_data *next;
    struct _sif_queue_data *base;
} sceSifServeData;

typedef struct _sif_queue_data {
    int key;
    int active;
    struct _sif_serve_data *link;
    struct _sif_serve_data *start;
    struct _sif_serve_data *end;
    struct _sif_queue_data *next;
} sceSifQueueData;

int sceSifBindRpc(sceSifClientData *cd, unsigned int request, unsigned int mode);
int sceSifCallRpc(sceSifClientData *cd, unsigned int fno, unsigned int mode, void *send, int ssize, void *receive,
                  int rsize, sceSifEndFunc end_func, void *end_para);

#endif
