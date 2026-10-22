# Batch 158_CMoLockColorDriveTask: 2 functions of `CMoLockColorDriveTask`

Name these functions: see `batches/README.md`. Write `data/names/158_CMoLockColorDriveTask.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100630F0` CMoLockColorDriveTask::vf011

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
int CMoLockColorDriveTask::vf011()
{
  return (int)orig::CMoLockLookAtDriveTask_GetRuntimeClass();
}
```

## `0x10063100` CMoLockColorDriveTask::vf012

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
int CMoLockColorDriveTask::vf012(int a1)
{
  Vt_1032BA18 * self = reinterpret_cast<Vt_1032BA18 *>(this);  // byte-exact view for the rest
  return (int)orig::CMoLockLookAtDriveTask_ScalarDeletingDtor((_DWORD *)self - 13, a1);
}
```
