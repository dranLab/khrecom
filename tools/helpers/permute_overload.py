#!/usr/bin/env python3
"""Scratch wrapper (tools/ untouched): permuter on one overload of a const method.
usage: permute_overload.py <unit> <method> <mangled> <param-substring> [permuter args]"""
import sys, re
sys.path.insert(0, 'tools')
import permuter
unit, method, mangled, psub = sys.argv[1:5]
orig_functions = permuter.functions
def patched_functions(path):
    d = orig_functions(path)
    if mangled in d:
        d = dict(d); d[method] = d[mangled]
    return d
permuter.functions = patched_functions
orig_dis = permuter.disassemble
permuter.disassemble = lambda obj, name, size: orig_dis(obj, mangled if name == method else name, size)
def find_function(text, name):
    for m in re.finditer(r"^[A-Za-z_][^;{}]*\b" + re.escape(name) + r"\s*\(([^;{}]*)\)\s*(const\s*)?\{", text, re.M):
        if psub not in m.group(1):
            continue
        start = m.end() - 1; depth = 0; i = start
        while i < len(text):
            c = text[i]
            if c == "{": depth += 1
            elif c == "}":
                depth -= 1
                if depth == 0: return start, i + 1
            elif c == '"': i = text.index('"', i + 1)
            elif text.startswith("/*", i): i = text.index("*/", i) + 1
            elif text.startswith("//", i): i = text.index("\n", i)
            i += 1
    raise SystemExit(f"function {name} ({psub}) not found")
permuter.find_function = find_function
sys.argv = ['permuter.py', unit, method] + sys.argv[5:]
permuter.main()
