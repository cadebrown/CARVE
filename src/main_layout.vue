<template>
  <splitpanes id="layout" class="default-theme" horizontal>

    <pane class="layout-pane" min-size="20">
      <splitpanes class="default-theme">

        <pane class="layout-pane" min-size="40" size="80">
          <splitpanes class="default-theme" style="height: calc(100% - 45px);">

            <pane class="layout-pane" min-size="30">
              <AceEditor 
                v-model="content"
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
            </pane>

            <pane class="layout-pane" min-size="10" size="20">
              <span>2</span>
            </pane>

          </splitpanes>
          <div id="execution-bar" class="toolbar">
            <vue-file-toolbar-menu :content="my_menu" />
            <input type="range" min="1" max="100" value="50" class="slider" id="myRange">
            hello
          </div>
        </pane>

        <pane class="layout-pane" min-size="20">
          <span>3</span>
        </pane>
      </splitpanes>

    </pane>

    <pane class="layout-pane" min-size="10" size="40">
      <span>4</span>
    </pane>
  </splitpanes>
</template>

<script>
import { Splitpanes, Pane } from 'splitpanes';
import AceEditor from 'vuejs-ace-editor';
import 'splitpanes/dist/splitpanes.css';
import VueFileToolbarMenu from 'vue-file-toolbar-menu'

export default {
  name: 'main_layout',
  components: {
    Splitpanes,
    Pane,
    AceEditor,
    VueFileToolbarMenu
  },
  methods: {
        dataSumit() {
            //code here
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
  computed: {
    my_menu () {
      return [
        {
          text: "Reassemble",
          click: () => { console.log("Sure pal... I'll compile it... Just look away for me real quick...") }
        },
        {
          text: "▶ Run",
        },
        {
          text: "⏸ Pause",
        },
        {
          text: "⏹ Stop",
        },
      ]
    }
  }
}
</script>

<style>
#main_layout {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  padding: 4px 4px 4px 4px;
}

#layout {
  height: calc(100vh - 54px);
  width: calc(100vw - 8px);
  padding: 4px 4px 4px 4px;
}

#execution-bar {
  width: calc(100%);
}

.toolbar {
  height: 45px;
  background-color: green;
  z-index: 10;
}

.layout-pane {
  background-color: aqua;
}

.bar-button {
  background-color: rgba(0, 0, 0, 0.1);
  border: 3px solid rgba(0, 0, 0, 0.3);
  margin-left: 4px;
  margin-right: 4px;
  margin-top: 1px;
}

.bar-button:hover {
  background-color: rgba(255, 255, 255, 0.5) !important;
  cursor: pointer;
}

#speed-slider {
  background-color: rgba(0, 0, 0, 0) !important;
  border: 3px solid rgba(0, 0, 0, 0) !important;
  cursor: default !important;
}

</style>
