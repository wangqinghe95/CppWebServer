server:
	g++ server.cpp util.cpp Epoll.cpp InetAddress.cpp Socket.cpp -o server -g && \
	g++ client.cpp util.cpp -o client

clean:
	rm server client