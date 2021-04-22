const REG_TAB = {
    "int": [
        ["x0",  "zero",  "Hard-wired zero",                   "--"    ],
        ["x1",  "ra",    "Return address",                    "Caller"],
        ["x2",  "sp",    "Stack pointer",                     "Callee"],
        ["x3",  "gp",    "Global pointer",                    "--"],
        ["x4",  "tp",    "Thread pointer",                    "--"],
        ["x5",  "t0",    "Temporary/alternate link register", "Caller"],
        ["x6",  "t1",    "Temporary register",                "Caller"],
        ["x7",  "t2",    "Temporary register",                "Caller"],
        ["x8",  "s0/fp", "Saved register/frame pointer",      "Callee"],
        ["x9",  "s1",    "Saved register",                    "Callee"],
        ["x10", "a0",    "Function argument/return value",    "Caller"],
        ["x11", "a1",    "Function argument/return value",    "Caller"],
        ["x12", "a2",    "Function argument",                 "Caller"],
        ["x13", "a3",    "Function argument",                 "Caller"],
        ["x14", "a4",    "Function argument",                 "Caller"],
        ["x15", "a5",    "Function argument",                 "Caller"],
        ["x16", "a6",    "Function argument",                 "Caller"],
        ["x17", "a7",    "Function argument",                 "Caller"],
        ["x18", "s2",    "Saved register",                    "Callee"],
        ["x19", "s3",    "Saved register",                    "Callee"],
        ["x20", "s4",    "Saved register",                    "Callee"],
        ["x21", "s5",    "Saved register",                    "Callee"],
        ["x22", "s6",    "Saved register",                    "Callee"],
        ["x23", "s7",    "Saved register",                    "Callee"],
        ["x24", "s8",    "Saved register",                    "Callee"],
        ["x25", "s9",    "Saved register",                    "Callee"],
        ["x26", "s10",   "Saved register",                    "Callee"],
        ["x27", "s11",   "Saved register",                    "Callee"],
        ["x28", "t3",    "Temporary register",                "Caller"],
        ["x29", "t4",    "Temporary register",                "Caller"],
        ["x30", "t5",    "Temporary register",                "Caller"],
        ["x31", "t6",    "Temporary register",                "Caller"],
    ],
    "flt": [
        ["f0",  "ft0",  "FP temporary",               "Caller"],
        ["f1",  "ft1",  "FP temporary",               "Caller"],
        ["f2",  "ft2",  "FP temporary",               "Caller"],
        ["f3",  "ft3",  "FP temporary",               "Caller"],
        ["f4",  "ft4",  "FP temporary",               "Caller"],
        ["f5",  "ft5",  "FP temporary",               "Caller"],
        ["f6",  "ft6",  "FP temporary",               "Caller"],
        ["f7",  "ft7",  "FP temporary",               "Caller"],
        ["f8",  "fs0",  "FP saved register",          "Callee"],
        ["f9",  "fs1",  "FP saved register",          "Callee"],
        ["f10", "fa0",  "FP argument / return value", "Caller"],
        ["f11", "fa1",  "FP argument / return value", "Caller"],
        ["f12", "fa2",  "FP argument",                "Caller"],
        ["f13", "fa3",  "FP argument",                "Caller"],
        ["f14", "fa4",  "FP argument",                "Caller"],
        ["f15", "fa5",  "FP argument",                "Caller"],
        ["f16", "fa6",  "FP argument",                "Caller"],
        ["f17", "fa7",  "FP argument",                "Caller"],
        ["f18", "fs2",  "FP saved register",          "Callee"],
        ["f19", "fs3",  "FP saved register",          "Callee"],
        ["f20", "fs4",  "FP saved register",          "Callee"],
        ["f21", "fs5",  "FP saved register",          "Callee"],
        ["f22", "fs6",  "FP saved register",          "Callee"],
        ["f23", "fs7",  "FP saved register",          "Callee"],
        ["f24", "fs8",  "FP saved register",          "Callee"],
        ["f25", "fs9",  "FP saved register",          "Callee"],
        ["f26", "fs10", "FP saved register",          "Callee"],
        ["f27", "fs11", "FP saved register",          "Callee"],
        ["f28", "ft8",  "FP temporary",               "Caller"],
        ["f29", "ft9",  "FP temporary",               "Caller"],
        ["f30", "ft10", "FP temporary",               "Caller"],
        ["f31", "ft11", "FP temporary",               "Caller"],
    ],
    "stack": [
        ["pancakes", "pancakes", "pancakes", "pancakes"]
    ]
}