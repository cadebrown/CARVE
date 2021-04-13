<template>
    <div id="main_bar" class="toolbar">
        <span id="logo">
            <table>
                <tr>
                    <td>
                        Logo here
                    </td>
                    <td>
                        <vue-file-toolbar-menu :content="bar" />
                    </td>
                </tr>
            </table>
        </span>
    </div>
</template>

<script>
import VueFileToolbarMenu from 'vue-file-toolbar-menu'

export default {
  name: 'main_layout',
  components: {
    VueFileToolbarMenu,
  },
  computed: {
    bar () {
      return [
        {
          text: "File",
          menu: [
            { text: "Item 1", click: () => alert("Action 1") },
            { text: "Item 2", click: () => alert("Action 2") }
          ],
        },
        {
          text: "Options",
          menu: [
            { text: "Item 1", click: () => alert("Action 1") },
            { text: "Item 2", click: () => alert("Action 2") }
          ],
        },
        {
          text: "Help",
          menu: [
            { text: "Item 1", click: () => alert("Action 1") },
            { text: "Item 2", click: () => alert("Action 2") }
          ],
        },
      ]
    }
  }
}
</script>

<style>
#main_bar {
  width: calc(100%);
}
</style>