# Batch 121_CMoActorColorDriveTask: 2 functions of `CMoActorColorDriveTask`

Name these functions: see `batches/README.md`. Write `data/names/121_CMoActorColorDriveTask.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10062FE0` CMoActorColorDriveTask::vf011

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
int CMoActorColorDriveTask::vf011()
{
  return (int)orig::CMoActorRotationDriveTask_GetRuntimeClass();
}
```

## `0x10062FF0` CMoActorColorDriveTask::vf012

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
tagRECT * CMoActorColorDriveTask::vf012(char a2)
{
  Vt_1032BAA8 * self = reinterpret_cast<Vt_1032BAA8 *>(this);  // byte-exact view for the rest
  return orig::CMoActorRotationDriveTask_ScalarDeletingDtor((tagRECT *)(self - 13), a2);
}
```
