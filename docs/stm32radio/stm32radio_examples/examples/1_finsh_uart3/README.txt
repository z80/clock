finsh shellʹ��UART3�����ӣ�console���Ҳʹ��UART3������Ҫ��ԭ�д���Ӳ��ģ��Ļ����ϣ���RX��TX�ӵ�PB10��PB11��

Ҫ����Ҫ�м��㣺
1. rtconfig.h�ж���RT_USING_UART3��ʹ�ܴ���3
2. rtconfig.h�ж���finsh device nameΪuart3

3. ��board.c�У���Ҫ��console���豸���õ�uart3 (rt_console_set_device("uart3");)

