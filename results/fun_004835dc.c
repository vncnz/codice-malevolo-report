
void FUN_004835dc(undefined *param_1,undefined *param_2)

{
  BYTE *lpData;
  LPCSTR lpValueName;
  ulong cbData;
  undefined4 *in_FS_OFFSET;
  DWORD Reserved;
  DWORD dwType;
  undefined4 uStack52;
  undefined *puStack48;
  undefined *puStack44;
  undefined4 uStack40;
  undefined *puStack36;
  undefined *puStack32;
  HKEY run_regkey;
  undefined *local_c;
  undefined *local_8;
  
  puStack44 = &stack0xfffffffc;
  puStack32 = (undefined *)0x4835f3;
  local_c = param_2;
  local_8 = param_1;
  FUN_004059cc((int)param_1);
  puStack32 = (undefined *)0x4835fb;
  FUN_004059cc((int)local_c);
  puStack36 = &DAT_00483696;
  uStack40 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack40;
  puStack48 = &DAT_0048366a;
  uStack52 = *in_FS_OFFSET;
  *in_FS_OFFSET = &uStack52;
  puStack32 = &stack0xfffffffc;
  RegOpenKeyA((HKEY)0x80000001,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&run_regkey);
  cbData = (ulong)local_c;
  if (local_c != (undefined *)0x0) {
    cbData = *(ulong *)(local_c + -4);
  }
  cbData = cbData + 1;
  lpData = FUN_004059dc(local_c);
  dwType = 1;
  Reserved = 0;
  lpValueName = FUN_004059dc(local_8);
  RegSetValueExA(run_regkey,lpValueName,Reserved,dwType,lpData,cbData);
  RegCloseKey(run_regkey);
  *in_FS_OFFSET = uStack52;
  *in_FS_OFFSET = uStack40;
  puStack32 = &LAB_0048369d;
  puStack36 = (undefined *)0x483695;
  FUN_00405554((int *)&local_c,2);
  return;
}

