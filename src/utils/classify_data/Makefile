OUTPUT :=classify

CPPSOURCE := $(wildcard *.cpp )
OBJS := $(patsubst %.cpp,%.o,$(CPPSOURCE)) $(patsubst %.c,%.o,$(CSOURCE))

USER_MARCOS := __PTHREADS_MUTEXES__ __linux__ __PTHREADS__ EPOLL _DEBUG CONVERT_CHARSET TIXML_USE_STL MEM_POOL NEWS_PROJECT SAVE_NEWS_URL TRIM_INVALID_BLOCK NEED_TRIM_HEAD CHECK_TEMPLATE SAVE_VIDEO_CONTENT VIDEO_PROJECT _WITH_COMBINE_ 

CFLAGS = -O2 
#CFLAGS = -g 
CC = g++  

%.o: %.cpp
	$(CC) $(CFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I, $(HEADERS))  -c $< -o $@ 

%.o: %.c 
	$(CC) $(CFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I, $(HEADERS))  -c $< -o $@

$(OUTPUT):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(addprefix -L,$(LIBS_PATH)) $(addprefix -l,$(LIBS))
 

clean:
	rm -rf $(OBJS)
	rm -rf $(OUTPUT)

realclean : clean
	rm -rf $(OUTPUT)

install:$(OUTPUT)

show:
	@echo $(OBJS)
