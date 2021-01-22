/* pre.js - pre-amble for compiled Emscripten module
 * 
 * @author: Cade Brown <cade@kscript.org>
 */

/* This will be the main module loaded in the website */
var Module = {
    /* Custom lists of callbacks to call when a line is written to either 
     *   stdout or stderr
     */
    '_write_stdout': [],
    '_write_stderr': [],

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
    }
};
