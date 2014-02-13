#include <finsh.h>
#include <dfs_posix.h>
#include "stm32f10x.h"

//����mempool���С.
#define  mempll_block_size      16384
//���ǹ���������mempool,������4�ֽ���Ϊ���ƿ�.
static rt_uint8_t mempool[ (mempll_block_size+4) *2];
static struct rt_mempool _mp;
//�ڴ�س�ʼ����ʶ
static rt_bool_t is_inited = RT_FALSE;

static rt_err_t wav_tx_done(rt_device_t dev, void *buffer)
{
    /* release memory block */
    rt_mp_free(buffer);

    return RT_EOK;
}

void wav(char* filename)
{
    int fd;

    //���mempool�Ƿ񱻳�ʼ��,������г�ʼ��.
    if (is_inited == RT_FALSE)
    {
        rt_mp_init(&_mp, "wav_buf", &mempool[0], sizeof(mempool), mempll_block_size);
        is_inited = RT_TRUE;
    }


    //���ļ�
    fd = open(filename, O_RDONLY, 0);
    if (fd >= 0)
    {
        rt_uint8_t* buf;
        rt_size_t 	len;
        rt_device_t device;

        /* open audio device and set tx done call back */
        device = rt_device_find("snd");
        //���÷�����ɻص�����,��DAC���ݷ���ʱִ��wav_tx_done�����ͷſռ�.
        rt_device_set_tx_complete(device, wav_tx_done);
        rt_device_open(device, RT_DEVICE_OFLAG_WRONLY);

        do
        {
            //��mempoll����ռ�,������벻�ɹ���һֱ�ڴ˵ȴ�.
            buf = rt_mp_alloc(&_mp, RT_WAITING_FOREVER);
            //���ļ���ȡ����
            len = read(fd, (char*)buf, mempll_block_size);
            //��ȡ�ɹ��Ͱ�����д���豸
            if (len > 0)
            {
                rt_device_write(device, 0, buf, len);
            }
            //�����ͷŸղ�����Ŀռ�,����������Ƕ����ļ�βʱ.
            else
            {
                rt_mp_free(buf);
            }
        }
        while (len != 0);

        /* close device and file */
        rt_device_close(device);
        close(fd);
    }
}
FINSH_FUNCTION_EXPORT(wav, wav test. e.g: wav("/test.wav"))

