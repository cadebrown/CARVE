/* pre.js - preamble for compiled Emscripten module
 * 
 * @author: Cade Brown <cade@utk.edu>
 */

/* This will be the main module loaded in the website */
var Module = {
    /* Custom lists of callbacks to call when a line is written to either 
     *   stdout or stderr
     */
    '_write_stdout': [],
    '_write_stderr': [],

    '_buf_stdin': '',
    '_buf_stdin_i': 0,

    /* These names are special, and they are the callbacks (see: https://emscripten.org/docs/api_reference/module.html) */
    'print': function(text) {
        for (var i = 0; i < Module._write_stdout.length; ++i) {
            Module._write_stdout[i](text);
        } 
    },
    'printErr': function(text) {
        for (var i = 0; i < Module._write_stderr.length; ++i) {
            Module._write_stderr[i](text);
        }
    },

    /* Called just before it is ran */
    preRun: function() {
        function stdin() {

            if (Module._buf_stdin_i >= Module._buf_stdin.length) {
                // no input
                return null;
            } else {
                // return next character
                var res = Module._buf_stdin.charCodeAt(Module._buf_stdin_i);
                Module._buf_stdin_i++;
                if (Module._buf_stdin_i >= 256) {
                    Module._buf_stdin_i -= 256;
                    Module._buf_stdin = Module._buf_stdin.slice(256);
                }
                return res;
            }
        }
        FS.init(stdin);
    }

};
