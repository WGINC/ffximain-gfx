# Batch 159_CMoLockLookAtDriveTask: 2 functions of `CMoLockLookAtDriveTask`

Name these functions: see `batches/README.md`. Write `data/names/159_CMoLockLookAtDriveTask.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10062F80` CMoLockLookAtDriveTask::vf011

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
int CMoLockLookAtDriveTask::vf011()
{
  return (int)orig::CMoActorColorDriveTask_GetRuntimeClass();
}
```

## `0x10062F90` CMoLockLookAtDriveTask::vf012

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
int CMoLockLookAtDriveTask::vf012(int a1)
{
  Vt_1032BA60 * self = reinterpret_cast<Vt_1032BA60 *>(this);  // byte-exact view for the rest
  return orig::CMoActorColorDriveTask_ScalarDeletingDtor((int)(self - 13), a1);
}
```
