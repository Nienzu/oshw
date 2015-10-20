
EXEC = server client

make:  
	gcc client.c -o client
	gcc server.c -o server

clean:
	$(RM) $(EXEC) *.o perf.*
