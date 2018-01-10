TARGET = aeProfileMaker

SOURCE_FILES = translater_main.cpp \
               roland_cmd.cpp \
               wsd_api.cpp \
               xml_profile.cpp \
               debug_config.cpp \
               pugixml/pugixml.cpp

STATIC_LIB =

OBJ = $(SOURCE_FILES:.cpp=.o)

CC = g++
CFLAGS = -Wall -O
LDFLAGS =

$(TARGET):$(OBJ) $(STATIC_LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJ) $(STATIC_LIB)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(SOURCE_FILES:.cpp=.d)

.PHONY : auto
.PHONY : test
auto : $(TARGET) test clean
auto : $(TARGET) test clean

test:
	./$(TARGET) -i roland.txt -o profile

.PHONY : clean
clean:
	-rm -f $(TARGET) *.d *.o
