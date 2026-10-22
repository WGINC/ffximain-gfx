# Batch r2_099_CMoSchedularTask: 2 functions of `CMoSchedularTask`

Name these functions: see `batches/README.md`. Write `data/names/r2_099_CMoSchedularTask.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100573B0` fn_100573B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100571E0`, `CMoSchedularTask_Construct` (medium), `sub_100572E0`, `sub_10057D60`
- **Named before** (a naming session's reading, low): `CMoSchedularTask_ResetFlags`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_1008ABE0` → `AfterimageTask_Execute` → `Scheduler_Execute` → `Scheduler_CreateTask` → `CMoSchedularTask_Construct` → `CMoSchedularTask_ResetFlags`
- **Named callers' callers:** `Scheduler_CreateTask`
- **How its callers call it** (their verified code):
  - in `CMoSchedularTask_Construct` (medium): `CMoSchedularTask_ResetFlags(self_);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CMoSchedularTask_Construct` (medium): Constructs a schedular task and initialises its members.

```cpp
XI_API void __thiscall CMoSchedularTask_ResetFlags(void * self_)
{
  *(uint8_t *)((int)self_ + 0x141) = 0;
  *(uint8_t *)((int)self_ + 0x143) = 0;
  *(uint8_t *)((int)self_ + 0x144) = 0;
  *(uint8_t *)((int)self_ + 0x140) = 0;
  *(uint8_t *)((int)self_ + 0x145) = 0;
  *(uint32_t *)((int)self_ + 0x146) = 0;
  return;
}
```

## `0x10057CC0` fn_10057CC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100573E0`
- **Named before** (a naming session's reading, low): `CMoSchedularTask_SetSharedRef`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `sub_100DAF30` → `sub_1008ABE0` → `AfterimageTask_Execute` → `Scheduler_Execute` → `Scheduler_CreateTask` → `CMoSchedularTask_Construct` → `sub_100573E0` → `CMoSchedularTask_SetSharedRef`
- **Named callers' callers:** `CMoSchedularTask_Construct`

```cpp
XI_API void __thiscall CMoSchedularTask_SetSharedRef(void * self_, short * param_1)
{
  if (param_1 != (short *)0x0) {
    *(short **)((int)self_ + 0x90) = param_1;
    *param_1 = *param_1 + 1;
    Unit10064190_SetDwordValue((void *)((int)self_ + 0xc0),0);
    return;
  }
  return;
}
```
