<template>
  <splitpanes id="layout" class="default-theme" horizontal>

    <pane class="layout-pane" min-size="20">
      <splitpanes class="default-theme">

        <pane class="layout-pane" min-size="40" size="80">
          <splitpanes class="default-theme" style="height: calc(100% - 45px);">
            <pane class="layout-pane" min-size="30">
                <editor></editor>
            </pane>

            <pane class="layout-pane" min-size="10" size="20">
              <exe></exe>
            </pane>

          </splitpanes>
            <cbar></cbar>
        </pane>

        <pane class="layout-pane" min-size="20">
          <registers></registers>
        </pane>
      </splitpanes>

    </pane>

    <pane class="layout-pane" min-size="10" size="40">
      <console></console>
    </pane>
  </splitpanes>
</template>

<script>
import { Splitpanes, Pane } from 'splitpanes';
import editor from '@/components/panes/code/editor';
import exe from '@/components/panes/code/exe';
import cbar from '@/components/panes/code/code_toolbar';
import console from '@/components/panes/console';
import registers from '@/components/panes/reg_tab';

import 'splitpanes/dist/splitpanes.css';

export default {
  name: 'main_layout',
  components: {
    Splitpanes,
    Pane,
    editor,
    console,
    exe,
    registers,
    cbar
  },
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