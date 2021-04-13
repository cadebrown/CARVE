import Vue from 'vue'
import app from './App.vue'
import $ from 'jquery'

// Our module
Vue.prototype.$libcarve = null

new Vue({
  render: h => h(app),
}).$mount('#app')
