����һ��������finsh shell�Ĺ������ӡ�

��application.c�ļ������������������finsh shell�ϣ�
- fun_a
- int_is

��ϵͳִ����������finsh shell����������ʾ��
finsh>>

������fun_a()��int_is(10)���ɵ�����Ӧ�ĺ�����

Note
----
void int_is(int a)
{
	rt_kprintf("int is: %d\n", a);
}
FINSH_FUNCTION_EXPORT(int_is, show a integer);

���������У�FINSH_FUNCTION_EXPORT�ǹؼ��ĵط�����һ�������Ǻ������������궨����finsh.hͷ�ļ��ж��塣

��finsh shell���ܹ�֧�ֲ��ò����ķ�ʽ����ϵͳ��������ǰ֧��һЩ�������ͣ�����int, long, char, char*�Լ�����ָ�롣

���Ժ��һЩ�����У����һЩ���̺���ֱ�������finsh shell�У���Ҫ����ʱ������ò�ͬ��������ʱ��������finsh shell��ֱ�ӵ�����Щ������

Note Driver
------------
finsh shell������һ�����豸���ؼ��ط���rtthread_startup������
#ifdef RT_USING_FINSH
	/* init finsh */
	finsh_system_init();
	finsh_set_device("uart1");
#endif

�������finsh_set_device������������Ҫ����finsh shell�õ����豸����������Ǵ���1�豸������豸������finsh shell�������豸�й�ϵ��
������޹أ���RT-Threadϵͳ�У�ͳһʹ��rt_kprintf�������������
