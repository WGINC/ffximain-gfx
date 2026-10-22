# Batch 182_CMoTask_VT_1032B95C: 2 functions of `CMoTask_VT_1032B95C`

Name these functions: see `batches/README.md`. Write `data/names/182_CMoTask_VT_1032B95C.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10063130` CMoTask_VT_1032B95C::vf011

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
int CMoTask_VT_1032B95C::vf011()
{
  return (int)orig::CMoPathDriveActorTask_GetRuntimeClass();
}
```

## `0x10063140` CMoTask_VT_1032B95C::vf012

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
int CMoTask_VT_1032B95C::vf012(int a1)
{
  return (int)orig::CMoPathDriveActorTask_ScalarDeletingDtor((_DWORD *)this - 13, a1);
}
```
