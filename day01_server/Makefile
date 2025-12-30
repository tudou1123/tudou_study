# CC := g++

# CFLAGS == -Wall  -g -Wextra -Werror -std=c++17

# #OBJss := server.o client.o

# all: server client

# server:server.o log.o
# 	$(CC) $(CFLAGS) server.cpp log.c -o server

# client:client.o
# 	$(CC) $(CFLAGS) client.cpp -o client

# clean:
# 	rm -f *.o server client

# 编译器
CC := g++
# 编译选项（-c仅用于生成.o文件，可执行文件不需要）
CFLAGS := -Wall -g -Wextra -Werror -std=c++17 -Wno-unused-parameter
# 链接选项（添加std::filesystem所需的库）
LDFLAGS := -lstdc++fs  # 若用GCC9+，可省略此选项

# 目标文件
OBJS := server.o log.o client.o

# 默认目标
all: server client

# 生成server可执行文件（链接阶段，不需要-c）
server: server.o log.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# 生成client可执行文件
client: client.o
	$(CC) $(CFLAGS) $^ -o $@

# 生成.o文件的规则（通用模式规则）
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# 清理目标
clean:
	rm -f $(OBJS) server client