var libcarve = null;
var editor = null;
var state = null;
var reg_table = null;

loadlibcarve().then(function (_libcarve) {
    libcarve = _libcarve;

    // Initialize the editor
    editor = ace.edit("editor", {
        selectionStyle: "text"
    });
    ace.config.set('basePath', '/js')
    editor.setTheme('ace/theme/monokai')
    editor.setOptions({
        fontSize: '13pt',
    })
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
        hori_drag($("#resize-tabedit"), $("#editor-container"), $("#registers"));

        vert_drag($("#resize-console"), $("#main"), $("#console"));

        reg_table = $("#reg-table");

        $("#tab_type_sel").change(
            function() {
                genregtable(reg_table, REG_TAB[this.value], this.value);
                update_registers(state);
            }
        )

        $("#tab_type_sel").change();

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
        return '<tr"><td class="register_table reg_id">'+reg+'</td><td class="register_table reg_name">'+abi+'</td><td class="register_table reg_hex" id="reg_'+type+reg+'_hex">0</td><td class="register_table reg_dec" id="reg_'+type+reg+'_dec">0</td></tr>';
    }

    let out = "<table class='register_table' id='reg_table'>";

    out += '<tr><td class="register_table reg_id t_head">REG</td><td class="register_table reg_name t_head">NAME</td><td class="register_table reg_hex t_head">HEX</td><td class="register_table reg_dec t_head">DEC</td></tr>'

    for (let i = 0; i < regs.length; i++) {
        out += genrow(...regs[i]);
    }

    out += "</table>";

    elem[0].innerHTML = out;
}

function selregtable(elem, val) {
    genregtable(elem, REG_TAB[val], val)
}

function doeasteregg(num) {
    if (num == 1) {
        var audio = new Audio('/assets/google_sound1.mp3');
        audio.play();
    }

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
        left["size"] = 100 * Math.min(ww - 256, Math.max(0.20 * ww, 460, (e.clientX))) / ww

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

/* Compile text box */
function compile() {

    // Filename and source
    var fname = "<>"
    var src = editor.getValue()

    var _fname = libcarve._malloc(fname.length + 2)
    var _src = libcarve._malloc(src.length + 2)

    libcarve.stringToUTF8(fname, _fname, fname.length + 1)
    libcarve.stringToUTF8(src, _src, src.length + 1)

    var prog = libcarve._carve_prog_new(_fname, _src)
    if (!prog) return

    
    libcarve._carve_exec(state, prog)

    var ee = libcarve._carve_easteregg(state);
    if (ee != 0) {
        doeasteregg(ee)
    }


    update_registers(state)


}


/* Menu Options */

function do_file_open() {
    let elem = $("#file-select")
    
    elem.trigger('click')
    elem.on('change', function() {
        elem[0].files[0].text().then(function (src) {
            // Change ace editor contents
            //console.log(src)
            editor.setValue(src)
        })
        elem.off('change')
    })

}

function do_file_save() {
    let fname = 'src.s'
    let src = editor.getValue()

    var elem = document.createElement('a');
    elem.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(src));
    elem.setAttribute('download', fname);
  
    elem.style.display = 'none';
    document.body.appendChild(elem);
  
    elem.click();
    document.body.removeChild(elem);

}
