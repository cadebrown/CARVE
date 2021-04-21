function load_ui() {
    const update_ace = () => {editor.resize(); editor.renderer.updateFull();};
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
        onDragEnd: () => {update_ace();},
        columnMinSize: $(window).width() / 10,
        rowMinSize: $(window).height() / 10,
    });
    Split({ // gutters specified in options
        columnGutters: [{
            track: 1,
            element: $('.reg_bar')[0],
        }],
        onDragEnd: () => {update_ace();},
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
    //editor.setAutoScrollEditorIntoView(true);
}