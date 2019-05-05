/**
 * @brief AUTO GENERATED BY `genwrap.pl'. DO NOT EDIT! This is header file
 *  is used by manually written source code for Java bindings.
 *
 * @file exjglue.h
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * AGPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * AGPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License, version 3 as published
 * by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License, version 3
 * for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * -----------------------------------------------------------------------------
 * A commercial use license is available from Mavimax, Ltd
 * contact@mavimax.com
 * -----------------------------------------------------------------------------
 */
 
#ifndef NDRXJ_EXGLUE_H__
#define NDRXJ_EXGLUE_H__

#ifdef  __cplusplus
extern "C" {
#endif

/*---------------------------Includes-----------------------------------*/
#include <jni.h>
#include <ndrx_java_config.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_getAtmiError
        (JNIEnv *, jobject);
extern NDRX_JAVA_API jlong JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpnewctxt
        (JNIEnv *, jclass);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpfreectxt
        (JNIEnv *, jclass, jlong);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpalloc
        (JNIEnv *, jobject, jstring, jstring, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpterm
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_finalizeC
        (JNIEnv *, jclass, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpreturn
        (JNIEnv *, jobject, jint, jlong, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpforward
        (JNIEnv *, jobject, jstring, jobject, jlong);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpRunC
        (JNIEnv *, jobject, jobjectArray, jboolean);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogC
        (JNIEnv *, jobject, jint, jstring, jlong, jstring);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogndrxC
        (JNIEnv *, jobject, jint, jstring, jlong, jstring);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogqinfo
        (JNIEnv *, jobject, jint, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpadvertiseC
        (JNIEnv *, jobject, jstring, jstring);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogubfex
        (JNIEnv *, jobject, jint, jstring, jthrowable);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogndrxex
        (JNIEnv *, jobject, jint, jstring, jthrowable);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogex
        (JNIEnv *, jobject, jint, jstring, jthrowable);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_userlogex
        (JNIEnv *, jobject, jstring, jthrowable);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogDump
        (JNIEnv *, jobject, jint, jstring, jbyteArray);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogDumpDiff
        (JNIEnv *, jobject, jint, jstring, jbyteArray, jbyteArray);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpinit
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_userlogC
        (JNIEnv *, jobject, jstring);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpcall
        (JNIEnv *, jobject, jstring, jobject, jlong);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpacall
        (JNIEnv *, jobject, jstring, jobject, jlong);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpgetrply
        (JNIEnv *, jobject, jint, jobject, jlong);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpconnect
        (JNIEnv *, jobject, jstring, jobject, jlong);
extern NDRX_JAVA_API jlong JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpsend
        (JNIEnv *, jobject, jint, jobject, jlong);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tprecv
        (JNIEnv *, jobject, jint, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpdiscon
        (JNIEnv *, jobject, jint);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpcancel
        (JNIEnv *, jobject, jint);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_Bboolco
        (JNIEnv *, jobject, jstring);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_Bboolpr
        (JNIEnv *, jobject, jobject, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_BboolsetcbfC
        (JNIEnv *, jobject, jstring);
extern NDRX_JAVA_API jstring JNICALL ndrxj_Java_org_endurox_AtmiCtx_Btype
        (JNIEnv *, jobject, jint);
extern NDRX_JAVA_API jstring JNICALL ndrxj_Java_org_endurox_AtmiCtx_Bfname
        (JNIEnv *, jobject, jint);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_AtmiCtx_tptoutget
        (JNIEnv *, jobject);
extern NDRX_JAVA_API jlong JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpurcode
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tptoutset
        (JNIEnv *, jobject, jint);
extern NDRX_JAVA_API jlongArray JNICALL ndrxj_Java_org_endurox_AtmiCtx_getBuffers
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpsetunsolC
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpnotify
        (JNIEnv *, jobject, jobject, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpbroadcast
        (JNIEnv *, jobject, jstring, jstring, jstring, jobject, jlong);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpchkunsol
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpenqueue
        (JNIEnv *, jobject, jstring, jstring, jobject, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpenqueueex
        (JNIEnv *, jobject, jshort, jshort, jstring, jobject, jobject, jlong);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpdequeue
        (JNIEnv *, jobject, jstring, jstring, jobject, jobject, jlong);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpdequeueex
        (JNIEnv *, jobject, jshort, jshort, jstring, jobject, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpopen
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpclose
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpbegin
        (JNIEnv *, jobject, jlong, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpcommit
        (JNIEnv *, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpabort
        (JNIEnv *, jobject, jlong);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpsuspend
        (JNIEnv *, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpresume
        (JNIEnv *, jobject, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_BExprTree_Btreefree
        (JNIEnv *, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedBuffer_tpfree
        (JNIEnv *, jobject, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedBuffer_tprealloc
        (JNIEnv *, jobject, jlong);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_TypedBuffer_tptypes
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedCarray_setBytes
        (JNIEnv *, jobject, jbyteArray);
extern NDRX_JAVA_API jbyteArray JNICALL ndrxj_Java_org_endurox_TypedCarray_getBytes
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedJson_setJSON
        (JNIEnv *, jobject, jstring);
extern NDRX_JAVA_API jstring JNICALL ndrxj_Java_org_endurox_TypedJson_getJSON
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedString_setString
        (JNIEnv *, jobject, jstring);
extern NDRX_JAVA_API jstring JNICALL ndrxj_Java_org_endurox_TypedString_getString
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__IS
        (JNIEnv *, jobject, jint, jshort);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__IJ
        (JNIEnv *, jobject, jint, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__IB
        (JNIEnv *, jobject, jint, jbyte);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__IF
        (JNIEnv *, jobject, jint, jfloat);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__ID
        (JNIEnv *, jobject, jint, jdouble);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__ILjava_lang_String_2
        (JNIEnv *, jobject, jint, jstring);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__I_3B
        (JNIEnv *, jobject, jint, jbyteArray);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bprint
        (JNIEnv *, jobject);
extern NDRX_JAVA_API jshort JNICALL ndrxj_Java_org_endurox_TypedUbf_BgetShort
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API jlong JNICALL ndrxj_Java_org_endurox_TypedUbf_BgetLong
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API jbyte JNICALL ndrxj_Java_org_endurox_TypedUbf_BgetByte
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API jfloat JNICALL ndrxj_Java_org_endurox_TypedUbf_BgetFloat
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API jdouble JNICALL ndrxj_Java_org_endurox_TypedUbf_BgetDouble
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API jstring JNICALL ndrxj_Java_org_endurox_TypedUbf_BgetString
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API jbyteArray JNICALL ndrxj_Java_org_endurox_TypedUbf_BgetByteArr
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__ISLorg_endurox_BFldLocInfo_2
        (JNIEnv *, jobject, jint, jshort, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__IJLorg_endurox_BFldLocInfo_2
        (JNIEnv *, jobject, jint, jlong, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__IBLorg_endurox_BFldLocInfo_2
        (JNIEnv *, jobject, jint, jbyte, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__IFLorg_endurox_BFldLocInfo_2
        (JNIEnv *, jobject, jint, jfloat, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__IDLorg_endurox_BFldLocInfo_2
        (JNIEnv *, jobject, jint, jdouble, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__ILjava_lang_String_2Lorg_endurox_BFldLocInfo_2
        (JNIEnv *, jobject, jint, jstring, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__I_3BLorg_endurox_BFldLocInfo_2
        (JNIEnv *, jobject, jint, jbyteArray, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bchg__IIS
        (JNIEnv *, jobject, jint, jint, jshort);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bchg__IIJ
        (JNIEnv *, jobject, jint, jint, jlong);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bchg__IIB
        (JNIEnv *, jobject, jint, jint, jbyte);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bchg__IIF
        (JNIEnv *, jobject, jint, jint, jfloat);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bchg__IID
        (JNIEnv *, jobject, jint, jint, jdouble);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bchg__IILjava_lang_String_2
        (JNIEnv *, jobject, jint, jint, jstring);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bchg__II_3B
        (JNIEnv *, jobject, jint, jint, jbyteArray);
extern NDRX_JAVA_API jboolean JNICALL ndrxj_Java_org_endurox_TypedUbf_Bboolev
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API jboolean JNICALL ndrxj_Java_org_endurox_TypedUbf_Bqboolev
        (JNIEnv *, jobject, jstring);
extern NDRX_JAVA_API jdouble JNICALL ndrxj_Java_org_endurox_TypedUbf_Bfloatev
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bdel
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bdelall
        (JNIEnv *, jobject, jint);
extern NDRX_JAVA_API jboolean JNICALL ndrxj_Java_org_endurox_TypedUbf_Bpres
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bextread
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API jboolean JNICALL ndrxj_Java_org_endurox_TypedUbf_Bisubf
        (JNIEnv *, jobject);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_TypedUbf_Blen
        (JNIEnv *, jobject, jint, jint);
extern NDRX_JAVA_API jobject JNICALL ndrxj_Java_org_endurox_TypedUbf_Bnext
        (JNIEnv *, jobject, jboolean);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_TypedUbf_Boccur
        (JNIEnv *, jobject, jint);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bproj
        (JNIEnv *, jobject, jintArray);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bread
        (JNIEnv *, jobject, jbyteArray);
extern NDRX_JAVA_API jbyteArray JNICALL ndrxj_Java_org_endurox_TypedUbf_Bwrite
        (JNIEnv *, jobject);
extern NDRX_JAVA_API jlong JNICALL ndrxj_Java_org_endurox_TypedUbf_Bsizeof
        (JNIEnv *, jobject);
extern NDRX_JAVA_API jlong JNICALL ndrxj_Java_org_endurox_TypedUbf_Bunused
        (JNIEnv *, jobject);
extern NDRX_JAVA_API jlong JNICALL ndrxj_Java_org_endurox_TypedUbf_Bused
        (JNIEnv *, jobject);
extern NDRX_JAVA_API jlong JNICALL ndrxj_Java_org_endurox_TypedUbf_Binit
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bdelete
        (JNIEnv *, jobject, jintArray);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bconcat
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bcpy
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bprojcpy
        (JNIEnv *, jobject, jobject, jintArray);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bupdate
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_TypedUbf_Bcmp
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API jboolean JNICALL ndrxj_Java_org_endurox_TypedUbf_Bsubset
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_tpjsontoubf
        (JNIEnv *, jobject, jstring);
extern NDRX_JAVA_API jstring JNICALL ndrxj_Java_org_endurox_TypedUbf_tpubftojson
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_tplogprintubf
        (JNIEnv *, jobject, jint, jstring);
extern NDRX_JAVA_API jint JNICALL ndrxj_Java_org_endurox_TypedUbf_Bnum
        (JNIEnv *, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bjoin
        (JNIEnv *, jobject, jobject);
extern NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bojoin
        (JNIEnv *, jobject, jobject);

#endif  /* NDRXJ_EXGLUE_H__ */

/* vim: set ts=4 sw=4 et smartindent: */
