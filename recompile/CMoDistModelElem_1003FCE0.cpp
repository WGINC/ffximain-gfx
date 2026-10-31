/* 0x1003FCE0 CMoDistModelElem::GetVertexBase and 0x1003FCF0 CMoDistModelElem::GetDataEnd (tree/src/CMoDistModelElem.cpp,
   frame set), at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's (gate-verified).
   Byte-deciding form (recompile/d3d_round_121.md rule 28): GetVertexBase is defined first in this file; GetDataEnd
   calls it and then reads ECX (this) again without reloading it, which VC6 does only for a callee it has already
   compiled in the same file. GetDataEnd's three cases of the low four bits of +0x30 as a switch; the field names
   are offsets (the tree names none). Return types: the pointer arithmetic is on 2-byte units (`lea eax,
   [esi + ecx*2]`), so short *. */
struct CMoDistModelElem { unsigned char pad[0x30]; unsigned char b30; unsigned char pad31[3]; unsigned char b34, b35; short *GetVertexBase(); short *GetDataEnd(); };
// FUNCTION: FFXIMAIN 0x1003FCE0
short *CMoDistModelElem::GetVertexBase()
{
  return (short *)((char *)this + 0x38);
}
// FUNCTION: FFXIMAIN 0x1003FCF0
short *CMoDistModelElem::GetDataEnd()
{
  switch (b30 & 0xF)
  {
  case 5:
  case 6:
    {
      short *p = GetVertexBase();
      int n = b35 + b34;
      int r = n & 3;
      if (r)
        return p + (n - r) + 3;
      return p + n;
    }
  case 7:
    {
      short *p = GetVertexBase();
      if (b34 == 0)
        return p;
      int n = b35 + b34;
      int r = n & 3;
      if (r)
        return p + (n - r) + 3;
      return p + n;
    }
  }
  return (short *)((char *)this + 0x40);
}
