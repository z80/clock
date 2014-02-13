#include <rtthread.h>
#include <rtgui/rtgui_server.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/widgets/label.h>
#include <rtgui/widgets/workbench.h>

#include "lcd.h"
#include "key.h"
#include "touch.h"

void workbench_panel1(void* parameter)
{
	rt_mq_t mq;
	rtgui_view_t* view;
	rtgui_label_t* label;
	struct rtgui_workbench* workbench;
	rtgui_rect_t rect;

	mq = rt_mq_create("wmq", 256, 8, RT_IPC_FLAG_FIFO);
	/* ע�ᵱǰ�߳�ΪGUI�߳� */
	rtgui_thread_register(rt_thread_self(), mq);
	/* ����һ������̨ */
	workbench = rtgui_workbench_create("main", "workbench #1");
	if (workbench == RT_NULL) return;

	view = rtgui_view_create("view");
	if (view == RT_NULL) return;
	/* ָ����ͼ�ı���ɫ */
	RTGUI_WIDGET_BACKGROUND(RTGUI_WIDGET(view)) = white;

	/* ���һ��label */
	label = rtgui_label_create("��ã�RT-Thread��");
	rect.x1 = 10; rect.y1 = 10;
	rect.x2 = 210; rect.y2 = 30;
	/* ����label��λ�� */
	rtgui_widget_set_rect(RTGUI_WIDGET(label), &rect);
	rtgui_container_add_child(RTGUI_CONTAINER(view), RTGUI_WIDGET(label));

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

/* ��ʼ��workbench */
void workbench_init()
{
	rt_thread_t tid;

	tid = rt_thread_create("wb", workbench_panel1, RT_NULL, 2048, 20, 5);
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

    /* ע����� */
    rect.x1 = 0;
    rect.y1 = 0;
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

		/* ��ʼ��workbench */
		workbench_init();
	}
}
