define("ace/mode/riscv_highlight_rules",["require","exports","module","ace/lib/oop","ace/mode/text_highlight_rules"], function(require, exports, module) {
    var oop = require("../lib/oop");
    var TextHighlightRules = require("./text_highlight_rules").TextHighlightRules;
    
    var riscvHighlightRules = function() {
        this.$rules = { start: 
           [ { token: 'keyword.control.assembly',
               /* Put all instructions here */
               regex: '\\b(?:a|b|c)\\b',
               caseInsensitive: true },
             { token: 'variable.parameter.register.assembly',
               /* Put all registers here */
               regex: '\\b(?:x[0-9]|x[1-2][0-9]|x3[0-1]|zero|ra|sp|gp|tp|t[0-6]|s[0-1]|fp|a[0-9]|s[0-9]|s1[0-1]|f[0-9]|f[1-2][0-9]|f3[0-1]|ft[0-9]|ft1[0-1]|fs[0-9]|fs1[0-1]|fa[0-7])\\b',
               caseInsensitive: true },
             { token: 'constant.character.decimal.assembly',
               regex: '\\b[0-9]+\\b' },
             { token: 'constant.character.hexadecimal.assembly',
               regex: '\\b0x[A-F0-9]+\\b',
               caseInsensitive: true },
             { token: 'constant.character.hexadecimal.assembly',
               regex: '\\b[A-F0-9]+h\\b',
               caseInsensitive: true },
             { token: 'string.assembly', regex: /'([^\\']|\\.)*'/ },
             { token: 'string.assembly', regex: /"([^\\"]|\\.)*"/ },
             { token: 'support.function.directive.assembly', regex: '\\.\\S+' },
             { token: 'entity.name.function.assembly', regex: '^\\s*%%[\\w.]+?:$' },
             { token: 'entity.name.function.assembly', regex: '^\\s*%\\$[\\w.]+?:$' },
             { token: 'entity.name.function.assembly', regex: '^\\s*[\\w.]+?:' },
             { token: 'entity.name.function.assembly', regex: '^\\s*[\\w.]+?\\b' },
             { token: 'comment.assembly', regex: '#.*$' } ] 
        };
        
        this.normalizeRules();
    };
    
    riscvHighlightRules.metaData = { fileTypes: [ 'asm', 's' ],
          name: 'RISC-V',
          scopeName: 'source.assembly' };
    
    
    oop.inherits(riscvHighlightRules, TextHighlightRules);
    
    exports.riscvHighlightRules = riscvHighlightRules;
    });
    
    define("ace/mode/folding/coffee",["require","exports","module","ace/lib/oop","ace/mode/folding/fold_mode","ace/range"], function(require, exports, module) {
    "use strict";
    
    var oop = require("../../lib/oop");
    var BaseFoldMode = require("./fold_mode").FoldMode;
    var Range = require("../../range").Range;
    
    var FoldMode = exports.FoldMode = function() {};
    oop.inherits(FoldMode, BaseFoldMode);
    
    (function() {
    
        this.getFoldWidgetRange = function(session, foldStyle, row) {
            var range = this.indentationBlock(session, row);
            if (range)
                return range;
    
            var re = /\S/;
            var line = session.getLine(row);
            var startLevel = line.search(re);
            if (startLevel == -1 || line[startLevel] != "#")
                return;
    
            var startColumn = line.length;
            var maxRow = session.getLength();
            var startRow = row;
            var endRow = row;
    
            while (++row < maxRow) {
                line = session.getLine(row);
                var level = line.search(re);
    
                if (level == -1)
                    continue;
    
                if (line[level] != "#")
                    break;
    
                endRow = row;
            }
    
            if (endRow > startRow) {
                var endColumn = session.getLine(endRow).length;
                return new Range(startRow, startColumn, endRow, endColumn);
            }
        };
        this.getFoldWidget = function(session, foldStyle, row) {
            var line = session.getLine(row);
            var indent = line.search(/\S/);
            var next = session.getLine(row + 1);
            var prev = session.getLine(row - 1);
            var prevIndent = prev.search(/\S/);
            var nextIndent = next.search(/\S/);
    
            if (indent == -1) {
                session.foldWidgets[row - 1] = prevIndent!= -1 && prevIndent < nextIndent ? "start" : "";
                return "";
            }
            if (prevIndent == -1) {
                if (indent == nextIndent && line[indent] == "#" && next[indent] == "#") {
                    session.foldWidgets[row - 1] = "";
                    session.foldWidgets[row + 1] = "";
                    return "start";
                }
            } else if (prevIndent == indent && line[indent] == "#" && prev[indent] == "#") {
                if (session.getLine(row - 2).search(/\S/) == -1) {
                    session.foldWidgets[row - 1] = "start";
                    session.foldWidgets[row + 1] = "";
                    return "";
                }
            }
    
            if (prevIndent!= -1 && prevIndent < indent)
                session.foldWidgets[row - 1] = "start";
            else
                session.foldWidgets[row - 1] = "";
    
            if (indent < nextIndent)
                return "start";
            else
                return "";
        };
    
    }).call(FoldMode.prototype);
    
    });
    
    define("ace/mode/riscv",["require","exports","module","ace/lib/oop","ace/mode/text","ace/mode/riscv_highlight_rules","ace/mode/folding/coffee"], function(require, exports, module) {
    "use strict";
    
    var oop = require("../lib/oop");
    var TextMode = require("./text").Mode;
    var riscvHighlightRules = require("./riscv_highlight_rules").riscvHighlightRules;
    var FoldMode = require("./folding/coffee").FoldMode;
    
    var Mode = function() {
        this.HighlightRules = riscvHighlightRules;
        this.foldingRules = new FoldMode();
        this.$behaviour = this.$defaultBehaviour;
    };
    oop.inherits(Mode, TextMode);
    
    (function() {
        this.lineCommentStart = [";"];
        this.$id = "ace/mode/riscv";
    }).call(Mode.prototype);
    
    exports.Mode = Mode;
    });                (function() {
                        window.require(["ace/mode/riscv"], function(m) {
                            if (typeof module == "object" && typeof exports == "object" && module) {
                                module.exports = m;
                            }
                        });
                    })();
                