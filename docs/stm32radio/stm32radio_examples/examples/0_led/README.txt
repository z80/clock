����һ���򵥵ĵ�Ƴ���

��ڵط���application.c�ļ���rt_application_init�����У�����������д�����һ����̬�̡߳�

����̵߳������rt_thread_entry_led1������
���������һ���򵥵�while(1)ѭ����
�ȵ����ƣ�
Ȼ���50��OSʱ�ӽ��ĺ�Ϩ��
�ٹ�50��OSʱ�ӽ��ĺ��ٵ���
������

Note Appliation: 
- ������ڵ������߳���ִ�У�

Note Drivers:
- ��Ƶ��õ���rt_hw_led_on/rt_hw_led_off����������1�ǵƵ���ţ���ΪRadio��ֻ��һ���ƣ����������0
- rt_kprintf�������������Ϣ����������console�豸��

Note System:
- RT-Threadϵͳ�������startup.c�ļ��е�main������
- ��main�����У���ΪĬ��Keil MDK�������ж��ǿ����ģ����Ե�һ�����ǹر�ϵͳ���жϡ�
- ϵͳ�����жϻ��ڵ�һ���̱߳�����ִ��ʱ�Զ��򿪣���Ϊ����״̬����ջ�ĳ�ʼ��ʱ�����ϴ��жϣ�
- main�����е���������RT-Threadϵͳ��ں�����rtthread_startup
- rtthread_startup������Ҫ��Ϊ�������֣�
  * rt_hw_board_initӲ����صĳ�ʼ����
  * rt_system_heap_initϵͳ�ѳ�ʼ����������ϵͳ�Ķ�̬�ڴ���䣻
  * rt_application_init�û�Ӧ�ó�ʼ����
  * rt_system_scheduler_start����ϵͳ��������
