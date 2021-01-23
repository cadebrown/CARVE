var libcarve = null;
var editor = null;
var state = null;

loadlibcarve().then(function (_libcarve) {
    // Initialize the editor
    editor = ace.edit("editor", {
        selectionStyle: "text"
    });
    ace.config.set('basePath', '/')
    editor.setTheme('ace/theme/monokai')
    //editor.getSession().setMode('ace/mode/riscv')

    libcarve = _libcarve;

    // Add hooks to libcarve to print console output
    libcarve._write_stdout.push(function (text) {
        console.log('[stdout]', text)
    })
    libcarve._write_stderr.push(function (text) {
        console.warn('[stderr]', text)
    })

    // Initialize the library before any more calls
    libcarve._carve_init();

    state = libcarve._carve_state_new()
    
    $(document).ready( function () {
        // Now, we are updating the state
        update_registers(state)
    })
})


/* Update the view with a given register */
function update_registers(s) {
    var dptr;
    for (var i = 0; i < 15; ++i) {
        dptr = libcarve._carve_state_sri(s, i, 16)
        $('#ir' + i.toString() + '-16').text(libcarve.UTF8ToString(dptr))
        libcarve._free(dptr)
        dptr = libcarve._carve_state_sri(s, i, 10)
        $('#ir' + i.toString() + '-10').text(libcarve.UTF8ToString(dptr))
        libcarve._free(dptr)
    }
}

/* Generate the register table with appropriate IDs */
function genregtable(id) {
    const genrow = (reg, abi, desc, saver) => {
        return '<tr id='+reg+'_row><td>'+reg+'</td><td>'+abi+'</td><td id='+reg+'_val>0</td></tr>';
    }

    let regs = [
        ["zero",  "Hard-wired zero",                   "--"    ],
        ["ra",    "Return address ",                   "Caller"],
        ["sp",    "Stack pointer",                     "Callee"],
        ["gp",    "Global pointer",                    "--"],
        ["tp",    "Thread pointer",                    "--"],
        ["t0",    "Temporary/alternate link register", "Caller"],
        ["t1",    "Temporary register",                "Caller"],
        ["t2",    "Temporary register",                "Caller"],
        ["s0/fp", "Saved register/frame pointer",      "Callee"],
        ["s1",    "Saved register",                    "Callee"],
        ["a0",    "Function argument/return value",    "Caller"],
        ["a1",    "Function argument/return value",    "Caller"],
        ["a2",    "Function argument",                 "Caller"],
        ["a3",    "Function argument",                 "Caller"],
        ["a4",    "Function argument",                 "Caller"],
        ["a5",    "Function argument",                 "Caller"],
        ["a6",    "Function argument",                 "Caller"],
        ["a7",    "Function argument",                 "Caller"],
        ["s2",    "Saved register",                    "Callee"],
        ["s3",    "Saved register",                    "Callee"],
        ["s4",    "Saved register",                    "Callee"],
        ["s5",    "Saved register",                    "Callee"],
        ["s6",    "Saved register",                    "Callee"],
        ["s7",    "Saved register",                    "Callee"],
        ["s8",    "Saved register",                    "Callee"],
        ["s9",    "Saved register",                    "Callee"],
        ["s10",   "Saved register",                    "Callee"],
        ["s11",   "Saved register",                    "Callee"],
        ["t3",    "Temporary register",                "Caller"],
        ["t4",    "Temporary register",                "Caller"],
        ["t5",    "Temporary register",                "Caller"],
        ["t6",    "Temporary register",                "Caller"],
    ];

    let out = "<table id='reg_table'>";

    out += '<tr><td>Register</td><td>ABI Name</td><td>Value</td></tr>'

    for (let i = 0; i < regs.length; i++) {
        out += genrow("x" + i, ...regs[i]);
    }

    out += "</table>";

    $("#" + id)[0].innerHTML = out;
}