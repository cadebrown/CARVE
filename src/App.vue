<template>
    <div>
        <toolbar></toolbar>
        <layout></layout>
    </div>
</template>

<script>
import toolbar from '@/components/main_toolbar';
import layout from '@/components/layout';

export default {
  name: 'app',
  components: {
    layout,
    toolbar,
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