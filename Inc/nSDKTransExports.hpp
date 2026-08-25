#pragma once

#define EXT_EXPORT_GETMODULETYPE comment(linker, "/EXPORT:GetModuleType=" __FUNCDNAME__ ",@2")
#define EXT_EXPORT_GETMODULENAME comment(linker, "/EXPORT:GetModuleName=" __FUNCDNAME__ ",@3")
#define EXT_EXPORT_GETMODULEID comment(linker, "/EXPORT:GetModuleID=" __FUNCDNAME__ ",@4")
#define EXT_EXPORT_CREATETRANSITION comment(linker, "/EXPORT:CreateTransition=" __FUNCDNAME__ ",@5")
#define EXT_EXPORT_GETTRANSCOUNT comment(linker, "/EXPORT:GetTransCount=" __FUNCDNAME__ ",@6")
#define EXT_EXPORT_GETTRANSNAME comment(linker, "/EXPORT:GetTransName=" __FUNCDNAME__ ",@7")
#define EXT_EXPORT_GETTRANSID comment(linker, "/EXPORT:GetTransID=" __FUNCDNAME__ ",@8")
#define EXT_EXPORT_GETTRANSMODE comment(linker, "/EXPORT:GetTransMode=" __FUNCDNAME__ ",@9")
#define EXT_EXPORT_ISUNICODE comment(linker, "/EXPORT:IsUnicode=" __FUNCDNAME__ ",@10")