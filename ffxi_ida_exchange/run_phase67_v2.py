"""run_phase67.py -- phases 6 (prototypes) and 7 (names/comments) of apply_recon_ida_v2.py.

Run in IDA via File -> Script file... AFTER phases 1-5 have been applied (the IDB must contain
the CMoElem struct). Writes a line to progress.txt before every IDA call that changes the
database, so a hang can be pinned to one function and one call.
"""
import os, json, hashlib, time
import ida_funcs, ida_typeinf, ida_nalt, idc, ida_kernwin

D = r"C:\Users\wadeglover\Downloads\ffxi_ida_exchange"
SRC = os.path.join(D, "apply_recon_ida_v2.py")
PROG = os.path.join(D, "progress.txt")

def prog(msg):
    with open(PROG, "w") as f:
        f.write("%s  %s\n" % (time.strftime("%H:%M:%S"), msg))

b = open(SRC, "rb").read()
assert hashlib.md5(b).hexdigest() == "cc4d957882499008d0486faffc1551ed", "apply_recon_ida_v2.py changed"
t = ida_typeinf.tinfo_t()
assert t.get_named_type(ida_typeinf.get_idati(), "CMoElem"), "phases 1-5 not applied to this IDB"

ns = {"DRY_RUN": False, "RECON_DIR": D, "__name__": "recon", "RECON_DEFS_ONLY": True}
try:
    exec(compile(b.decode(), SRC, "exec"), ns)
except SystemExit:
    pass
P, C, LOG = ns["P"], ns["C"], ns["LOG"]

# ---- phase 6, one function at a time, logged per call
created = [int(x, 16) for x in P["create"]
           if ida_funcs.get_func(int(x, 16)) and ida_funcs.get_func(int(x, 16)).start_ea == int(x, 16)]
for i, ea in enumerate(created):
    if ida_nalt.get_tinfo(ida_typeinf.tinfo_t(), ea):
        C["p6_has_type"] += 1; continue
    x = hex(ea)
    conv, n = P["protos"].get(x, [None, None])
    if conv not in ("thiscall", "stdcall", "cdecl") or n is None:
        C["p6_no_conv:%s" % conv] += 1; continue
    if n > 16:   # cc.py loses ESP after __alloca_probe and counts locals as arguments (up to 199,558)
        C["p6_skipped_implausible_nargs"] += 1; LOG["p6_implausible_nargs"].append([x, conv, n]); continue
    purge = idc.get_func_attr(ea, idc.FUNCATTR_ARGSIZE)
    prog("p6 %d/%d %s ecx_read" % (i, len(created), x))
    r = ns["ecx_read"](ea)
    final = conv
    if r is True and conv != "thiscall": final = "thiscall"
    if r is False and conv == "thiscall": final = "stdcall" if (purge or 0) > 0 else "cdecl"
    if final != conv:
        C["p6_ecx_corrected"] += 1; LOG["p6_corrected"].append([x, conv, final])
    m = P["names"].get(x)
    cls = m["class"] if m and m["kind"] == "vt" and not m["static"] else None
    args = ["int a%d" % (k + 1) for k in range(n)]
    if final == "thiscall":
        args = ["%s *this" % ns["struct_name"](cls) if cls else "void *this"] + args
    decl = "int __%s f(%s);" % (final, ", ".join(args) if args else "void")
    prog("p6 %d/%d %s parse_decl %s" % (i, len(created), x, decl))
    tif = ida_typeinf.tinfo_t()
    ok = ida_typeinf.parse_decl(tif, None, decl, ns["PT_SIL"]) is not None
    if not ok and cls:
        decl = decl.replace("%s *this" % ns["struct_name"](cls), "void *this")
        ok = ida_typeinf.parse_decl(tif, None, decl, ns["PT_SIL"]) is not None
    prog("p6 %d/%d %s apply_tinfo %s" % (i, len(created), x, decl))
    if ok and ida_typeinf.apply_tinfo(ea, tif, ida_typeinf.TINFO_DEFINITE):
        C["p6_typed"] += 1
    else:
        C["p6_type_failed"] += 1; LOG["p6_failed"].append([x, decl])

# ---- phase 7 (names and comments), logged at start and end
prog("p7 start")
ns["phase7"]()
prog("p7 done")
json.dump(LOG, open(os.path.join(D, "apply_log_p6p7.json"), "w"), indent=0)
json.dump(dict(C), open(os.path.join(D, "apply_counts_p6p7.json"), "w"), indent=0)
prog("ALL DONE " + json.dumps({k: v for k, v in C.items() if k[:2] in ("p6", "p7")}))
ida_kernwin.info("run_phase67 finished -- see ffxi_ida_exchange\\progress.txt")
