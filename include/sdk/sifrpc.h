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
    void *cbuff;
    sceSifEndFunc func;
    void *para;
    struct _sif_serve_data *serve;
} sceSifClientData;

int sceSifBindRpc(sceSifClientData *cd, unsigned int request, unsigned int mode);
int sceSifCallRpc(sceSifClientData *cd, unsigned int fno, unsigned int mode, void *send, int ssize, void *receive,
                  int rsize, sceSifEndFunc end_func, void *end_para);

#endif
