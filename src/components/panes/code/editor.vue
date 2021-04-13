<template>
    <AceEditor 
        @init="editorInit" 
        lang="javascript" 
        theme="monokai"
        width="100%" 
        height="100%"
        :options="{
            enableBasicAutocompletion: true,
            fontSize: 14,
            highlightActiveLine: true,
            enableSnippets: true,
            showLineNumbers: true,
            tabSize: 2,
            showPrintMargin: false,
            showGutter: true,
        }"
        :commands="[
            {
                name: 'save',
                bindKey: { win: 'Ctrl-s', mac: 'Command-s' },
                exec: dataSumit,
                readOnly: true,
            },
        ]"
        />
</template>

<script>
import AceEditor from 'vuejs-ace-editor';

export default {
  name: "editor",
  components: {
    AceEditor,
  },
methods: {
    dataSumit() {
        alert("le epic file saved moment");
    },
    editorInit: function () {
        require('brace/ext/language_tools') //language extension prerequsite...
        require('brace/mode/html')                
        require('brace/mode/javascript')    //language
        require('brace/mode/less')
        require('brace/theme/monokai')
        require('brace/snippets/javascript') //snippet
    }
  },
};
</script>