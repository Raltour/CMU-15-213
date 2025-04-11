#include <stdio.h>
#include <string.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *hostname, char *path, char *server_port);
void get_filetype(char *filename, char *filetype);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
int connect_to_server(char *buf, char *host, char *port);
void buil_request(char *buf, char *method, char *hostname, char *path);


int main(int argc, char **argv)
{
    printf("%s", user_agent_hdr);

    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        doit(connfd);
        Close(connfd);
    }
}


void doit(int fd)
{
    char buf[MAXBUF], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE];
    char path[MAXLINE];
    rio_t rio;
    char server_port[MAXLINE];
    //不太清楚这里的端口号怎么获取，可能是自己指定的？

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    if (strcmp(method, "GET") != 0) {
        clienterror(fd, method, "501", "Not Implemented",
                    "Server does not implement this method");
        return;
    }
    read_requesthdrs(&rio);

    /* Parse URI from GET request */
    if (parse_uri(uri, hostname, path, server_port) >= 0) {
        buil_request(buf, method, hostname, path);
        if (connect_to_server(buf, hostname, server_port)) {
            Rio_writen(fd, buf, strlen(buf));
        } else {
            printf("Connection to server failed\n");
        }
    } else {
        printf("Request is not valid");
    }
}


/*
 * parse_uri - parse URI to the hostname, path to the file
 * return whether the request is valid
 * for example: http://www.cmu.edu/hub/index.html or /hub/index.html
 */
int parse_uri(char *uri, char *hostname, char *path, char *server_port) {
    if (strncmp(uri, "http://", strlen("http://")) == 0) {
        uri += strlen("http://");
        char *ptr1 = strchr(uri, ':');
        char *ptr2 = strchr(uri, '/');

        if (ptr2 != NULL) {
            if (ptr1 == NULL) {
                strncpy(hostname, uri, ptr2 - uri);
                strcpy(server_port, "4500");
            } else {
                strncpy(server_port, ptr1 + 1, ptr2 - (ptr1 + 1));
                strncpy(hostname, ptr1, ptr1 - uri);
            }
            strncpy(path, ptr2, MAXLINE);
        } else {
            return -1;
        }
    } else {
        strcpy(hostname, "localhost");
    }

    return 1;
}


int connect_to_server(char *buf, char *host, char *server_port) {
    int clientfd;
    rio_t rio;

    if ((clientfd = Open_clientfd(host, server_port)) < 0) {
        return 0;
    }
    Rio_readinitb(&rio, clientfd);

    Rio_writen(clientfd, buf, strlen(buf));
    Rio_readn(clientfd, buf, MAXBUF);

    Close(clientfd);
    return 1;
}


/*
 * clienterror - returns an error message to the client
 */
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) {
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}


/*
 * read_requesthdrs - read HTTP request headers
 */
void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    while(strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
}


void buil_request(char *buf, char *method, char *hostname, char *path) {
    sprintf(buf, "GET %s HTTP/1.0\r\n", path);
    sprintf(buf + strlen(buf), "Host: %s\r\n", hostname);
    sprintf(buf + strlen(buf), "%s", user_agent_hdr);
    sprintf(buf + strlen(buf), "Connection: close\r\n");
    sprintf(buf + strlen(buf), "Proxy-Connection: close\r\n");
    strcat(buf, "\r\n");
}
