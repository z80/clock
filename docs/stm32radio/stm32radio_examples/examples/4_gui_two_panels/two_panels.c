#include <rtthread.h>
#include <rtgui/rtgui_server.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/widgets/workbench.h>

#include "lcd.h"
#include "key.h"
#include "touch.h"

void workbench_panel1(void* parameter)
{
	rt_mq_t mq;
	rtgui_view_t* view;
	struct rtgui_workbench* workbench;

	mq = rt_mq_create("wmq1", 256, 8, RT_IPC_FLAG_FIFO);
	/* ע�ᵱǰ�߳�ΪGUI�߳� */
	rtgui_thread_register(rt_thread_self(), mq);
	/* ����һ������̨ */
	workbench = rtgui_workbench_create("info", "workbench #1");
	if (workbench == RT_NULL) return;

	view = rtgui_view_create("view1");
	if (view == RT_NULL) return;
	/* ָ����ͼ�ı���ɫ */
	RTGUI_WIDGET_BACKGROUND(RTGUI_WIDGET(view)) = white;

	/* ��ӵ���workbench�� */
	rtgui_workbench_add_view(workbench, view);
	/* ��ģʽ��ʽ��ʾ��ͼ */
	rtgui_view_show(view, RT_FALSE);

	/* ִ�й���̨�¼�ѭ�� */
	rtgui_workbench_event_loop(workbench);

	/* ȥע��GUI�߳� */
	rtgui_thread_deregister(rt_thread_self());

	/* delete message queue */
	rt_mq_delete(mq);
}

void workbench_panel2(void* parameter)
{
	rt_mq_t mq;
	rtgui_view_t* view;
	struct rtgui_workbench* workbench;

	mq = rt_mq_create("wmq2", 256, 8, RT_IPC_FLAG_FIFO);
	/* ע�ᵱǰ�߳�ΪGUI�߳� */
	rtgui_thread_register(rt_thread_self(), mq);
	/* ����һ������̨ */
	workbench = rtgui_workbench_create("main", "workbench #2");
	if (workbench == RT_NULL) return;

	view = rtgui_view_create("view2");
	if (view == RT_NULL) return ;
	/* ָ����ͼ�ı���ɫ */
	RTGUI_WIDGET_BACKGROUND(RTGUI_WIDGET(view)) = green;
	/* ��ӵ���workbench�� */
	rtgui_workbench_add_view(workbench, view);
	/* ��ģʽ��ʽ��ʾ��ͼ */
	rtgui_view_show(view, RT_FALSE);

	/* ִ�й���̨�¼�ѭ�� */
	rtgui_workbench_event_loop(workbench);
	/* ȥע��GUI�߳� */
	rtgui_thread_deregister(rt_thread_self());
	rt_mq_delete(mq);
}

/* ��ʼ������workbench���ֱ�λ��panel1��panel2 */
void workbench_init()
{
	rt_thread_t tid;

	tid = rt_thread_create("wb1", workbench_panel1, RT_NULL, 2048, 20, 5);
	if (tid != RT_NULL) rt_thread_startup(tid);

	tid = rt_thread_create("wb2", workbench_panel2, RT_NULL, 2048, 20, 5);
	if (tid != RT_NULL) rt_thread_startup(tid);
}

void gui_init()
{
 	extern void rtgui_touch_hw_init(void);
	extern rt_err_t load_setup(void);
    rtgui_rect_t rect;
	rt_device_t lcd;

	/* ��ʼ��LCD���� */
    rt_hw_lcd_init();
	lcd = rt_device_find("lcd");
	if (lcd != RT_NULL)
	{
		rt_device_init(lcd);
		rtgui_graphic_set_device(lcd);

		/* ��ʼ��RT-Thread/GUI server */
	    rtgui_system_server_init();

    /* ע�����1 */
    rect.x1 = 0;
    rect.y1 = 0;
    rect.x2 = 240;
    rect.y2 = 25;
    rtgui_panel_register("info", &rect);

	/* ע�����2 */
    rect.x1 = 0;
    rect.y1 = 25;
    rect.x2 = 240;
    rect.y2 = 320;
    rtgui_panel_register("main", &rect);
    rtgui_panel_set_default_focused("main");

	/* ��ʼ���������� */
	rt_hw_key_init();

	/* ��ʼ������������ */
	load_setup(); //touchװ��Ĭ��ֵ
	rtgui_touch_hw_init();	
    rt_device_init_all();

	/* ��ʼ��workbenchӦ�� */
	workbench_init();
	}
}
