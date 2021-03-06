#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define MAX_BUFFER_SIZE 1024

#define GET_REQUEST_LEN 32

#define TEST_LEN ( ( (MAX_BUFFER_SIZE)*(2) )+ (GET_REQUEST_LEN) )

void get_params(char const * uri,char ** hostname,char ** pathname)
{
	static char host[MAX_BUFFER_SIZE];

	static char url[MAX_BUFFER_SIZE];

	static char * path_slash = NULL;

	strncat(url,uri,MAX_BUFFER_SIZE-strlen(url)-1);


	if ( strstr(url,"//") != NULL )
	{ 
		strncat(*hostname,&url[abs(strstr(url,"//")-&url[0]) + 2],MAX_BUFFER_SIZE - strlen(*hostname)-1);


		
	}

	else
	{
		strncat(*hostname,&url[0],MAX_BUFFER_SIZE-strlen(*hostname)-1);

		
	}
	

	if (strstr(*hostname,"/") != NULL)	
	{	
		
		strncat(*pathname,&url[abs( strstr(*hostname,"/")  - &url[0] ) + 1],MAX_BUFFER_SIZE-strlen(*pathname)-1);
	}

	else
	{
		*pathname[0] = '\0';	
			
	}

// separating hostname from pathname
	
	if ( strstr(*hostname,"/") != NULL )
	{
		*strstr(*hostname,"/") = '\0';
	}

}

int main(int argc, char ** argv)
{
	if ( argc < 2)
	{
		fprintf(stderr,"%d: ",__LINE__);

		fprintf(stderr,"Usage: %s: [-p http://[username:password@]\
				proxy-host:proxy-port] <URL>\n",argv[0]);


		exit(EXIT_FAILURE);
	}	
	
	static struct addrinfo hints, *res = NULL, *p = NULL; 

	int sockd = 0;

	int proxy_port = 0;

	static char msg[MAX_BUFFER_SIZE];

	static char test[1024*2+32];

	static char host[MAX_BUFFER_SIZE];
	
	static char * host_p = &host[0];

	static char path[MAX_BUFFER_SIZE];

	static char * path_p = &path[0];

	static char proxy_host[MAX_BUFFER_SIZE];

	static char * proxy_host_p = &proxy_host[0];

	static char proxy_port[MAX_BUFFER_SIZE];

	static char * proxy_port_p = &proxy_host[0];

	static char proxy_passwd[MAX_BUFFER_SIZE];

	static char * proxy_passwd_p = &proxy_passwd[0];

	static char proxy_user[MAX_BUFFER_SIZE];

	static char * proxy_user_p= &proxy_passwd[0];


	if ( strstr(argv[1],"-p") != NULL )
	{ 
		if ( proxy_params(argv[2],&proxy_host_p,&proxy_port_p,&proxy_user_p,&proxy_passwd_p) == 0 )
		{
			fprintf(stderr,"%d: Error - malformed proxy parameter: %s.\n",__LINE__,argv[2]);

			exit(EXIT_FAILURE);
		}
	}

	else
	{	
		if ( get_params(argv[1],&host_p,&path_p) == 0 )
		{
			fprintf(stderr,"%d: Error - malformed URL: %s.\n",__LINE__,argv[1]);

			exit(EXIT_FAILURE);
			
		}
	}

	if ( proxy_host_p != NULL )
	{
		printf("Proxy-host:%s\nPath:%s\n",proxy_host,proxy_path);
	}
	
	else
	{
		printf("Host:%s\nPath:%s\n",host,path);
	}

	
	int recv_bytes = 0, sent_bytes = 0, gstrerror = 0;

	memset(&hints,0,sizeof(hints));

	hints.ai_family = AF_UNSPEC;

	hints.ai_socktype = SOCK_STREAM;

	if ( proxy_host != NULL )
	{

		if ( (gstrerror = getaddrinfo(proxy_host,"http",&hints,&res) ) != 0)
		{
			fprintf(stderr,"%d: getaddrinfo(): %s\n",__LINE__,gai_strerror(gstrerror));
			exit(EXIT_FAILURE);

		}

	}
	
	else
	{	

		if ( (gstrerror = getaddrinfo(host,"http",&hints,&res) ) != 0)
		{
			fprintf(stderr,"%d: getaddrinfo(): %s\n",__LINE__,gai_strerror(gstrerror));
			exit(EXIT_FAILURE);

		}

	}

	for ( p = res; p!= NULL; p = p->ai_next)
	{
		if ( (p->ai_family == AF_INET) && ((sockd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) != -1) )
		{
			if (connect(sockd,(struct sockaddr *)p->ai_addr,p->ai_addrlen) != -1)
			{ res = p; break; }	
			
		}

	}
	
	if ( p == NULL)
	{
		fprintf(stderr,"%d: ",__LINE__);
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	printf("Data Sent and Received\n");
	

// path remains the same regardless of whether requesting from a proxy server

	snprintf(test,MAX_BUFFER_SIZE+GET_REQUEST_LEN+1,"GET /%s HTTP/1.1\r\n\0",path);


	if ( (sent_bytes = send(sockd,test,strlen(test),0)) == -1)
	{
		fprintf(stderr,"%d: ",__LINE__);

		perror("send");
		
		exit(EXIT_FAILURE);
	}
	
	snprintf(test,TEST_LEN,"Host: %s\r\n\0",host);

	if ( (sent_bytes = send(sockd,test,strlen(test),0)) == -1)
	{
		fprintf(stderr,"%d: ",__LINE__);

		perror("send");
		
		exit(EXIT_FAILURE);
	}
	
	snprintf(test,TEST_LEN,"Connection: close\r\n\r\n\0");

	if ( (sent_bytes = send(sockd,test,strlen(test),0)) == -1)
	{
		fprintf(stderr,"%d: ",__LINE__);

		perror("send");
		
		exit(EXIT_FAILURE);
	}
	printf("%s",recv_bytes,msg);

	while ( ( ( recv_bytes = recv(sockd,msg,sizeof(msg),0) )  > 0 ) 
		&& (msg[0] != '\0') )
	{
		printf("%s",msg);
		
		memset(msg,0,sizeof(msg));	
	}


	close(sockd);
		
	freeaddrinfo(res);
	
	return 0;
}
