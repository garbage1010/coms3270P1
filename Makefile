CC = gcc

TARGET = stata
SRCS = pointstat.c pointcloud.c 

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

