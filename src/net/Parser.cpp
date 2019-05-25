#include <cstdio>
#include <cstdlib>
#include <string.h>
 
#define METHOD_DECODE(mode) mode?"POST":"GET"
 
static char data[] =
"GET /joyent/http-parser.txt HTTP/1.1\r\n"
"Host: github.com\r\n"
"DNT: 1\r\n"
"Accept-Encoding: gzip, deflate, sdch\r\n"
"Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) "
"AppleWebKit/537.36 (KHTML, like Gecko) "
"Chrome/39.0.2171.65 Safari/537.36\r\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,"
"image/webp,*/*;q=0.8\r\n"
"Connection: keep-alive\r\n"
"Transfer-Encoding: chunked\r\n"
"Cache-Control: max-age=0\r\n\r\nb\r\nhello world\r\n0\r\n\r\n";
 
struct http_parser_urt
{
	char filename[32];
	char method; /* 0:GET;1:POST */
};
 
char parse_url(struct http_parser_urt * httpurl, char * strurl)
{
	char ret = 0;
	char * line_start = NULL;
	char * line_end = NULL;
	char * start_temp = NULL;
	char * end_temp = NULL;
	char * line_temp = NULL;
 
 
	/* input check */
	if( (httpurl == NULL)||(strurl == NULL) )
	{
		ret = 1;
		return ret;
	}
 
	/* check http end flag */
	line_temp = (char *)strstr(strurl, "\r\n\r\n");
	if (line_temp == NULL)
	{
		ret = 1;
		return ret;
	}
	
	/* ----------------------------------- first line ------------------------------*/
	/* Determine the first and end addresses of a line */
	line_start = strurl;
	line_end = (char *)strstr(line_start, "\r\n");
	if (line_end == NULL)
	{
		ret = 1;
		return ret;
	}
 
	/* find method name */
	if (strncmp(line_start, "GET ", 4) == 0)
	{
		httpurl->method = 0;
		start_temp = line_start + 4;
	}
	else if (strncmp(strurl, "POST ", 5) == 0)
	{
		httpurl->method = 1;
		start_temp = line_start + 5;
	}
	else
	{
		/* now do not suport method */
	}
	
	/* find file name */
	start_temp++;
	end_temp = (char *)strstr(line_start, " HTTP");
	strncpy(httpurl->filename, start_temp, end_temp-start_temp);
 
	return ret;
}
 
 
 
 
int main(int argc, char * argv[])
{
	char ret1 = 0;
	struct http_parser_urt url_test1;
	ret1 = parse_url(&url_test1, data);
	if (ret1 != 0)
	{
		printf("解析失败...\r\n");
		return 0;
	}
	printf("解析成功..\r\n");
	printf("方法名为:%s\r\n", METHOD_DECODE(url_test1.method));
	printf("文件名为:%s\r\n", url_test1.filename);
	system("pause");
}