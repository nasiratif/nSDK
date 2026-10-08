#pragma once

#define TRANS_EXPORT_GETMODULETYPE comment(linker, "/EXPORT:GetModuleType=" __FUNCDNAME__ ",@2")
#define TRANS_EXPORT_GETMODULENAME comment(linker, "/EXPORT:GetModuleName=" __FUNCDNAME__ ",@3")
#define TRANS_EXPORT_GETMODULEID comment(linker, "/EXPORT:GetModuleID=" __FUNCDNAME__ ",@4")
#define TRANS_EXPORT_CREATETRANSITION comment(linker, "/EXPORT:CreateTransition=" __FUNCDNAME__ ",@5")
#define TRANS_EXPORT_GETTRANSCOUNT comment(linker, "/EXPORT:GetTransCount=" __FUNCDNAME__ ",@6")
#define TRANS_EXPORT_GETTRANSNAME comment(linker, "/EXPORT:GetTransName=" __FUNCDNAME__ ",@7")
#define TRANS_EXPORT_GETTRANSID comment(linker, "/EXPORT:GetTransID=" __FUNCDNAME__ ",@8")
#define TRANS_EXPORT_GETTRANSMODE comment(linker, "/EXPORT:GetTransMode=" __FUNCDNAME__ ",@9")
#define TRANS_EXPORT_ISUNICODE comment(linker, "/EXPORT:IsUnicode=" __FUNCDNAME__ ",@10")