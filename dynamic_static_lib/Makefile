# Makefile 변수 설정
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -L.
LDLIBS_STATIC = -lstatic
LDLIBS_DYNAMIC = -ldynamic
RPATH_FLAG = -Wl,-rpath,'$$ORIGIN'

# 기본 타겟 설정
all: executable

# 실행 파일 생성
executable: main.o libstatic.a libdynamic.so
	$(CC) -o $@ main.o $(LDFLAGS) $(LDLIBS_STATIC) $(LDLIBS_DYNAMIC) $(RPATH_FLAG)

# main.c 컴파일
main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

# 정적 라이브러리 생성
libstatic.a: static_library.o
	ar rcs $@ $<

# 정적 라이브러리 컴파일
static_library.o: static_library.c
	$(CC) $(CFLAGS) -c $< -o $@

# 동적 라이브러리 생성
libdynamic.so: dynamic_library.o
	$(CC) -shared -o $@ $<

# 동적 라이브러리 컴파일
dynamic_library.o: dynamic_library.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# clean 타겟 설정
clean:
	rm -f *.o *.a *.so executable
