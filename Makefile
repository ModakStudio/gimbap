# 1. 컴파일러 및 플래그 설정
CC = gcc
# -Wall, -Wextra: 경고 메시지 활성화
# -Iinclude: include 폴더 내의 헤더 파일 참조
CFLAGS = -Wall -Wextra -Iinclude
# -lncurses: ncurses 라이브러리 연결
# -lpthread: 시스템 프로그래밍(pthread.h)을 위한 스레드 라이브러리 연결 [2, 3]
LDFLAGS = -lncursesw -lpthread

# 2. 디렉토리 설정
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# 3. 대상 실행 파일 이름
TARGET = $(BIN_DIR)/gimbap

# 4. 소스 및 오브젝트 파일 목록
# src 폴더 내의 모든 .c 파일을 찾아 .o 파일 목록으로 변환
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# 5. 기본 빌드 규칙
all: $(TARGET)

# 6. 실행 파일 생성 규칙
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 7. 오브젝트 파일 생성 규칙
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 8. 유틸리티 규칙
# 생성된 파일 삭제
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# 프로그램 실행
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run