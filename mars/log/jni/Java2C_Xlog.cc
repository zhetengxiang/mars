// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.





#include <jni.h>


#include <vector>
#include <string>

#include "mars/comm/xlogger/xlogger.h"
#include "mars/comm/jni/util/scoped_jstring.h"
#include "mars/comm/jni/util/var_cache.h"
#include "mars/comm/jni/util/scope_jenv.h"
#include "mars/comm/jni/util/comm_function.h"

#include "mars/log/appender.h"
#include "mars/log/xlogger_interface.h"

#define LONGTHREADID2INT(a) ((a >> 32)^((a & 0xFFFF)))
DEFINE_FIND_CLASS(KXlog, "com/ybxiang/mars/xlog/Xlog")

#define CHECK_LOG_INSTANCE(log_instance) \
    if (log_instance < 0) return

extern "C" {

DEFINE_FIND_METHOD(KXlog_newXlogInstance, KXlog, "newXlogInstance", "(Lcom/ybxiang/mars/xlog/Xlog$XLogConfig;)J")
JNIEXPORT jlong JNICALL Java_com_ybxiang_mars_xlog_Xlog_newXlogInstance
    (JNIEnv *env, jobject, jobject _log_config) {

    if (NULL == _log_config) {
        xerror2(TSF"logconfig is null");
        return -1;
    }

    jint level = JNU_GetField(env, _log_config, "level", "I").i;
    jint mode = JNU_GetField(env, _log_config, "mode", "I").i;
    jstring logdir = (jstring)JNU_GetField(env, _log_config, "logdir", "Ljava/lang/String;").l;
    jstring nameprefix = (jstring)JNU_GetField(env, _log_config, "nameprefix", "Ljava/lang/String;").l;
    jstring pubkey = (jstring)JNU_GetField(env, _log_config, "pubkey", "Ljava/lang/String;").l;
    jint compressmode = JNU_GetField(env, _log_config, "compressmode", "I").i;
    jint compresslevel = JNU_GetField(env, _log_config, "compresslevel", "I").i;
    jstring cachedir = (jstring)JNU_GetField(env, _log_config, "cachedir", "Ljava/lang/String;").l;
    jint cachedays = JNU_GetField(env, _log_config, "cachedays", "I").i;

    std::string cachedir_str;
    if (NULL != cachedir) {
        ScopedJstring cache_dir_jstr(env, cachedir);
        cachedir_str = cache_dir_jstr.GetChar();
    }

    std::string pubkey_str;
    if (NULL != pubkey) {
        ScopedJstring pubkey_jstr(env, pubkey);
        pubkey_str = pubkey_jstr.GetChar();
    }

    std::string logdir_str;
    if (NULL != logdir) {
        ScopedJstring logdir_jstr(env, logdir);
        logdir_str = logdir_jstr.GetChar();
    }

    std::string nameprefix_str;
    if (NULL != nameprefix) {
        ScopedJstring nameprefix_jstr(env, nameprefix);
        nameprefix_str = nameprefix_jstr.GetChar();
    }

    mars::xlog::XLogConfig config = {(mars::xlog::TAppenderMode)mode, logdir_str, nameprefix_str, pubkey_str,
                                     (mars::xlog::TCompressMode)compressmode, compresslevel, cachedir_str, cachedays};
    mars::comm::XloggerCategory* category = mars::xlog::NewXloggerInstance(config, (TLogLevel)level);
    if (nullptr == category) {
        return -1;
    }
    return reinterpret_cast<uintptr_t>(category);
}

DEFINE_FIND_METHOD(KXlog_getXlogInstance, KXlog, "getXlogInstance", "(Ljava/lang/String;)J")
JNIEXPORT jlong JNICALL Java_com_ybxiang_mars_xlog_Xlog_getXlogInstance
    (JNIEnv *env, jobject, jstring _nameprefix) {
    ScopedJstring nameprefix_jstr(env, _nameprefix);
    mars::comm::XloggerCategory* category = mars::xlog::GetXloggerInstance(nameprefix_jstr.GetChar());
    if (nullptr == category) {
        return -1;
    }
    return reinterpret_cast<uintptr_t>(category);
}

DEFINE_FIND_METHOD(KXlog_releaseXlogInstance, KXlog, "releaseXlogInstance", "(Ljava/lang/String;)V")
JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_releaseXlogInstance
    (JNIEnv *env, jobject, jstring _nameprefix) {
    ScopedJstring nameprefix_jstr(env, _nameprefix);
    mars::xlog::ReleaseXloggerInstance(nameprefix_jstr.GetChar());
}

DEFINE_FIND_STATIC_METHOD(KXlog_appenderOpenWithMultipathWithLevel, KXlog, "appenderOpen", "(Lcom/ybxiang/mars/xlog/Xlog$XLogConfig;)V")
JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_appenderOpen
	(JNIEnv *env, jclass clazz, jobject _log_config) {

    if (NULL == _log_config) {
        xerror2(TSF"logconfig is null");
        return;
    }

    jint level = JNU_GetField(env, _log_config, "level", "I").i;
    jint mode = JNU_GetField(env, _log_config, "mode", "I").i;
    jstring logdir = (jstring)JNU_GetField(env, _log_config, "logdir", "Ljava/lang/String;").l;
    jstring nameprefix = (jstring)JNU_GetField(env, _log_config, "nameprefix", "Ljava/lang/String;").l;
    jstring pubkey = (jstring)JNU_GetField(env, _log_config, "pubkey", "Ljava/lang/String;").l;
    jint compressmode = JNU_GetField(env, _log_config, "compressmode", "I").i;
    jint compresslevel = JNU_GetField(env, _log_config, "compresslevel", "I").i;
    jstring cachedir = (jstring)JNU_GetField(env, _log_config, "cachedir", "Ljava/lang/String;").l;
    jint cachedays = JNU_GetField(env, _log_config, "cachedays", "I").i;

    std::string cachedir_str;
	if (NULL != cachedir) {
        ScopedJstring cache_dir_jstr(env, cachedir);
        cachedir_str = cache_dir_jstr.GetChar();
    }

    std::string pubkey_str;
    if (NULL != pubkey) {
        ScopedJstring pubkey_jstr(env, pubkey);
        pubkey_str = pubkey_jstr.GetChar();
    }

    std::string logdir_str;
    if (NULL != logdir) {
        ScopedJstring logdir_jstr(env, logdir);
        logdir_str = logdir_jstr.GetChar();
    }

    std::string nameprefix_str;
    if (NULL != nameprefix) {
        ScopedJstring nameprefix_jstr(env, nameprefix);
        nameprefix_str = nameprefix_jstr.GetChar();
    }

    mars::xlog::XLogConfig config = {(mars::xlog::TAppenderMode)mode, logdir_str, nameprefix_str, pubkey_str,
                                     (mars::xlog::TCompressMode)compressmode, compresslevel, cachedir_str, cachedays};
    appender_open(config);
	xlogger_SetLevel((TLogLevel)level);
}

JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_appenderClose(JNIEnv *env, jobject) {
    mars::xlog::appender_close();
}

JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_appenderFlush(JNIEnv *env, jobject, jlong _log_instance_ptr, jboolean _is_sync) {
    CHECK_LOG_INSTANCE(_log_instance_ptr);
    mars::xlog::Flush(_log_instance_ptr, _is_sync);
}

DEFINE_FIND_STATIC_METHOD(KXlog_logWrite, KXlog, "logWrite", "(Lcom/ybxiang/mars/xlog/Xlog$XLoggerInfo;Ljava/lang/String;)V")
JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_logWrite
  (JNIEnv *env, jclass, jobject _log_info, jstring _log) {

    if (NULL == _log_info || NULL == _log) {
        xerror2(TSF"loginfo or log is null");
        return;
    }

    jint level = JNU_GetField(env, _log_info, "level", "I").i;

    if (!xlogger_IsEnabledFor((TLogLevel)level)) {
        return;
    }

    jstring tag = (jstring)JNU_GetField(env, _log_info, "tag", "Ljava/lang/String;").l;
    jstring filename = (jstring)JNU_GetField(env, _log_info, "filename", "Ljava/lang/String;").l;
    jstring funcname = (jstring)JNU_GetField(env, _log_info, "funcname", "Ljava/lang/String;").l;
    jint line = JNU_GetField(env, _log_info, "line", "I").i;
    jlong pid = JNU_GetField(env, _log_info, "pid", "J").i;
    jlong tid = JNU_GetField(env, _log_info, "tid", "J").j;
    jlong maintid = JNU_GetField(env, _log_info, "maintid", "J").j;

    XLoggerInfo xlog_info;
    gettimeofday(&xlog_info.timeval, NULL);
    xlog_info.level = (TLogLevel)level;
    xlog_info.line = line;
    xlog_info.pid = (int)pid;
    xlog_info.tid = LONGTHREADID2INT(tid);
    xlog_info.maintid = LONGTHREADID2INT(maintid);;

    ScopedJstring tag_jstr(env, tag);
    ScopedJstring filename_jstr(env, filename);
    ScopedJstring funcname_jstr(env, funcname);
    ScopedJstring log_jst(env, _log);

    xlog_info.tag = tag_jstr.GetChar();
    xlog_info.filename = filename_jstr.GetChar();
    xlog_info.func_name = funcname_jstr.GetChar();

    xlogger_Write(&xlog_info, log_jst.GetChar());

}

DEFINE_FIND_STATIC_METHOD(KXlog_logWrite2, KXlog, "logWrite2", "(JILjava/lang/String;Ljava/lang/String;Ljava/lang/String;IIJJLjava/lang/String;)V")
JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_logWrite2
  (JNIEnv *env, jclass, jlong _log_instance_ptr, int _level, jstring _tag, jstring _filename,
          jstring _funcname, jint _line, jint _pid, jlong _tid, jlong _maintid, jstring _log) {

    CHECK_LOG_INSTANCE(_log_instance_ptr);
    if (!mars::xlog::IsEnabledFor(_log_instance_ptr, (TLogLevel)_level)) {
        return;
    }

    XLoggerInfo xlog_info;
    gettimeofday(&xlog_info.timeval, NULL);
    xlog_info.level = (TLogLevel)_level;
    xlog_info.line = (int)_line;
    xlog_info.pid = (int)_pid;
    xlog_info.tid = LONGTHREADID2INT(_tid);
    xlog_info.maintid = LONGTHREADID2INT(_maintid);

    const char* tag_cstr = NULL;
    const char* filename_cstr = NULL;
    const char* funcname_cstr = NULL;
    const char* log_cstr = NULL;

    if (NULL != _tag) {
        tag_cstr = env->GetStringUTFChars(_tag, NULL);
    }

    if (NULL != _filename) {
        filename_cstr = env->GetStringUTFChars(_filename, NULL);
    }

    if (NULL != _funcname) {
        funcname_cstr = env->GetStringUTFChars(_funcname, NULL);
    }

    if (NULL != _log) {
        log_cstr = env->GetStringUTFChars(_log, NULL);
    }

    xlog_info.tag = NULL == tag_cstr ? "" : tag_cstr;
    xlog_info.filename = NULL == filename_cstr ? "" : filename_cstr;
    xlog_info.func_name = NULL == funcname_cstr ? "" : funcname_cstr;

    mars::xlog::XloggerWrite(_log_instance_ptr, &xlog_info, NULL == log_cstr ? "NULL == log" : log_cstr);

    if (NULL != _tag) {
        env->ReleaseStringUTFChars(_tag, tag_cstr);
    }

    if (NULL != _filename) {
        env->ReleaseStringUTFChars(_filename, filename_cstr);
    }

    if (NULL != _funcname) {
        env->ReleaseStringUTFChars(_funcname, funcname_cstr);
    }

    if (NULL != _log) {
        env->ReleaseStringUTFChars(_log, log_cstr);
    }
}

JNIEXPORT jint JNICALL Java_com_ybxiang_mars_xlog_Xlog_getLogLevel
  (JNIEnv *, jobject, jlong _log_instance_ptr) {
    if (_log_instance_ptr < 0) return kLevelNone;
    return mars::xlog::GetLevel(_log_instance_ptr);
}

//DEFINE_FIND_STATIC_METHOD(KXlog_setLogLevel, KXlog, "setLogLevel", "(I)V")
JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_setLogLevel
  (JNIEnv *, jobject, jlong _log_instance_ptr, jint _log_level) {
    CHECK_LOG_INSTANCE(_log_instance_ptr);
    mars::xlog::SetLevel(_log_instance_ptr, (TLogLevel)_log_level);
}

DEFINE_FIND_METHOD(KXlog_setAppenderMode, KXlog, "setAppenderMode", "(JI)V")
JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_setAppenderMode
  (JNIEnv *, jobject, jlong _log_instance_ptr, jint _mode) {
    CHECK_LOG_INSTANCE(_log_instance_ptr);
    mars::xlog::SetAppenderMode(_log_instance_ptr, (mars::xlog::TAppenderMode)_mode);
}

DEFINE_FIND_METHOD(KXlog_setConsoleLogOpen, KXlog, "setConsoleLogOpen", "(JZ)V")
JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_setConsoleLogOpen
  (JNIEnv *env, jobject, jlong _log_instance_ptr, jboolean _is_open) {
    CHECK_LOG_INSTANCE(_log_instance_ptr);
    mars::xlog::SetConsoleLogOpen(_log_instance_ptr, _is_open);
}

DEFINE_FIND_METHOD(KXlog_setMaxFileSize, KXlog, "setMaxFileSize", "(JJ)V")
JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_setMaxFileSize
        (JNIEnv *env, jobject, jlong _log_instance_ptr, jlong _max_size) {
    CHECK_LOG_INSTANCE(_log_instance_ptr);
    mars::xlog::SetMaxFileSize(_log_instance_ptr, _max_size);
}

DEFINE_FIND_METHOD(KXlog_setMaxAliveTime, KXlog, "setMaxAliveTime", "(JJ)V")
JNIEXPORT void JNICALL Java_com_ybxiang_mars_xlog_Xlog_setMaxAliveTime
        (JNIEnv *env, jobject, jlong _log_instance_ptr, jlong _max_time) {
    CHECK_LOG_INSTANCE(_log_instance_ptr);
    mars::xlog::SetMaxAliveTime(_log_instance_ptr, _max_time);
}
}

void ExportXlog() {}
