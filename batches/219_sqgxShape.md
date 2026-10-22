# Batch 219_sqgxShape: 1 functions of `sqgxShape`

Name these functions: see `batches/README.md`. Write `data/names/219_sqgxShape.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1028AB50` fn_1028AB50

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10311C2C` x4
- **Called by:** `sqgxContext_BatchIndices`, `sub_1028BBD0`
- **Reads globals:** `0x1066941C`, `0x1099D200`

```cpp
XI_API int __cdecl fn_1028AB50(void)
{
  int result;
  int *v1;
  int v2;
  unsigned int v3;
  int *v4;
  int v5;
  int v6;
  int v7;
  int *v8;
  int v9;
  int v10;
  int v11;
  int *v12;
  int i;
  int v14;
  int *v15;
  int v16;
  int v17;
  int v18;
  int *v19;
  int v20;
  int v21;
  int v22;
  alignas(16) unsigned char xi_frame[0x47C];  // address-taken locals at their original offsets
  int &v23 = *reinterpret_cast<int *>(xi_frame + 0x74);
  unsigned int &v24 = *reinterpret_cast<unsigned int *>(xi_frame + 0x78);

  v1 = g_pSqgxContext;
  v2 = g_pSqgxContext[614];
  if ( v2 != 0 )
  {
    switch ( g_pSqgxContext[616] )
    {
      case 1:
        goto LABEL_4;
      case 2:
        v3 = v2 / 2;
        break;
      case 3:
        v3 = v2 - 1;
        break;
      case 4:
        v2 /= 3;
LABEL_4:
        v3 = v2;
        break;
      case 5:
      case 6:
        v3 = v2 - 2;
        break;
      default:
        v3 = v24;
        break;
    }
    if ( g_pSqgxContext[180] != 0 )
    {
      if ( (g_pSqgxContext[652] & 1) != 0 )
      {
        (*(void (__stdcall **)(int, _DWORD, int, int *, int))(*(_DWORD *)g_pSqgxContext[598] + 44))(
          g_pSqgxContext[598],
          0,
          12 * g_pSqgxContext[614],
          &v23,
          0x2000);
        v4 = g_pSqgxContext;
        v5 = 0;
        if ( g_pSqgxContext[614] > 0 )
        {
          v6 = 8;
          do
          {
            v6 += 12;
            v7 = *(__int16 *)(v4[615] + 2 * v5++);
            v7 *= 3;
            *(_DWORD *)(v6 + v23 - 20) = *(_DWORD *)(v4[582] + 4 * v7);
            *(_DWORD *)(v6 + v23 - 16) = *(_DWORD *)(g_pSqgxContext[582] + 4 * v7 + 4);
            *(_DWORD *)(v6 + v23 - 12) = *(_DWORD *)(g_pSqgxContext[582] + 4 * v7 + 8);
            v4 = g_pSqgxContext;
          }
          while ( v5 < g_pSqgxContext[614] );
        }
        (*(void (__stdcall **)(int))(*(_DWORD *)v4[598] + 48))(v4[598]);
        v1 = g_pSqgxContext;
      }
      if ( (v1[652] & 2) != 0 )
      {
        (*(void (__stdcall **)(int, _DWORD, int, int *, int))(*(_DWORD *)v1[600] + 44))(
          v1[600],
          0,
          12 * v1[614],
          &v23,
          0x2000);
        v8 = g_pSqgxContext;
        v9 = 0;
        if ( g_pSqgxContext[614] > 0 )
        {
          v10 = 8;
          do
          {
            v10 += 12;
            v11 = *(__int16 *)(v8[615] + 2 * v9++);
            v11 *= 3;
            *(_DWORD *)(v10 + v23 - 20) = *(_DWORD *)(v8[583] + 4 * v11);
            *(_DWORD *)(v10 + v23 - 16) = *(_DWORD *)(g_pSqgxContext[583] + 4 * v11 + 4);
            *(_DWORD *)(v10 + v23 - 12) = *(_DWORD *)(g_pSqgxContext[583] + 4 * v11 + 8);
            v8 = g_pSqgxContext;
          }
          while ( v9 < g_pSqgxContext[614] );
        }
        (*(void (__stdcall **)(int))(*(_DWORD *)v8[600] + 48))(v8[600]);
        v1 = g_pSqgxContext;
      }
      if ( (v1[652] & 8) != 0 )
      {
        (*(void (__stdcall **)(int, _DWORD, int, unsigned int *, int))(*(_DWORD *)v1[610] + 44))(
          v1[610],
          0,
          4 * v1[614],
          &v24,
          0x2000);
        v12 = g_pSqgxContext;
        for ( i = 0; i < g_pSqgxContext[614]; v12 = g_pSqgxContext )
        {
          v14 = 16 * *(__int16 *)(v12[615] + 2 * i);
          *(_BYTE *)(v24 + 4 * i + 2) = (__int64)(*(float *)(v12[588] + v14) * 255.0);
          *(_BYTE *)(v24 + 4 * i + 1) = (__int64)(*(float *)(g_pSqgxContext[588] + v14 + 4) * 255.0);
          *(_BYTE *)(v24 + 4 * i++) = (__int64)(*(float *)(g_pSqgxContext[588] + v14 + 8) * 255.0);
          *(_BYTE *)(v24 + 4 * i - 1) = (__int64)(*(float *)(g_pSqgxContext[588] + v14 + 12) * 255.0);
        }
        (*(void (__stdcall **)(int))(*(_DWORD *)v12[610] + 48))(v12[610]);
        v1 = g_pSqgxContext;
      }
      if ( (v1[653] & 4) != 0 )
      {
        (*(void (__stdcall **)(int, _DWORD, int, int *, int))(*(_DWORD *)v1[602] + 44))(
          v1[602],
          0,
          8 * v1[614],
          &v23,
          0x2000);
        v15 = g_pSqgxContext;
        v16 = 0;
        if ( g_pSqgxContext[614] > 0 )
        {
          v17 = 0;
          do
          {
            v18 = 2 * *(__int16 *)(v15[615] + v17);
// ... 55 more lines (tree/src/sqgxShape.cpp)
```
