all:
	gcc fd.c -o fd
	gcc pipe.c -o pipe
	gcc named_pipe.c -o named_pipe
	gcc mq_client.c -o mq_client -lrt
	gcc mq_server.c -o mq_server -lrt
	gcc shm_server.c -o shm_server -lrt
	gcc shm_drinker.c -o shm_drinker -lrt
	gcc socket_client.c -o socket_client
	gcc socket_server.c -o socket_server
	gcc socket_udp_client.c -o socket_udp_client
	gcc socket_udp_server.c -o socket_udp_server
	gcc mult.c -o mult

clean:
	rm -f fd pipe named_ pipe mq_server mq_client shm_server \
		shm_drinker socke t_server socket_client \
		socket_udp_client socket_udp_server mult \
		named_pipe socket_server
