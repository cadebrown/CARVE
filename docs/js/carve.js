var libcarve = null;
var editor = null;
var state = null;

loadlibcarve().then(function (_libcarve) {
    libcarve = _libcarve;

    // Initialize the editor
    editor = ace.edit("editor", {
        selectionStyle: "text"
    });
    ace.config.set('basePath', '/js')
    editor.setTheme('ace/theme/monokai')
    editor.getSession().setMode('ace/mode/riscv')
    editor.setAutoScrollEditorIntoView(true);

    // Creates a terminal with a callback for some text
    var term = $('#console').terminal(function(cmd, term) {
        // Write to stdin
        libcarve._buf_stdin += cmd + '\n';
    }, {
        name: 'carve-console',
        greetings: '',
        prompt: '',
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

        genregtable($("#intreg-table"), CARVE_INTREG, "int");

        genregtable($("#fltreg-table"), CARVE_FLTREG, "float");

        update_registers(state);
    })
        
})


/* Update the view with a given state */
function update_registers(s) {
    var dptr;
    for (var i = 0; i <= 31; ++i) {
        dptr = libcarve._carve_state_sri(s, i, 16)
        $('#reg_' + "intx" + i.toString() + '_hex').text(libcarve.UTF8ToString(dptr))
        libcarve._free(dptr)
        dptr = libcarve._carve_state_sri(s, i, 10)
        $('#reg_' + "intx" + i.toString() + '_dec').text(libcarve.UTF8ToString(dptr))
        libcarve._free(dptr)
    }
}

/* Generate the register table with appropriate IDs */
function genregtable(elem, regs, type) {
    const genrow = (reg, abi, desc, saver) => {
        return '<tr"><td class="reg_id">'+type+reg+'</td><td class="reg_name">'+abi+'</td><td class="reg_hex" id="reg_'+type+reg+'_hex">0</td><td class="reg_dec" id="reg_'+type+reg+'_dec">0</td></tr>';
    }

    let out = "<table id='reg_table'>";

    out += '<tr><td class="reg_id">REG</td><td class="reg_name">NAME</td><td class="reg_hex">HEX</td><td class="reg_dec">DEC</td></tr>'

    for (let i = 0; i < regs.length; i++) {
        out += genrow(...regs[i]);
    }

    out += "</table>";

    elem[0].innerHTML += out;
}

/* Inspired by w3 schools lesson                          */
/* https://www.w3schools.com/howto/howto_js_draggable.asp */

function hori_drag(drag, left, right) {
    var old_x;
    
    left["size"] = 100 * left.width() / window.innerWidth;

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

        var ww = window.innerWidth;
        left["size"] = 100 * Math.min(0.75 * ww, Math.max(0.20 * ww, left["size"] * ww / 100 + (e.clientX - old_x))) / ww

        //left.css('width', "calc(50vw + " + left["adjust"] + "px)");
        left.css('width', left["size"].toString() + 'vw')
        old_x = e.clientX;
      }

    function closeDragElement() {
        // stop moving when mouse button is released:
        document.onmouseup = null;
        document.onmousemove = null;
        
        editor.resize();
        editor.renderer.updateFull();
    }
}

function vert_drag(drag, top, bottom) {
    var old_y;

    top["size"] = 100 * top.height() / window.innerHeight;

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

        var wh = window.innerHeight;
        top["size"] = 100 * Math.min(0.85 * wh, Math.max(0.15 * wh, top["size"] * wh / 100 + (e.clientY - old_y))) / wh

        top.css('height', top["size"].toString() + 'vh')
        bottom.css('height', (100 - top["size"] - 100 * (22 + 8) / wh).toString() + 'vh')
        old_y = e.clientY;
      }

    function closeDragElement() {
        // stop moving when mouse button is released:
        document.onmouseup = null;
        document.onmousemove = null;

        editor.resize();
        editor.renderer.updateFull();
    }
}