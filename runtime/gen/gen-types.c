/* Copyright (C) 2004-2005 Henry Cejtin, Matthew Fluet, Suresh
 *    Jagannathan, and Stephen Weeks.
 *
 * MLton is released under a BSD-style license.
 * See the file MLton-LICENSE for details.
 */

#include "cenv.h"
#include "util.h"

static char* mlTypesHPrefix[] = {
  "/* Copyright (C) 2004-2006 Henry Cejtin, Matthew Fluet, Suresh",
  " *    Jagannathan, and Stephen Weeks.",
  " *",
  " * MLton is released under a BSD-style license.",
  " * See the file MLton-LICENSE for details.",
  " */",
  "",
  "#ifndef _MLTON_MLTYPES_H_",
  "#define _MLTON_MLTYPES_H_",
  "",
  "/* We need these because in header files for exported SML functions, ",
  " * types.h is included without cenv.h.",
  " */",
  "#ifndef _ISOC99_SOURCE",
  "#define _ISOC99_SOURCE",
  "#endif",
  "#if (defined (__OpenBSD__))",
  "#include <inttypes.h>",
  "#elif (defined (__sun__))",
  "#include <sys/int_types.h>",
  "#else",
  "#include <stdint.h>",
  "#endif",
  "",
  NULL
};

static char* cTypesHPrefix[] = {
  "/* Copyright (C) 2004-2006 Henry Cejtin, Matthew Fluet, Suresh",
  " *    Jagannathan, and Stephen Weeks.",
  " *",
  " * MLton is released under a BSD-style license.",
  " * See the file MLton-LICENSE for details.",
  " */",
  "",
  "#ifndef _MLTON_CTYPES_H_",
  "#define _MLTON_CTYPES_H_",
  "",
  NULL
};

static char* cTypesSMLPrefix[] = {
  "(* Copyright (C) 2004-2006 Henry Cejtin, Matthew Fluet, Suresh",
  " *    Jagannathan, and Stephen Weeks.",
  " *",
  " * MLton is released under a BSD-style license.",
  " * See the file MLton-LICENSE for details.",
  " *)",
  "",
  "structure C = struct",
  "",
  NULL
};

static char* mlTypesHStd[] = {
  "/* ML types */",
  "typedef unsigned char* /* uintptr_t */ Pointer;",
  "#define Array(t) Pointer",
  "#define Ref(t) Pointer",
  "#define Vector(t) const Pointer",
  "",
  "typedef int8_t Int8_t;",
  "typedef int8_t Int8;",
  "typedef int16_t Int16_t;",
  "typedef int16_t Int16;",
  "typedef int32_t Int32_t;",
  "typedef int32_t Int32;",
  "typedef int64_t Int64_t;",
  "typedef int64_t Int64;",
  "typedef float Real32_t;",
  "typedef float Real32;",
  "typedef double Real64_t;",
  "typedef double Real64;",
  // "typedef long double Real128_t;",
  // "typedef long double Real128;",
  "typedef uint8_t Word8_t;",
  "typedef uint8_t Word8;",
  "typedef uint16_t Word16_t;",
  "typedef uint16_t Word16;",
  "typedef uint32_t Word32_t;",
  "typedef uint32_t Word32;",
  "typedef uint64_t Word64_t;",
  "typedef uint64_t Word64;",
  "",
  "typedef Int8_t WordS8_t;",
  "typedef Int8_t WordS8;",
  "typedef Int16_t WordS16_t;",
  "typedef Int16_t WordS16;",
  "typedef Int32_t WordS32_t;",
  "typedef Int32_t WordS32;",
  "typedef Int64_t WordS64_t;",
  "typedef Int64_t WordS64;",
  "",
  "typedef Word8_t WordU8_t;",
  "typedef Word8_t WordU8;",
  "typedef Word16_t WordU16_t;",
  "typedef Word16_t WordU16;",
  "typedef Word32_t WordU32_t;",
  "typedef Word32_t WordU32;",
  "typedef Word64_t WordU64_t;",
  "typedef Word64_t WordU64;",
  "",
  "typedef WordS8_t Char8_t;",
  "typedef WordS8_t Char8;",
  "typedef WordS16_t Char16_t;",
  "typedef WordS16_t Char16;",
  "typedef WordS32_t Char32_t;",
  "typedef WordS32_t Char32;",
  "",
  "typedef Vector(Char8_t) String8_t;",
  "typedef Vector(Char8_t) String8;",
  "typedef Vector(Char16_t) String16_t;",
  "typedef Vector(Char16_t) String16;",
  "typedef Vector(Char32_t) String32_t;",
  "typedef Vector(Char32_t) String32;",
  "",
  "typedef Int32_t Bool_t;",
  "typedef Int32_t Bool;",
  // "typedef Char8_t Char_t;",
  // "typedef Char8_t Char;",
  "typedef Int32_t Int_t;",
  "typedef Int32_t Int;",
  // "typedef Real64_t Real_t;",
  // "typedef Real64_t Real;",
  // "typedef String8_t String_t;",
  // "typedef String8_t String;",
  "typedef Word32_t Word_t;",
  "typedef Word32_t Word;",
  ""
  "typedef String8_t NullString8_t;",
  "typedef String8_t NullString8;",
  "typedef Array(NullString8_t) NullString8Array_t;",
  "typedef Array(NullString8_t) NullString8Array;",
  "",
  NULL
};

#define systype(t, bt, name)                        \
  do {                                              \
  char *btLower = strdup(bt);                       \
  btLower[0] = tolower(bt[0]);                      \
  writeString (cTypesHFd, "typedef ");              \
  writeString (cTypesHFd, "/* ");                   \
  writeString (cTypesHFd, #t);                      \
  writeString (cTypesHFd, " */ ");                  \
  writeString (cTypesHFd, bt);                      \
  writeUintmaxU (cTypesHFd, CHAR_BIT * sizeof(t));  \
  writeString (cTypesHFd, "_t ");                   \
  writeString (cTypesHFd, "C_");                    \
  writeString (cTypesHFd, name);                    \
  writeString (cTypesHFd, "_t;");                   \
  writeNewline (cTypesHFd);                         \
  writeString (cTypesSMLFd, "structure ");          \
  writeString (cTypesSMLFd, name);                  \
  writeString (cTypesSMLFd, " = struct open ");     \
  writeString (cTypesSMLFd, bt);                    \
  writeUintmaxU (cTypesSMLFd, CHAR_BIT * sizeof(t));\
  writeString (cTypesSMLFd, " type t = ");          \
  writeString (cTypesSMLFd, btLower);               \
  writeString (cTypesSMLFd, " end");                \
  writeNewline (cTypesSMLFd);                       \
  free (btLower);                                   \
  } while (0)
#define chksystype(t, name)                \
  do {                                     \
  if ((double)((t)(0.25)) > 0)             \
  systype(t, "Real", name);                \
  else if ((double)((t)(-1)) > 0)          \
  systype(t, "Word", name);                \
  else                                     \
  systype(t, "Int", name);                 \
  } while (0)
#define ptrtype(t, name)                            \
  do {                                              \
  writeString (cTypesHFd, "typedef ");              \
  writeString (cTypesHFd, "/* ");                   \
  writeString (cTypesHFd, #t);                      \
  writeString (cTypesHFd, " */ ");                  \
  writeString (cTypesHFd, "Pointer_t ");            \
  writeString (cTypesHFd, "C_");                    \
  writeString (cTypesHFd, name);                    \
  writeString (cTypesHFd, "_t;");                   \
  writeNewline (cTypesHFd);                         \
  writeString (cTypesSMLFd, "structure ");          \
  writeString (cTypesSMLFd, name);                  \
  writeString (cTypesSMLFd, " = Pointer");          \
  writeNewline (cTypesSMLFd);                       \
  } while (0)

#define aliastype(name1, name2)                     \
  do {                                              \
  writeString (cTypesHFd, "typedef ");              \
  writeString (cTypesHFd, "C_");                    \
  writeString (cTypesHFd, name1);                   \
  writeString (cTypesHFd, "_t ");                   \
  writeString (cTypesHFd, "C_");                    \
  writeString (cTypesHFd, name2);                   \
  writeString (cTypesHFd, "_t;");                   \
  writeNewline (cTypesHFd);                         \
  writeString (cTypesSMLFd, "structure ");          \
  writeString (cTypesSMLFd, name2);                 \
  writeString (cTypesSMLFd, " = ");                 \
  writeString (cTypesSMLFd, name1);                 \
  writeNewline (cTypesSMLFd);                       \
  } while (0)

static char* mlTypesHSuffix[] = {
  "",
  "#endif /* _MLTON_MLTYPES_H_ */",
  NULL
};

static char* cTypesHSuffix[] = {
  "",
  "#define C_Errno_t(t) t",
  "",
  "#endif /* _MLTON_CTYPES_H_ */",
  NULL
};

static char* cTypesSMLSuffix[] = {
  "",
  "structure Errno = struct type 'a t = 'a end",
  "end",
  NULL
};

int main (int argc, char* argv[]) {
  int mlTypesHFd, cTypesHFd, cTypesSMLFd;

  mlTypesHFd = open_safe ("ml-types.h", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  for (int i = 0; mlTypesHPrefix[i] != NULL; i++)
    writeStringWithNewline (mlTypesHFd, mlTypesHPrefix[i]);
  for (int i = 0; mlTypesHStd[i] != NULL; i++)
    writeStringWithNewline (mlTypesHFd, mlTypesHStd[i]);
  for (int i = 0; mlTypesHSuffix[i] != NULL; i++)
    writeStringWithNewline (mlTypesHFd, mlTypesHSuffix[i]);

  cTypesHFd= open_safe ("c-types.h", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  cTypesSMLFd = open_safe ("c-types.sml", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

  for (int i = 0; cTypesHPrefix[i] != NULL; i++) 
    writeStringWithNewline (cTypesHFd, cTypesHPrefix[i]);
  for (int i = 0; cTypesSMLPrefix[i] != NULL; i++) 
    writeStringWithNewline (cTypesSMLFd, cTypesSMLPrefix[i]);

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  writeStringWithNewline (cTypesHFd, "/* C */");
  writeStringWithNewline (cTypesSMLFd, "(* C *)");
  chksystype(char, "Char");
  chksystype(signed char, "SChar");
  chksystype(unsigned char, "UChar");
  chksystype(short, "Short");
  chksystype(signed short, "SShort");
  chksystype(unsigned short, "UShort");
  chksystype(int, "Int");
  chksystype(signed int, "SInt");
  chksystype(unsigned int, "UInt");
  chksystype(long, "Long");
  chksystype(signed long, "SLong");
  chksystype(unsigned long, "ULong");
  chksystype(long long, "LongLong");
  chksystype(signed long long, "SLongLong");
  chksystype(unsigned long long, "ULongLong");
  chksystype(float, "Float");
  chksystype(double, "Double");
  // chksystype(long double, "LongDouble");
  chksystype(size_t, "Size");
  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  // systype(void*, "Word", "Pointer");
  ptrtype(char*, "String");
  ptrtype(char**, "StringArray");

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  writeStringWithNewline (cTypesHFd, "/* Generic integers */");
  writeStringWithNewline (cTypesSMLFd, "(* Generic integers *)");
  aliastype("Int", "Fd");
  aliastype("Int", "Signal");
  aliastype("Int", "Status");
  aliastype("Int", "Sock");

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  writeStringWithNewline (cTypesHFd, "/* from <dirent.h> */");
  writeStringWithNewline (cTypesSMLFd, "(* from <dirent.h> *)");
  // ptrtype(DIR*, "DirP");
  systype(DIR*, "Word", "DirP");

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  writeStringWithNewline (cTypesHFd, "/* from <poll.h> */");
  writeStringWithNewline (cTypesSMLFd, "(* from <poll.h> *)");
  chksystype(nfds_t, "NFds");

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  writeStringWithNewline (cTypesHFd, "/* from <resource.h> */");
  writeStringWithNewline (cTypesSMLFd, "(* from <resource.h> *)");
  chksystype(rlim_t, "RLim");

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  writeStringWithNewline (cTypesHFd, "/* from <sys/types.h> */");
  writeStringWithNewline (cTypesSMLFd, "(* from <sys/types.h> *)");
  // chksystype(blkcnt_t, "BlkCnt");
  // chksystype(blksize_t, "BlkSize");
  chksystype(clock_t, "Clock");
  chksystype(dev_t, "Dev");
  chksystype(gid_t, "GId");
  chksystype(id_t, "Id");
  chksystype(ino_t, "INo");
  chksystype(mode_t, "Mode");
  chksystype(nlink_t, "NLink");
  chksystype(off_t, "Off");
  chksystype(pid_t, "PId");
  chksystype(ssize_t, "SSize");
  chksystype(suseconds_t, "SUSeconds");
  chksystype(time_t, "Time");
  chksystype(uid_t, "UId");
  chksystype(useconds_t, "USeconds");

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  writeStringWithNewline (cTypesHFd, "/* from <sys/socket.h> */");
  writeStringWithNewline (cTypesSMLFd, "(* from <sys/socket.h> *)");
  chksystype(socklen_t, "Socklen");

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  writeStringWithNewline (cTypesHFd, "/* from <termios.h> */");
  writeStringWithNewline (cTypesSMLFd, "(* from <termios.h> *)");
  chksystype(cc_t, "CC");
  chksystype(speed_t, "Speed");
  chksystype(tcflag_t, "TCFlag");

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  writeStringWithNewline (cTypesHFd, "/* from \"gmp.h\" */");
  writeStringWithNewline (cTypesSMLFd, "(* from \"gmp.h\" *)");
  chksystype(mp_limb_t, "MPLimb");

  writeNewline (cTypesHFd);writeNewline (cTypesSMLFd);
  for (int i = 0; cTypesHSuffix[i] != NULL; i++) 
    writeStringWithNewline (cTypesHFd, cTypesHSuffix[i]);
  for (int i = 0; cTypesSMLSuffix[i] != NULL; i++) 
    writeStringWithNewline (cTypesSMLFd, cTypesSMLSuffix[i]);

  return 0;
}
