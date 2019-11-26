#include "client.h"

int flags = 0;

static unsigned char _token_data[8];
str the_token = { 0, _token_data };

#define FLAGS_BLOCK 0x01

static coap_list_t *optlist = NULL;
/* Request URI.
 * TODO: associate the resources with transaction id and make it expireable */
static coap_uri_t uri;

static str payload = { 0, NULL }; /* optional payload to send */

unsigned char msgtype = COAP_MESSAGE_CON; /* usually, requests are sent confirmable */

typedef unsigned char method_t;

//  消息长度有限,1kbit

unsigned char  my_defined_coap_message[1024]; 
// unsigned char  coap_message_from_server[1024];

int my_defined_delay = 0;
int my_defined_interval = 0;

method_t method = 1;		/* the method we are using in our requests */

unsigned int wait_seconds = 90;	/* default timeout in seconds */

coap_tick_t max_wait;		/* global timeout (changed by set_timeout()) */

unsigned int obs_seconds = 30;	/* default observe time */

coap_tick_t obs_wait = 0;	/* timeout for current subscription */

#define min(a,b) ((a) < (b) ? (a) : (b))


coap_pdu_t *
coap_new_request(coap_context_t *ctx, method_t m, coap_list_t *options ) {
  coap_pdu_t *pdu;
  coap_list_t *opt;

  if ( ! ( pdu = coap_new_pdu() ) )
    return NULL;
 
  pdu->hdr->type = msgtype;
  pdu->hdr->id = coap_new_message_id(ctx);
  pdu->hdr->code = m;


  pdu->hdr->token_length = the_token.length;

  if ( !coap_add_token(pdu, the_token.length, the_token.s)) {
      debug("cannot add token to request\n");
  }
  coap_show_pdu(pdu);


  for (opt = options; opt; opt = opt->next) {
     coap_add_option(pdu,
        COAP_OPTION_KEY(*(coap_option *)opt->data),
		    COAP_OPTION_LENGTH(*(coap_option *)opt->data),
		    COAP_OPTION_DATA(*(coap_option *)opt->data));
  }

  if (payload.length) {
    if ((flags & FLAGS_BLOCK) == 0)
      coap_add_data(pdu, payload.length, payload.s);
  }
  return pdu;
}

int findPos(char *str, char *delim){
  int dst_len = strlen(str);
  int delim_len = strlen(delim);
  int i,j;
  int flag ;
  for(i=0;i<dst_len;i++){
    flag = 1;
    int  k = i;
    for(j=0;j<delim_len;j++){
      if(str[k++] != delim[j]){
        flag = 0 ;
        break;
      } 
    }
    if(flag)
      return i;
  }
  return -1;
}


void countDelay(int lastTimeStamp, int last_delay){
     struct timeval tv;
     gettimeofday(&tv,NULL);
     struct tm* pTime;
     pTime = localtime(&tv.tv_sec);
     int current_time = pTime->tm_sec * 1000 + tv.tv_usec / 1000;

     if( current_time < lastTimeStamp ) 
          current_time += 60 * 1000;

      my_defined_interval = current_time - lastTimeStamp ; //ms
      my_defined_delay = current_time - last_delay;
    //  printf("[delay] -------------------------> delay of this packet is : %d\n", my_defined_delay );
}

void parseReturnInfo(unsigned char *buf){
    int pos = findPos(buf,"##");
    char back_info[100];
    int i,j;
    for(i = 0; i < pos;i++){
       back_info[i] = buf[i];
    }

    back_info[pos] = '\0';

    //printf("%s\n", back_info);

    strncpy(my_defined_coap_message, back_info, strlen(back_info) );

   // my_defined_coap_message  =  back_info ;


    int current_time = 0;

    int last_delay = 0;

    for(j = pos + 2;j < strlen(buf)-5 ; j++ ){
        current_time = 10 * current_time + buf[j] - '0';
    }

    for(;j < strlen(buf) ; j++){
      last_delay = 10 * last_delay + buf[j] - '0';
    }
    
    countDelay(current_time, last_delay);

}

void message_handler(struct coap_context_t  *ctx, 
		const coap_address_t *remote, 
		coap_pdu_t *sent,
		coap_pdu_t *received,
		const coap_tid_t id) {
    size_t len;
    unsigned char *databuf;
  if ( received->hdr->code == COAP_RESPONSE_CODE(205) || 
                      received->hdr->code == COAP_RESPONSE_CODE(204)){
      if (coap_get_data(received, &len, &databuf)){
          parseReturnInfo(databuf);

        }
    }
}

void message_handler_new(struct coap_context_t  *ctx, 
    const coap_address_t *remote, 
    coap_pdu_t *sent,
    coap_pdu_t *received,
    const coap_tid_t id) {
    size_t len;
    unsigned char *databuf;
    memset(my_defined_coap_message, '\0', 1024);

  if (received->hdr->code == COAP_RESPONSE_CODE(205) || 
                      received->hdr->code == COAP_RESPONSE_CODE(204)){
      if (coap_get_data(received, &len, &databuf)){
          // printf("sizeof is : %d\n", sizeof(databuf));
          // printf("strlen is : %d\n", strlen(databuf));
          // printf("size_t len is : %d\n", len);
          strncpy(my_defined_coap_message,databuf,len);
      }
    }
}


int
order_opts(void *a, void *b) {
  if (!a || !b)
    return a < b ? -1 : 1;

  if (COAP_OPTION_KEY(*(coap_option *)a) < COAP_OPTION_KEY(*(coap_option *)b))
    return -1;

  return COAP_OPTION_KEY(*(coap_option *)a) == COAP_OPTION_KEY(*(coap_option *)b);
}


coap_list_t *
new_option_node(unsigned short key, unsigned int length, unsigned char *data) {
  coap_option *option;
  coap_list_t *node;

  option = coap_malloc(sizeof(coap_option) + length);
  if ( !option )
    goto error;

  COAP_OPTION_KEY(*option) = key;
  COAP_OPTION_LENGTH(*option) = length;
  memcpy(COAP_OPTION_DATA(*option), data, length);

  /* we can pass NULL here as delete function since option is released automatically  */
  node = coap_new_listnode(option, NULL);

  if ( node )
    return node;

 error:
  perror("new_option_node: malloc");
  coap_free( option );
  return NULL;
}

typedef struct { 
  unsigned char code;
  char *media_type;
} content_type_t;


void cmdline_uri(char *arg) {
  unsigned char portbuf[2];
#define BUFSIZE 40
  unsigned char _buf[BUFSIZE];
  unsigned char *buf = _buf;
  size_t buflen;
  int res;
  		/* split arg into Uri-* options */
    coap_split_uri((unsigned char *)arg, strlen(arg), &uri);

    if (uri.path.length) {
      buflen = BUFSIZE;
      res = coap_split_path(uri.path.s, uri.path.length, buf, &buflen);
      while (res--) {
	      coap_insert(&optlist, new_option_node(COAP_OPTION_URI_PATH,
					      COAP_OPT_LENGTH(buf),
					      COAP_OPT_VALUE(buf)),
		            order_opts);
        buf += COAP_OPT_SIZE(buf);      
      }
    }

    if (uri.query.length) {
      buflen = BUFSIZE;
      buf = _buf;
      res = coap_split_query(uri.query.s, uri.query.length, buf, &buflen);

      while (res--) {
	coap_insert(&optlist, new_option_node(COAP_OPTION_URI_QUERY,
					      COAP_OPT_LENGTH(buf),
					      COAP_OPT_VALUE(buf)),
		    order_opts);

	buf += COAP_OPT_SIZE(buf);      
      }
    }
}


extern int  check_segment(const unsigned char *s, size_t length);
extern void decode_segment(const unsigned char *seg, size_t length, unsigned char *buf);

int cmdline_input(char *text, str *buf) {
  int len;
  len = check_segment((unsigned char *)text, strlen(text));

  if (len < 0)
    return 0;

  buf->s = (unsigned char *)coap_malloc(len);
  if (!buf->s)
    return 0;

  buf->length = len;
  decode_segment((unsigned char *)text, strlen(text), buf->s);
  return 1;
}

void  my_defined_coap_send(char myip[],char myport[],char mypath[], int methodType,char infotext[],char result_info[], int *interval, int *delay) {
  coap_context_t  *ctx = NULL;
  coap_address_t dst,src_addr,dst_addr;
  static char addr[INET6_ADDRSTRLEN];
  void *addrptr = NULL;
  fd_set readfds;
  int result;
  coap_tick_t now;
  coap_queue_t *nextpdu;
  coap_pdu_t  *pdu,*request;
  static str server;
  unsigned short port = COAP_DEFAULT_PORT;
  char port_str[NI_MAXSERV] = "0";
  int res;

  optlist = NULL;
  struct timeval tval = {5,0};
 
  memset(my_defined_coap_message, '\0', 1024);
 
  coap_tid_t tid = COAP_INVALID_TID;
  method = methodType;  // 1 = get;2 = post;3 = put;4 = delete 
  switch(method){
    case 1: {        // get method,deal lock request
        char get_url[100]= "coap://";
        strcat(get_url,myip);
        strcat(get_url,":");
        strcat(get_url,myport);
        strcat(get_url,"/other/lockrequest?node_id=");
        strcat(get_url,infotext);
        int get_len = strlen(get_url);
        get_url[get_len] = '\0';
       // printf("get_url is-----> %s\n", get_url );
        cmdline_uri(get_url);
      }
    break;
    case 3: {        // put method,upload position information;
      if (!cmdline_input(infotext,&payload)){
            payload.length = 0;
        }
        char put_url[100] = "coap://";
        strcat(put_url,myip);
        strcat(put_url,":");
        strcat(put_url,myport);
        strcat(put_url,mypath);
        int put_len = strlen(put_url);
        put_url[put_len] = '\0';
        //printf("put_url is-----> %s\n", put_url );
        cmdline_uri(put_url);
      }
    break;
  }
  


  coap_address_init(&src_addr);
  src_addr.addr.sin.sin_family      = AF_INET;
  src_addr.addr.sin.sin_port        = htons(0);
  src_addr.addr.sin.sin_addr.s_addr = inet_addr("0.0.0.0");
  ctx = coap_new_context(&src_addr);


  coap_address_init(&dst_addr);
  dst_addr.addr.sin.sin_family      = AF_INET;
  dst_addr.addr.sin.sin_port        = htons(atoi(myport));
  dst_addr.addr.sin.sin_addr.s_addr = inet_addr(myip);

 // printf("%s, %s\n", myip, myport);



  coap_register_option(ctx, COAP_OPTION_BLOCK2);

  coap_register_response_handler(ctx, message_handler);


  if (! (request = coap_new_request(ctx, method, optlist)))
       return;


  if (request->hdr->type == COAP_MESSAGE_CON){
       tid = coap_send_confirmed(ctx, &dst_addr, request);
   }

     FD_ZERO(&readfds);
     FD_SET( ctx->sockfd, &readfds );
     /*
     * int select(int nfds,  fd_set* readset,  fd_set* writeset,  fe_set* exceptset,  struct timeval* timeout);
     */
     result = select(ctx->sockfd+1, &readfds, 0, 0, &tval);

     /*
     *   nfds         需要检查的文件描述字个数
     *   readset      用来检查可读性的一组文件描述字。
     *   writeset     用来检查可写性的一组文件描述字
     *   exceptset    用来检查是否有异常条件出现的文件描述字。(注：错误不包括在异常条件之内)
     *   timeout      超时，填NULL为阻塞，填0为非阻塞，其他为一段超时时间
     *   return value 返回fd的总数，错误时返回SOCKET_ERROR
     */

    if(result <= 0){
      ;
    }
    else if ( result > 0 ) {	/* read from socket */
      if ( FD_ISSET( ctx->sockfd, &readfds ) ) 
      {
	     coap_read( ctx );	/* read received data */
	     coap_dispatch( ctx );	/* and dispatch PDUs from receivequeue */
      }
    } 

   coap_free_context( ctx );

   *interval = my_defined_interval;
   *delay = my_defined_delay;
  // printf("message-------->%s\n", my_defined_coap_message);
   strcpy(result_info, my_defined_coap_message);
   return ;
}



//  add by qiufj at 2019-07-17
//  虚拟模式下，请求过快，会导致某些请求丢失

void  coap_send_data_by_get(char myip[],char myport[],char mypath[],char result_info[]) {
  coap_context_t  *ctx = NULL;
  coap_address_t dst,src_addr,dst_addr;
  static char addr[INET6_ADDRSTRLEN];
  void *addrptr = NULL;
  fd_set readfds;
  int result;
  coap_tick_t now;
  coap_queue_t *nextpdu;
  coap_pdu_t  *pdu,*request;
  static str server;
  unsigned short port = COAP_DEFAULT_PORT;
  char port_str[NI_MAXSERV] = "0";
  int res;

  optlist = NULL;
  struct timeval tval = {5,0};
 
  memset(my_defined_coap_message, '\0', 1024 );
 
  coap_tid_t tid = COAP_INVALID_TID;
  method = 1;  // 1 = get;2 = post;3 = put;4 = delete 

  // 构造url
  char get_url[512]= "coap://";
  strcat(get_url,myip);
  strcat(get_url,":");
  strcat(get_url,myport);
  strcat(get_url,"/");
  strcat(get_url,mypath);
  
  int get_len = strlen(get_url);
  get_url[get_len] = '\0';
  // printf("get_url is-----> %s\n", get_url );
  cmdline_uri(get_url);
  // end 

  

  coap_address_init(&src_addr);
  src_addr.addr.sin.sin_family      = AF_INET;
  src_addr.addr.sin.sin_port        = htons(0);
  src_addr.addr.sin.sin_addr.s_addr = inet_addr("0.0.0.0");
  ctx = coap_new_context(&src_addr);


  coap_address_init(&dst_addr);
  dst_addr.addr.sin.sin_family      = AF_INET;
  dst_addr.addr.sin.sin_port        = htons(atoi(myport));
  dst_addr.addr.sin.sin_addr.s_addr = inet_addr(myip);


  coap_register_option(ctx, COAP_OPTION_BLOCK2);

  coap_register_response_handler(ctx, message_handler_new);


  if (! (request = coap_new_request(ctx, method, optlist)))
       return;

  if (request->hdr->type == COAP_MESSAGE_CON){
       tid = coap_send_confirmed(ctx, &dst_addr, request);
   }

     FD_ZERO(&readfds);
     FD_SET( ctx->sockfd, &readfds );
     /*
     * int select(int nfds,  fd_set* readset,  fd_set* writeset,  fe_set* exceptset,  struct timeval* timeout);
     */
     result = select(ctx->sockfd+1, &readfds, 0, 0, &tval);

     /*
     *   nfds         需要检查的文件描述字个数
     *   readset      用来检查可读性的一组文件描述字。
     *   writeset     用来检查可写性的一组文件描述字
     *   exceptset    用来检查是否有异常条件出现的文件描述字。(注：错误不包括在异常条件之内)
     *   timeout      超时，填NULL为阻塞，填0为非阻塞，其他为一段超时时间
     *   return value 返回fd的总数，错误时返回SOCKET_ERROR
     */

    if(result <= 0){
      ;
    }
    else if ( result > 0 ) {  /* read from socket */
      if ( FD_ISSET( ctx->sockfd, &readfds ) ) 
      {
       coap_read( ctx );  /* read received data */
       coap_dispatch( ctx );  /* and dispatch PDUs from receivequeue */
      }
    } 


   coap_free_context( ctx );
   // *interval = my_defined_interval;
   // *delay = my_defined_delay;
   // printf("message-------->%s\n", my_defined_coap_message);
   strcpy(result_info, my_defined_coap_message);
   return ;
}