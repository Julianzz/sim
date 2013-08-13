OUTPUT :=poisim

# to build the library , just use "make"
# to clean the library object files, use "make clean"
# to clean all the library files, use "make realclean"
# to build the library file dependency, use "make depend"
# to install the header files, use "make install"

#CPPSOURCE := $(wildcard ./*.h ./*.cpp ../../src/*.h ../../src/*.cpp ../../src/*/*.h ../../src/*/*.cpp ../../src/utils/wordsplit/*.h ../../src/utils/wordsplit/*.cpp ../../src/utils/classify_data/*.h ../../src/utils/classify_data/*.cpp)
BASE_DIR :=../
DEPS_DIR :=../libs/local
CPPSOURCE := $(wildcard ../src/*.cpp ../src/utils/*.cpp 	\
			../src/utils/wordsplit/*.cpp ../src/utils/classify_data/*.cpp ./*.cpp	)
			
HEADERS :=  ../src $(DEPS_DIR)/include ../src/utils/classify_data/  ../libs/api/include

OBJS := $(patsubst %.cpp,%.o,$(CPPSOURCE)) $(patsubst %.c,%.o,$(CSOURCE))

USER_MARCOS := __PTHREADS_MUTEXES__ __linux__ __PTHREADS__ EPOLL _DEBUG \
	 		CONVERT_CHARSET TIXML_USE_STL MEM_POOL NEWS_PROJECT SAVE_NEWS_URL \
			TRIM_INVALID_BLOCK NEED_TRIM_HEAD CHECK_TEMPLATE SAVE_VIDEO_CONTENT VIDEO_PROJECT _WITH_COMBINE_ 

LIBS 		:= pthread  json mutithread storageapi cwxcmn  
#LIBS_PATH	:= ../lib 
LINK_LIBS	:= -L$(DEPS_DIR)/lib $(DEPS_DIR)/lib/libkyotocabinet.a $(DEPS_DIR)/lib/libkyototycoon.a -L../libs/api/lib 
CFLAGS = -O2 -Wall -g
#CFLAGS = -g
CC = g++  

%.o: %.cpp
	$(CC) $(CFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I, $(HEADERS))  -c $< -o $@ 

%.o: %.c 
	$(CC) $(CFLAGS) $(addprefix -D,$(USER_MARCOS)) $(addprefix -I, $(HEADERS))  -c $< -o $@

$(OUTPUT):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(addprefix -L,$(LIBS_PATH)) $(LINK_LIBS) $(addprefix -l,$(LIBS))
 

clean:
	rm -rf $(OBJS)
	rm -rf $(OUTPUT)

realclean : clean
	rm -rf $(OUTPUT)

install:$(OUTPUT)

show:
	@echo $(OBJS)
