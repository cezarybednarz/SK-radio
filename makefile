EXECS = radio-proxy

CPPFLAGS = -Wall
LDFLAGS	= 
LDLIBS = 

TARGET: $(EXECS)

radio-proxy.o err.o: radio-proxy.h err.h

radio-proxy: err.o


.PHONY: clean TARGET
clean:
	rm -f $(EXECS) *.o *~ 
