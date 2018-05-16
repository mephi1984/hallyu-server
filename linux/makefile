#CC=g++-4.7
CC=g++
CFLAGS=-Wall -O3 -DZIP_STATIC -std=c++11
LDFLAGS= -lpthread -lrt -lmysqlcppconn-static -lmysqlclient -lluabind -llua -lssl -lcrypto -ldl\
-L/home/ubuntu/work/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64/lib -L/home/ubuntu/work/mysql-connector-c++-1.1.9-linux-ubuntu16.04-x86-64bit/lib \
-L/home/ubuntu/work/lua-5.3.1/luabind/bin/gcc-5.4.0/release/link-static \
-L/home/ubuntu/work/lua-out/lib \
-L/usr/local/ssl/lib


BOOST_PATH=/home/ubuntu/work/boost_1_67_0

#======================================================
#====================== BOOST =========================
#======================================================

CFLAGS += -I$(BOOST_PATH)

#thread
LOCAL_SRC_FILES := $(BOOST_PATH)/libs/thread/src/pthread/thread.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/thread/src/pthread/once.cpp

#signals
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/signals/src/connection.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/signals/src/named_slot_map.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/signals/src/signal_base.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/signals/src/slot.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/signals/src/trackable.cpp

#system
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/system/src/error_code.cpp

#regex
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/c_regex_traits.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/cpp_regex_traits.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/cregex.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/fileiter.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/icu.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/instances.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/posix_api.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/regex.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/regex_debug.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/regex_raw_buffer.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/regex_traits_defaults.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/static_mutex.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/usinstances.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/w32_regex_traits.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/wc_regex_traits.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/wide_posix_api.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/regex/src/winstances.cpp

#date_time
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/date_time/src/gregorian/greg_month.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/date_time/src/gregorian/greg_weekday.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/date_time/src/gregorian/date_generators.cpp

#program_options
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/cmdline.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/config_file.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/convert.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/options_description.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/parsers.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/positional_options.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/split.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/utf8_codecvt_facet.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/value_semantic.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/variables_map.cpp
LOCAL_SRC_FILES += $(BOOST_PATH)/libs/program_options/src/winmain.cpp

#======================================================
#======================= APP ==========================
#======================================================

CFLAGS += -I../src -I/home/ubuntu/work/mysql-connector-c++-1.1.9-linux-ubuntu16.04-x86-64bit/include \
-I/home/ubuntu/work/lua-out/include \
-I/home/ubuntu/work/lua-5.3.1/luabind \
-I/home/ubuntu/work/mysql-connector-c++-1.1.9-linux-ubuntu16.04-x86-64bit \
-I/usr/local/ssl/include


LOCAL_SRC_FILES += ../src/main.cpp
LOCAL_SRC_FILES += ../src/ImageDownloader.cpp
LOCAL_SRC_FILES += ../src/HangulDictionary.cpp
LOCAL_SRC_FILES += ../src/HangulProcessor.cpp
LOCAL_SRC_FILES += ../src/NumbersProcessor.cpp
LOCAL_SRC_FILES += ../src/inner_base64.cpp
LOCAL_SRC_FILES += ../src/MySqlConnector.cpp
LOCAL_SRC_FILES += ../src/linux_stuff.cpp
LOCAL_SRC_FILES += ../src/LuaHelper.cpp
LOCAL_SRC_FILES += ../src/socket_server.cpp
LOCAL_SRC_FILES += ../src/http_request_handler.cpp
LOCAL_SRC_FILES += ../src/http_connection.cpp
LOCAL_SRC_FILES += ../src/http_request_parser.cpp
LOCAL_SRC_FILES += ../src/http_server.cpp
LOCAL_SRC_FILES += ../src/http_connection_manager.cpp

LOCAL_SRC_FILES += ../src/SE/misc.cpp
LOCAL_SRC_FILES += ../src/SE/Network.cpp
LOCAL_SRC_FILES += ../src/SE/Server.cpp


all:
	$(CC) $(LOCAL_SRC_FILES) $(CFLAGS) $(LDFLAGS) -o hallyu_server
