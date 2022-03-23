objects=csapp_alter.o client_error.o parse_uri.o fill_http_request.o service_dynamic.o service_static.o doit.o sbuf.o start.o

create:$(objects)
	gcc -o tinywebd.out $(objects) -lpthread

client_error.o:tinyweb.h client_error.c
	gcc -c client_error.c

parse_uri.o:parse_uri.c
	gcc -c parse_uri.c

fill_http_request.o:fill_http_request.c
	gcc -c fill_http_request.c

service_static.o:tinyweb.h service_static.c
	gcc -c service_static.c

service_dynamic.o:tinyweb.h service_dynamic.c
	gcc -c service_dynamic.c

doit.o:tinyweb.h doit.c
	gcc -c doit.c

start.o:sbuf.h tinyweb.h start.c
	gcc -c start.c

sbuf.o:sbuf.h sbuf.c
	gcc -c sbuf.c
	
csapp_alter.o:csapp_alter.h
	gcc -c csapp_alter.c

clean:
	rm -f $(objects)
