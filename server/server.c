#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>

int listener_d;

int read_in(int socket, char *buf, int len);
void handle_shutdown(int sig);

int main(){
  listener_d = socket(PF_INET, SOCK_STREAM, 0);
  signal(SIGINT, handle_shutdown);
  if(listener_d == -1){
    puts("Erro no socket");
    exit(1);
  }
  struct sockaddr_in name;
  name.sin_family = PF_INET;
  name.sin_port = (in_port_t) htons(30000);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  int reuse = 1; 
  
  if(setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse,sizeof(int)) == -1){
    puts("Erro no socket");
    exit(1);
  }

  int c = bind(listener_d, (struct sockaddr *) &name, sizeof(name));

  if(c == -1){
    puts("Erro no socket");
    exit(1);
  }
  if(listen(listener_d, 10) == -1){
    puts("Erro no socket");
    exit(1);
  }
  puts("Waiting for connection!!!!!!!!!!!");
  char buf[255];
  
  struct sockaddr_storage client_addr;
  unsigned int address_size = sizeof(client_addr);
  while(1){
      int connect_d = accept(listener_d, (struct sockaddr *) &client_addr, &address_size);
    if(connect_d == -1){
      puts("Erro no socket");
      exit(1);
    }
    char *logo =  "                _                       \n                \\`*-.                   \n                 )  _`-.                \n                .  : `. .               \n                : _   '  \\              \n                ; *` _.   `*-._         \n                `-.-'          `-.      \n                  ;       `       `.    \n                  :.       .        \\   \n                  . \\  .   :   .-'   .  \n                  '  `+.;  ;  '      :  \n                  :  '  |    ;       ;-.\n                  ; '   : :`-:     _.`* ;\n       le chat .*' /  .*' ; .*`-/ +'  `*'\n               `*-*   `*-*  `*-'       by @vlopess using Vim\n______________________________________________________\n";
    send(connect_d, logo, strlen(logo) , 0);
    char *txt = "Enter username:";
    send(connect_d, txt, sizeof(txt) * 2, 0);
    char username[255];
    read_in(connect_d, username, sizeof(username));
    size_t username_len = strlen(username);  
    strcat(username, "said:");
    printf("%s", username);
    while(1){ 
	 char buffer[255];	 
         read_in(connect_d, buf, sizeof(buf));         	 		 
	 printf("%s", username);         
     	 printf("%s\n", buf);
    }    
    close(connect_d);
  }  
  return 0;
}

int read_in(int socket, char *buf, int len){
  char *s = buf;
  int slen = len;
  int c = recv(socket, s, slen, 0);
  while((c > 0) && (s[c-1] != '\n')){
    s += c ;
    c = recv(socket, s, slen, 0);      
  }
  if(c < 0)
    return 0;
  else if (c == 0)
    buf[0] = '\0';
  else
    s[c-1] = '\0';
  return  len - slen;
}

void handle_shutdown(int sig){
   if(listener_d)
     close(listener_d);
   puts("\nBye Bye!");
   exit(0);
}

