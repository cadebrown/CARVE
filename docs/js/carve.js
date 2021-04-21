// CARVE WASM library handle
let libcarve = null

// carve::State object, the current state
let state = null
// carve::Program object, the current program being compiled
let program = null


// Ace editor
let editor = null

// Terminal/console output
let term = null

// Temporary buffer for register strings
let tmpbuf = null
let tmpbuf_len = 1024



loadlibcarve().then(function (_libcarve) {
    libcarve = _libcarve;
    
    
    /* ACE EDITOR */
    // Initialize the editor
    editor = ace.edit("ace_editor", {
        selectionStyle: "text"
    });
    ace.config.set('basePath', '/js')
    editor.setTheme('ace/theme/monokai')
    editor.setOptions({
        fontSize: '13pt',
    })
    editor.getSession().setMode('ace/mode/riscv')


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
    libcarve._carve_init();

    state = libcarve._carve_state_new()

    $(document).ready(function () {

        // Set the callback for when the selector is changed
        $("#tab_type_sel").change(function() {

            // Table of registers, the element that should be modified
            let elem = $("#reg-table")
            

            // What type of registers/view?
            let type = this.value

            // List of registers of this type
            let regs = REG_TAB[type]

            
            let out = "<table class='register_table' id='reg_table'>"
        
            // Add header row
            out += "<tr><td class='register_table reg_id t_head'>REG</td><td class='register_table reg_name t_head'>NAME</td><td class='register_table reg_hex t_head'>HEX</td><td class='register_table reg_dec t_head'>DEC</td></tr>"

            const make_row = (reg, abi, desc, saver) => {
                return '<tr id="row_' + (type+reg) + '"><td id="reg_' + (type) + 'lab' + (reg) + '"class="register_table reg_' + (type) + 'id">' + (reg) + '</td><td id="reg_name' + (reg) + '"class="register_table reg_name">' + (abi) + '</td><td class="register_table reg_hex" id="reg_' + (type+reg) + '_hex">0</td><td class="register_table reg_dec" id="reg_' + (type+reg) + '_dec">0</td></tr>'
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
        
                tippy('#row_'+type+regs[i][0], {
                    content: content,
                    placement: 'left',
                    allowHTML: true,
                    interactive: true,
                });
                out += make_row(...regs[i]);
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
            rowMinSize: $(window).height() / 10,
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
            'options': [['autocomplete', 'autocomplete <input type="checkbox" id="autocomplete_sel">']],
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
                arrow: false,
                offset: [0, 4],
            });
        }


        // Simulate the change event for the first time
        $("#tab_type_sel").change()
    })
})


/** Engine Functions **/

// Re-compiles and updates the program
function update_program() {
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

    update_ui()
}

// Updates the UI elements for the engine's current register values
function update_ui() {
    for (let i = 0; i < 32; ++i) {
        libcarve._carve_getrx(state, tmpbuf_len, tmpbuf, i, 16)
        $('#reg_' + "intx" + i.toString() + '_hex').text(libcarve.UTF8ToString(tmpbuf))

        libcarve._carve_getrx(state, tmpbuf_len, tmpbuf, i, 10)
        $('#reg_' + "intx" + i.toString() + '_dec').text(libcarve.UTF8ToString(tmpbuf))
    }
}



/** UI Functions **/


// Run the entire program
function do_run() {
    update_program()
    libcarve._carve_exec_all(state)
    update_ui()
}

// Run a single step
function do_step() {
    libcarve._carve_exec_single(state)
    update_ui()
}


// Do the 'File / Open' dialogue, which lets the user select a file and uses that as the source code
function do_file_open() {
    let elem = $("#file-select")
    
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
