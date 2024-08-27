# 使用するコンパイラを指定
CC = gcc
# コンパイル時に使用するフラグを指定
CFLAGS = -Wall -O2

# # 最適化されない？？？
# CFLAGS = -Wall -O0

# 生成する実行ファイルの名前を指定
TARGET = simplesh
# コンパイルするソースファイルのリストを指定
SRCS = simplesh.c
# ソースファイルから生成されるオブジェクトファイルのリストを指定
OBJS = $(SRCS:.c=.o)
# 依存するヘッダファイルのリストを指定
HEADERS = simplesh.h

# 特殊なターゲット。cleanターゲットを常に実行可能にする
.PHONY: all clean

# デフォルトのターゲット。makeコマンドだけで実行される
all: $(TARGET)

# 実行ファイルを生成するターゲット
$(TARGET): $(OBJS)
	# オブジェクトファイルをリンクして実行ファイルを生成
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# ソースファイルからオブジェクトファイルを生成するルール
%.o: %.c $(HEADERS)
	# ソースファイルをコンパイルしてオブジェクトファイルを生成
	$(CC) $(CFLAGS) -c -o $@ $<

# 生成されたファイルを削除するターゲット
clean:
	# 実行ファイルとオブジェクトファイルを削除
	rm -f $(TARGET) $(OBJS)
