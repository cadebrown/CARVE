var libcarve = null;
var editor = null;
var state = null;

loadlibcarve().then(function (_libcarve) {
    libcarve = _libcarve;

    // Initialize the editor
    editor = ace.edit("editor", {
        selectionStyle: "text"
    });
    ace.config.set('basePath', '/')
    editor.setTheme('ace/theme/monokai')
    //editor.getSession().setMode('ace/mode/riscv')

    // Creates a terminal with a callback for some text
    var term = $('#console').terminal(function(cmd, term) {
        // Write to stdin
        libcarve._buf_stdin += cmd + '\n';
    }, {
        name: 'carve-console',
        greetings: '',
        prompt: '',
        height: 128,
    })

    // write to stdout
    libcarve._write_stdout.push(function (text) {
        term.echo("[[;#BBBBBB;]" + text + "]")
    })
    // write to stderr
    libcarve._write_stderr.push(function (text) {
        term.echo("[[;#EA6452;]" + text + "]")
    })

    // Add hooks to libcarve to print console output as well as to the on screen terminal
    libcarve._write_stdout.push(function (text) {
        console.log('[stdout]', text)
    })
    libcarve._write_stderr.push(function (text) {
        console.warn('[stderr]', text)
    })

    // Initialize the library before any more calls
    libcarve._carve_init();

    state = libcarve._carve_state_new()
    
    $(document).ready(function () {
        // Now, we are updating the state
        hori_drag($("#resize-tabedit"), $("#editor"), $("#registers"));

        vert_drag($("#resize-console"), $("#main"), $("#console"));

        genregtable($("#registers"));

        update_registers(state);
    })
        
})


/* Update the view with a given state */
function update_registers(s) {
    var dptr;
    for (var i = 0; i <= 31; ++i) {
        dptr = libcarve._carve_state_sri(s, i, 16)
        $('#reg_x' + i.toString() + '_hex').text(libcarve.UTF8ToString(dptr))
        libcarve._free(dptr)
        dptr = libcarve._carve_state_sri(s, i, 10)
        $('#reg_x' + i.toString() + '_dec').text(libcarve.UTF8ToString(dptr))
        libcarve._free(dptr)
    }
}

/* Generate the register table with appropriate IDs */
function genregtable(elem) {
    const genrow = (reg, abi, desc, saver) => {
        return '<tr"><td class="reg_id">'+reg+'</td><td class="reg_name">'+abi+'</td><td class="reg_hex" id="reg_' + reg + '_hex">0</td><td class="reg_dec" id="reg_' + reg + '_dec">0</td></tr>';
    }

    let regs = [
        ["x0",  "zero",  "Hard-wired zero",                   "--"    ],
        ["x1",  "ra",    "Return address ",                   "Caller"],
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
    ];

    let out = "<table id='reg_table'>";

    out += '<tr><td class="reg_id">REG</td><td class="reg_name">NAME</td><td class="reg_hex">HEX</td><td class="reg_dec">DEC</td></tr>'

    for (let i = 0; i < regs.length; i++) {
        out += genrow(...regs[i]);
    }

    out += "</table>";

    elem[0].innerHTML = out;
}

/* Inspired by w3 schools lesson                          */
/* https://www.w3schools.com/howto/howto_js_draggable.asp */

function hori_drag(drag, left, right) {
    var old_x;
    
    left["adjust"] = 0;
    right["adjust"] = 0;


    drag[0].onmousedown = dragMouseDown;

    function dragMouseDown(e) {
        e = e || window.event;
        e.preventDefault();
        // get the mouse cursor position at startup:
        old_x = e.clientX;
        document.onmouseup = closeDragElement;
        // call a function whenever the cursor moves:
        document.onmousemove = elementDrag;
    }

    function elementDrag(e) {
        e = e || window.event;
        e.preventDefault();

        left["adjust"] -= (old_x - e.clientX) * 2;
        //left.css('width', (parseInt(left.css('width'))) + "px");
        left.css('width', "calc(50vw + " + left["adjust"] + "px)");
        old_x = e.clientX;
      }

    function closeDragElement() {
        // stop moving when mouse button is released:
        document.onmouseup = null;
        document.onmousemove = null;
    }
}

function vert_drag(drag, top, bottom) {
    var old_y, adjust = 0;

    drag[0].onmousedown = dragMouseDown;

    function dragMouseDown(e) {
        e = e || window.event;
        e.preventDefault();
        // get the mouse cursor position at startup:
        old_y = e.clientY;
        document.onmouseup = closeDragElement;
        // call a function whenever the cursor moves:
        document.onmousemove = elementDrag;
    }

    function elementDrag(e) {
        e = e || window.event;
        e.preventDefault();

        adjust += (old_y - e.clientY);
        old_y = e.clientY;
        console.log(top);
        top.css('height', 'calc(75vh + ' + ((-1 * adjust) - 45) + 'px)');
        bottom.css('height', 'calc(25vh + ' + (adjust) + 'px)');
      }

    function closeDragElement() {
        // stop moving when mouse button is released:
        document.onmouseup = null;
        document.onmousemove = null;
    }
}