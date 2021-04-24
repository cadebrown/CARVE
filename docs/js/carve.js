// CARVE WASM library handle
let libcarve = null

// carve::State object, the current state
let state = null
// carve::Program object, the current program being compiled
let program = null

// Whether the worker is active
let worker_active = false

// The delay (in seconds) between each instruction, or < 0 if the worker
//   is inactive
let worker_delay = 0.000

// Ace editor
let editor = null

// Terminal/console output
let term = null

// debug table
let debug_table = null

// last time editor was altered / program built
let update_time = null
let build_time = null

// Temporary buffer for register strings
let tmpbuf = null
let tmpbuf_len = 1024

// menu setting for console verbosity
var verbosity = 1;

// Color for meta
const COL_META = '#e3cb17'
const COL_WARN = '#ffa500'
const COL_ERR  = '#ff0000'

loadlibcarve().then(function (_libcarve) {
    libcarve = _libcarve;
    
    
    /* ACE EDITOR */
    // Initialize the editor
    editor = ace.edit("ace_editor", {
        selectionStyle: "text"
    })
    ace.config.set('basePath', '/js')
    editor.setTheme('ace/theme/monokai')
    editor.setOptions({
        fontSize: '13pt',
    })
    editor.getSession().setMode('ace/mode/riscv')
    editor.getSession().on('change', function() {
        update_time = Date.now();
    });

    /* Debug Table */
    debug_table = $("#debug_table");

    /* JQUERY TERMINAL */
    // Creates a terminal with a callback for some text
    term = $('#console').terminal(function(cmd, term) {
        // Write to stdin
        libcarve._buf_stdin += cmd + '\n';
    }, {
        name: 'carve-console',
        greetings: '',
        prompt: '',
    })

    // Write to stdout
    libcarve._write_stdout.push(function (text) {
        term.echo("[[;#BBBBBB;]" + text + "]")
    })
    // Write to stderr
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
    if (verbosity > 0) send_meta("Initializing...")
    libcarve._carve_init();
    if (verbosity > 0) send_meta("Initialized!")

    // Allocate temporary buffior
    tmpbuf = libcarve._malloc(tmpbuf_len)

    // Allocate an empty state
    state = libcarve._carve_state_new()

    $(document).ready(function () {
        function update_menu_speed() {
            // Raw value, from 0-1
            let raw = Number($("#menu_speed")[0].value)

            let hz = Math.pow(raw, 2.5) * 99 + 1
            $("#menu_speed_text").text(hz.toFixed(1) + "hz")

            let amt = 1 / hz
            worker_delay = amt

        }
        // Handle when the menu speed changes
        $("#menu_speed").on("input", function() {
            update_menu_speed()
        })
        update_menu_speed()

        // Give execution bar buttons descriptions
        $(".exe").each(element => {
            const e = $(".exe")[element]
            tippy(`#${e.id}`, {
                content: EXE_DESC[e.id],
                placement: 'top',
            });
        });

        // Set the callback for when the selector is changed
        $("#tab_type_sel").change(function() {

            // Table of registers, the element that should be modified
            let elem = $("#reg_table")

            // What type of registers/view?
            let type = this.value

            // List of registers of this type
            let regs = REG_TAB[type]


            let out = "<table class='regs reg_table'>"

        
            // Add header row
            out += "<tr class='regs'><td class='regs reg_head reg_id'>REG</td><td class='regs reg_head reg_name'>NAME</td><td class='regs reg_head reg_hex'>HEX</td><td class='regs reg_head reg_dec'>DEC</td></tr>"
            
            const make_row = (id, name, desc, save) => {
                return "<tr class='regs' id='reg_" + (id) + "'><td class='regs reg_id' id='reg_" + (id) + "_id'>" + (id) + "</td><td class='regs reg_name' id='reg_" + (id) + "_name'>" + (name) + "</td><td class='regs reg_hex' id='reg_" + (id) + "_hex'></td><td class='regs reg_dec' id='reg_" + (id) + "_dec'></td></tr>"
            }

            for (let i = 0; i < regs.length; ++i) {
                out += make_row(...regs[i])
            }

            out += "</table>"
            elem[0].innerHTML = out

            for (let i = 0; i < regs.length; i++) {
                let content = regs[i][2];
        
                if (regs[i][3] != "--") {
                    content += "; " + regs[i][3] + " saved"
                }
                
                tippy('#reg_' + regs[i][0], {
                    content: content,
                    placement: 'left',
                    allowHTML: true,
                    interactive: true,
                });
            }

            // Update the graphical interface
            update_ui()
        })


        // Updates the ACE editor
        const update_ace = () => {
            editor.resize()
            editor.renderer.updateFull()
        }

        /* SPLIT (SPLITPANEL LIBRARY) */
        Split({ // gutters specified in options
            columnGutters: [{
                track: 1,
                element: $('.exe_bar')[0],
            }, {
                track: 1,
                element: $('.editor_bar')[0],
            }],
            rowGutters: [{
                track: 2,
                element: $('.console_bar')[0],
            }],
            onDragEnd: () => { 
                update_ace()
            },
            columnMinSize: $(window).width() / 10,
            rowMinSize: $(window).height() / 10 + 42,
        });
        Split({ // gutters specified in options
            columnGutters: [{
                track: 1,
                element: $('.reg_bar')[0],
            }],
            onDragEnd: () => {
                update_ace()
            },
            rowMinSize: $(window).height() / 10,
            columnMinSize: ($(window).width() / 10) * 2 + 16,
        });
    
        // tippy
        let tree = {
            'file': ['open', 'save', 'new'],
            'options': [['verbosity', `<span class="menu-dropdown nohov">Verbosity</span> \
                <input oninput="verbosity = parseInt($('#verbosity')[0].value)" type="number" id="verbosity" name="verbosity" min="0" max="3" style="width: 32px;" value=${verbosity}>`]],
            'assembler': ['ASSEMBLER INFORMATION (extensions, compilation date, etc.)'],
            'help': ['CARVE Documentation', 'Syscall reference', 'RISC-V Spec']
        }
        
        for (let k in tree) {
            let content = ''
            for (let i = 0; i < tree[k].length; ++i) {
                let name, elm;
                if (Array.isArray(tree[k][i])) {
                    name = [tree[k][i][0]];
                    elm = [tree[k][i][1]];
                } else {
                    name = tree[k][i];
                    elm = '<div class="menu-dropdown" onclick="do_' + k + '_' + name + '()">' + name + '</div>'
                }
                content += elm;
            }
        
            tippy('#menu_' + k, {
                content: content,
                allowHTML: true,
                interactive: true,
                hideOnClick: false,
                arrow: false,
                offset: [0, 4],
            });
        }


        // Simulate the change event for the first time
        $("#tab_type_sel").change()
    })
})


/** Engine Functions **/

// Updates the UI elements for the engine's current register values
function update_ui() {
    for (let i = 0; i < 32; ++i) {
        let base = 'reg_' + "x" + i.toString()

        libcarve._carve_getrx(state, tmpbuf_len, tmpbuf, i, 16)
        $('#' + base + '_hex').text(libcarve.UTF8ToString(tmpbuf))

        libcarve._carve_getrx(state, tmpbuf_len, tmpbuf, i, 10)
        $('#' + base + '_dec').text(libcarve.UTF8ToString(tmpbuf))
    }
    for (let i = 0; i < 32; ++i) {
        let base = 'reg_' + "f" + i.toString()

        libcarve._carve_getrf(state, tmpbuf_len, tmpbuf, i)
        $('#' + base + '_dec').text(libcarve.UTF8ToString(tmpbuf))

        libcarve._carve_getrfx(state, tmpbuf_len, tmpbuf, i)
        $('#' + base + '_hex').text(libcarve.UTF8ToString(tmpbuf))
    }
}

function update_debug(str) {
    let rows = str.split("!");
    let out = `<tr><th>Code</th><th>Inst</th><th>Line</th></tr>`;
    for (r in rows) {
        const data = rows[r].split(";");
        out += `<tr><td>${data[0]}</td><td>${data[1]}</td><td>${data[2]}</td></tr>`;
    }

    debug_table[0].innerHTML = out;
}

/** UI Functions **/

// Run a single instruction
function _worker_single() {
    if (worker_active) {
        if (do_step() < 0) {send_err("Error stepping, execution ended!"); return;}

        // Read the speed, turn it into a delay
        setTimeout(_worker_single, worker_delay * 1000)
    }
}

// Start the engine working
function worker_start() {
    if (worker_active) {
        // Do nothing
    } else {
        worker_active = true
        _worker_single()
    }
}

// Stop the engine working
function worker_stop() {
    worker_active = false
}

// Re-compiles and updates the program
function do_build() {
    worker_stop()
    build_time = Date.now()
    if (verbosity > 0) send_meta("Building...")

    // Filename string
    let fname = "<>"
    // Source string
    let src = editor.getValue()

    if (program !== null) {
        libcarve._carve_program_free(program)
        program = null
    }

    // Convert to NUL-terminated buffers
    let buf_fname = libcarve._malloc(fname.length + 2)
    let buf_src = libcarve._malloc(src.length + 2)
    libcarve.stringToUTF8(fname, buf_fname, fname.length + 1)
    libcarve.stringToUTF8(src, buf_src, src.length + 1)

    // Create program
    program = libcarve._carve_program_new(buf_fname, buf_src)
    
    // Free temporary buffers
    libcarve._free(buf_fname)
    libcarve._free(buf_src)

    // Error occured!
    if (!program) {
        return
    }

    // Update the state for the program
    libcarve._carve_state_init(state, program)

    // Update execution table
    let debug = libcarve._carve_get_debug(program)
    update_debug(libcarve.UTF8ToString(debug))
    libcarve._free(debug);

    if (verbosity > 0) send_meta("Built!")
    update_ui()
}

// Run the entire program
function do_restart() {
    send_err("Not implemented")
}

// Continue executing
function do_play() {
    if (verbosity > 0 && (update_time > build_time)) send_warn("Running, but build not updated!")
    if (verbosity > 0) send_meta("Running...")
    worker_start()
}

// Stop executing
function do_pause() {
    if (verbosity > 0) send_meta("Paused execution")
    worker_stop()
}

// Run a single step
function do_step() {
    if (build_time === null) {send_err("Cannot step, not built!"); return -1;}
    if (libcarve._carve_is_halted(state) == 1) {send_err("Cannot step, state is halted!"); return -1;}
    if (verbosity > 0 && (update_time > build_time)) send_warn("Stepping, but build not updated!")
    if (verbosity > 2) send_meta("Performed step")
    libcarve._carve_exec_single(state)
    update_ui()
    return 0;
}

// Do the 'File / Open' dialogue, which lets the user select a file and uses that as the source code
function do_file_open() {
    let elem = $("#file-select")
    console.log(elem)
    
    elem.trigger('click')
    elem.on('change', function() {
        elem[0].files[0].text().then(function (src) {
            // Change ace editor contents
            editor.setValue(src)
        })
        elem.off('change')
    })
}

// Do the 'File / Save' dialogue, which lets the user select a destination file to save the current source code
//   as
function do_file_save() {
    let fname = 'src.s'
    let src = editor.getValue()

    let elem = document.createElement('a')
    elem.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(src))
    elem.setAttribute('download', fname)
  
    elem.style.display = 'none'
    document.body.appendChild(elem)
  
    elem.click()
    document.body.removeChild(elem)
}

// Do the 'File / New' dialogue, which lets the user reset the current contents
function do_file_new() {
    editor.setValue("")
}

// send messages to console
function send_meta(msg, do_head=true) {
    let head = ""
    if (do_head) head = "CARVE: "
    term.echo(`[[;${COL_META};]${head}${msg}]`)
}

function send_warn(msg, do_head=true) {
    let head = ""
    if (do_head) head = "CARVE_WARN: "
    term.echo(`[[;${COL_WARN};]${head}${msg}]`)
}

function send_err(msg, do_head=true) {
    let head = ""
    if (do_head) head = "CARVE_ERR: "
    term.echo(`[[;${COL_ERR};]${head}${msg}]`)
}