/* ʹ����С�޶�APIʵ�ֵ�һ���򵥵�HTTP/1.0������ */
#include <lwip/api.h>
#include <finsh.h>

/*  ����ʵ�ʵ�webҳ�����ݡ��󲿷ֵı������Ὣ��Щ���ݷ���ROM�� */
const static char indexdata[] = "<html> \
	<head><title>A test page</title></head> \
	<body> \
	This is a small test page. \
	</body> \
	</html>";
const static char http_html_hdr[] = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n";

/*  ������������������� */
static void process_connection(struct netconn *conn)
{
	struct netbuf *inbuf;
	char *rq;
	rt_uint16_t len;

	/*  ��������Ӷ�ȡ���ݵ�inbuf�����Ǽٶ������netbuf�а������������� */
	inbuf = netconn_recv(conn);

	/*  ��ȡָ��netbuf�е�һ������Ƭ�ϵ�ָ�룬���������Ƭ��������ϣ������������� */
	netbuf_data(inbuf, (void**)&rq, &len);

	/*  �����������ǲ���HTTP "GET /\r\n"  */
	if( rq[0] == 'G' &&
		rq[1] == 'E' &&
		rq[2] == 'T' &&
		rq[3] == ' ')
	{
		/*  ����ͷ������ */
		netconn_write(conn, http_html_hdr, sizeof(http_html_hdr), NETCONN_NOCOPY);
		/*  ����ʵ�ʵ�webҳ�� */
		netconn_write(conn, indexdata, sizeof(indexdata), NETCONN_NOCOPY);

		/*  �ر����� */
		netconn_close(conn);
	}
	netbuf_delete(inbuf);
}

/* �߳���� */
void lw_thread(void* paramter)
{
	struct netconn *conn, *newconn;

	/*  ����һ���µ�TCP���Ӿ�� */
	conn = netconn_new(NETCONN_TCP);

	/*  �����Ӱ�������ı���IP��ַ��80�˿��� */
	netconn_bind(conn, NULL, 80);

	/*  ���ӽ������״̬ */
	netconn_listen(conn);

	/*  ѭ������ */
	while(1)
	{
		/*  �����µ��������� */
		newconn = netconn_accept(conn);

		/*  ������������ */
		process_connection(newconn);

		/*  ɾ�����Ӿ�� */
		netconn_delete(newconn);
	}
}

void websrv()
{
	rt_thread_t tid;

	tid = rt_thread_create("websrv", lw_thread, RT_NULL,
		1024, 25, 5);
	if (tid != RT_NULL) rt_thread_startup(tid);
}
FINSH_FUNCTION_EXPORT(websrv, startup a simple web server);
