<template>
  <splitpanes class="default-theme" horizontal style="height: 400px">
    <pane min-size="20" max-size="70">
      <span>1</span>
    </pane>
    <pane min-size="20">
      <span>2</span>
    </pane>
    <pane min-size="20">
      <span>3</span>
    </pane>
  </splitpanes>
</template>

<script>
  import { Splitpanes, Pane } from 'splitpanes'
  import 'splitpanes/dist/splitpanes.css'

  export default {
    name: 'HelloWorld',
    components: {
      Splitpanes,
      Pane
    }
  }
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h3 {
  margin: 40px 0 0;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #42b983;
}
</style>
